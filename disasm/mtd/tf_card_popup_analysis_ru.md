# Анализ экрана "Please Insert TF Card"

Дата: 2026-06-03.

## Картинка

Картинка с сообщением "Please Insert TF Card" найдена в MTD2 `eromfs`:

```text
mtd2_eromfs.bin
  popup.bmp.gz       offset 0x0080, size 0x22ae
  batterylow.bmp.gz  offset 0x2360, size 0x15b6
```

В полном NOR dump тот же ROMFS начинается в `mtd0_nor.bin` с `0x6c000`, поэтому
gzip-данные popup-картинки находятся в MTD0 по `0x6c080`.

`popup.bmp.gz` распаковывается в 640x480 32-bit BMP. Это и есть экран с
индикатором загрузки и текстом `Please Insert TF Card`.

## DTB-признаки

В DTB есть отдельный включенный узел:

```dts
Unplug_card_display {
    status = "okay";
};
```

Рядом в `panel` есть вход детекта SD:

```dts
sdio-det = <0x6d>;
```

А RTOS/MMC-конфигурация задает контроллер карты и задержку детекта:

```dts
mmc {
    hcrtos-compatible = "hichip,dw-mshc";
    reg = <0x1884c000 0x2000>;
    card-detect-delay = <0xc8>;
    bus-width = <0x4>;
    cap-sd-highspeed;
    sd-uhs-sdr12;
    sd-uhs-sdr25;
};
```

Тот же DTB перечисляет SD boot-файлы:

```dts
external_files {
    path-prefix = "cubegm";
    part1-filename = "dtb.bin";
    part2-filename = "avp.uImage";
    part3-filename = "vmlinux.uImage";
    part4-filename = "xgame-logo.bmp";
};
```

## Что видно в MTD1

Уже декомпилированные first/stage2 части MTD1 не содержат строк `popup.bmp.gz`,
`Please Insert TF Card`, `cubegm`, `FAT` или `mmc` как читаемых путей.

MTD1 stage2 декодирует следующий packed payload:

```text
source MTD offset: 0x5e48
packed length:    0x62f4f
decoded address:  0x89da0000
```

Ключевой вызов из Ghidra:

```c
FUN_89eb2dcc(0x89eb2e48, 0x62f4f, 0, 0, &LAB_89da0000, 0, 0, auStack_10, 0x89eb0300);
```

Заголовок packed payload:

```text
5d 00 00 80 00 3c e3 10 00 00 00 00 00 00 00 69
```

Это LZMA-Alone stream. Первые 13 байт задают properties `0x5d`, dictionary
`0x00800000` и unpacked size `0x0010e33c`.

Распаковка дала:

```text
stage3_89da0000.lzma  size 0x62f4f   sha256 378ae082fbb6c2b52a9cfeac3e8f6ebb4248a2436f4cadc41110b98c9cc79680
stage3_89da0000.bin   size 0x10e33c  sha256 b9a9d1ae074bcd68109161c1219fe98b8c4f7e3fd70045b394dd9aa4ab15981d
```

Использованный скрипт: `disasm/mtd/unpack_stage3_lzma.py`.

Stage2 распаковывает этот LZMA stream функциями:

```text
stage2_payload_header_parse
stage2_stream_copy_or_decode_loop
stage2_payload_decode_dispatch
FUN_89eb2dcc
```

## Найденная логика показа

После распаковки stage3 точное место показа найдено в коде по базе
`0x89da0000`:

- `FUN_89da9474` ищет boot-ресурс сначала как MTD partition, затем через
  `/hcrtos/external_partitions`, затем через `/hcrtos/external_files`.
- В external-files режиме она читает `part-num`, `part%d-label`,
  `path-prefix`, `part%d-filename` и строит путь на SD.
- В external-partitions режиме она строит `/dev/mmcblk0p%d`.
- Если выбранный ресурс или раздел все еще недоступен, функция входит в цикл
  повторной проверки. После таймера `2000` она печатает строку `show popup` и
  вызывает `FUN_89da98ec(0)`.
- `FUN_89da98ec(0)` пробует `eromfs`, `eromfs2`, `eromfs3`.
- `FUN_89da97c0` монтирует ROMFS и запускает `showlogo` для
  `/eromfs/popup.bmp`, затем fallback `/eromfs/popup.bmp.gz`, затем
  `/eromfs/popup.hc`.

То есть экран показывается не потому, что сама popup-картинка не найдена, а как
индикатор ожидания/ошибки доступа к внешнему SD boot resource.

## Условия

С высокой уверенностью popup появляется, если `FUN_89da9474` нашла описание
ресурса, но не может получить сам ресурс:

- `/hcrtos/external_partitions` указывает на `part%d-label`, но
  `/dev/mmcblk0p%d` не становится доступным;
- `/hcrtos/external_files` указывает на `part%d-label`, `path-prefix` и
  `part%d-filename`, но соответствующий файл на SD не открывается;
- карта физически не детектится или MMC init не создает нужное
  `/dev/mmcblk0p%d`;
- FAT/VFAT первого раздела не читается достаточно рано для RTOS loader;
- один из boot resources не найден или не читается.

В main boot flow stage3 затем запрашивает ресурсы `dtb`, `initramdisk`, `avp` и
`linux`. Через `external_files` им соответствуют SD-файлы вроде `dtb.bin`,
`avp.uImage`, `vmlinux.uImage` и `xgame-logo.bmp`.

## Почему это важно для наших образов

Если физическая карта вставлена, но firmware все равно показывает этот экран,
то проблема не обязательно в наличии отдельных файлов на FAT. Возможны:

- новая геометрия MBR/разделов не нравится раннему loader;
- loader читает только один FAT-раздел ожидаемого типа/размера;
- наличие второго ext4-раздела ломает его простую FAT/MBR логику;
- измененный `dtb.bin` доходит до loader, но затем ломает ранние условия;
- loader не доходит даже до чтения `external_files`, а падает на card/FAT init.

## Подтвержденный тест

После анализа был собран консервативный образ
`local_artifacts/r36sx_stage3_pass_stockfat.img` через
`tools/build_stage3_pass_stock_sd_image_wsl.sh`.

Параметры:

```text
2 GiB raw image
MBR / msdos partition table
partition 1: FAT32, start 4 MiB, flags boot,lba
stock cubegm/dtb.bin with root=/dev/ram0 init=/linuxrc
```

На FAT-раздел скопированы stock-like `cubegm/`, `rootfs/`, `MIPS_NATIVE/`,
`Chkdsk.*`, `Movie/`, `Ebook/` и `Music/`. Проверка подтвердила наличие и
совпадение SHA256 для:

```text
cubegm/dtb.bin
cubegm/avp.uImage
cubegm/vmlinux.uImage
cubegm/xgame-logo.bmp
```

Пользователь записал этот образ на SD-карту. Устройство загрузилось и дошло до
TinyMC. Это подтверждает, что Stage 3 принимает такой MBR/FAT32 baseline и что
оригинальный полный размер SD-карты не является обязательным условием для
прохождения external resource lookup.

Подробный итог: `disasm/mtd/stage3_sd_boot_findings_ru.md`.

## Что еще стоит проверить

Точное низкоуровневое место, где MMC/card-detect решает "карты нет", находится
ниже по стеку MMC-драйвера. Stage3 уже содержит строки и код `mmc`, `sdio`,
`card-detect-delay`, `dw_mmc_hc`, но ветка popup в найденном boot flow
срабатывает на уровне недоступности external resource.

Следующий технический шаг: глубже декомпилировать функции, которые вызываются
из `FUN_89da9474`:

```text
FUN_89da926c
FUN_89de0894
FUN_89ddc438
FUN_89ddc3c0
```

## Практические тесты для образов

Для проверки гипотез лучше идти от наиболее консервативного к рискованному:

1. Оригинальная single-FAT геометрия без второго раздела, но с минимальным
   видимым изменением, например только marker-файл.
2. Single-FAT образ с полностью штатными `cubegm`, `rootfs`, `/bin`, `/lib`,
   `/etc`, `/usr`.
3. Тот же single-FAT, но с нашим `MIPS_NATIVE/shell`.
4. Эти single-FAT условия теперь подтверждены через рабочий
   `r36sx_stage3_pass_stockfat.img`, который загрузился до TinyMC.
5. Следующий тест: добавить второй раздел, не меняя stock `dtb.bin`.
6. Только после подтверждения второго раздела возвращаться к измененному
   `dtb.bin` с `root=/dev/mmcblk0p2`.
