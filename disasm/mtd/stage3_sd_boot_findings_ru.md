# Находки по Stage 3 и SD-загрузке

Дата: 2026-06-03.

Этот документ фиксирует текущие подтвержденные выводы по третьей стадии
загрузчика, SD-карте и образу, который реально прошел Stage 3 на устройстве.

## Что ищет Stage 3

Stage 3 не ищет Debian-раздел и не проверяет явный фиксированный размер SD.
В найденном коде главный критерий - доступность boot resources, описанных в
DTB.

Функция `FUN_89da9474` ищет ресурс в таком порядке:

1. Сначала как внутренний MTD partition. Если метка найдена, строится путь
   `/dev/mtdblock%d`.
2. Затем через DTB-узел `/hcrtos/external_partitions`. В этом режиме ресурс
   сопоставляется с `/dev/mmcblk0p%d`.
3. Затем через DTB-узел `/hcrtos/external_files`. В этом режиме Stage 3 читает
   `part-num`, `path-prefix`, `part%d-label`, `part%d-filename` и открывает файл
   на SD.

В нашем DTB используется `external_files`:

```dts
external_files {
    part-num = <0x4>;
    path-prefix = "cubegm";
    part1-label = "dtb";
    part1-filename = "dtb.bin";
    part2-label = "avp";
    part2-filename = "avp.uImage";
    part3-label = "linux";
    part3-filename = "vmlinux.uImage";
    part4-label = "logo";
    part4-filename = "xgame-logo.bmp";
};
```

Практически это означает, что Stage 3 должен прочитать:

```text
cubegm/dtb.bin
cubegm/avp.uImage
cubegm/vmlinux.uImage
cubegm/xgame-logo.bmp
```

Ресурс `initramdisk` тоже запрашивается в main boot flow, но в нашем DTB он не
описан. Если он не найден, код пропускает этот шаг, поэтому сейчас он считается
опциональным.

## Экран "Please Insert TF Card"

Картинка находится во внутреннем MTD2 EROMFS как `popup.bmp.gz`. Stage 3
показывает ее не потому, что сама картинка отсутствует, а когда найденное
описание внешнего ресурса не удается открыть или подготовить.

Найденная цепочка:

```text
FUN_89da9474
  -> retry external resource
  -> after timer 2000: "show popup"
  -> FUN_89da98ec(0)
  -> try eromfs / eromfs2 / eromfs3
  -> showlogo /eromfs/popup.bmp
  -> fallback /eromfs/popup.bmp.gz
  -> fallback /eromfs/popup.hc
```

Высоковероятные причины popup:

- карта физически не детектится;
- ранний MMC init не создает ожидаемое устройство;
- первый FAT/VFAT-раздел не читается Stage 3;
- путь `cubegm/...` отсутствует или файл не открывается;
- измененный DTB меняет ранние условия загрузки;
- дополнительная разметка мешает простому раннему parser'у MBR/FAT.

## Подтвержденный рабочий образ

Был создан консервативный образ:

```text
local_artifacts/r36sx_stage3_pass_stockfat.img
```

`local_artifacts/` не кладется в Git. Образ воспроизводится скриптом:

```text
tools/build_stage3_pass_stock_sd_image_wsl.sh
```

Проверяется скриптом:

```text
tools/verify_stage3_pass_stock_sd_image_wsl.sh
```

Параметры образа:

```text
size:             2 GiB
partition table:  msdos / MBR
partition 1:      FAT32
partition start:  4 MiB
partition size:   2044 MiB
partition flags:  boot, lba
label:            R36SXBOOT
```

На FAT были скопированы:

```text
cubegm/
rootfs/
MIPS_NATIVE/
Chkdsk.bat
chkdsk.exe
Movie/
Ebook/
Music/
```

Ключевой момент: `cubegm/dtb.bin` оставлен stock. Его bootargs не менялись:

```text
root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc console=tty1 earlycon= no_console_suspend noirqdebug
```

Проверка WSL подтвердила, что образ содержит все Stage 3 ресурсы, а SHA256
файлов совпадает с исходными `disk_image/cubegm/*`:

```text
cubegm/dtb.bin
cubegm/avp.uImage
cubegm/vmlinux.uImage
cubegm/xgame-logo.bmp
```

## Результат на железе

Пользователь записал образ на SD-карту и устройство загрузилось. Вместо экрана
`Please Insert TF Card` загрузочная цепочка дошла до TinyMC.

Это подтверждает:

1. Stage 3 принимает MBR-образ на 2 GiB.
2. Первый раздел может быть FAT32 со стартом 4 MiB.
3. Для прохождения Stage 3 не требуется оригинальный полный размер карты.
4. Stock `dtb.bin`, `avp.uImage`, `vmlinux.uImage` и `xgame-logo.bmp` в
   `cubegm/` достаточны для перехода через внешний resource lookup.
5. Stock `root=/dev/ram0 init=/linuxrc` схема продолжает запускать userspace,
   который затем использует SD/FAT runtime.

Рабочая цепочка сейчас выглядит так:

```text
SPI NOR / MTD boot
  -> Stage 2 распаковывает LZMA Stage 3 в RAM по 0x89da0000
  -> Stage 3 читает external_files из DTB
  -> FAT32:/cubegm/dtb.bin
  -> FAT32:/cubegm/avp.uImage
  -> FAT32:/cubegm/vmlinux.uImage
  -> FAT32:/cubegm/xgame-logo.bmp
  -> Linux root=/dev/ram0 init=/linuxrc
  -> SD/FAT runtime overlays /bin, /lib, /usr, /etc
  -> hcdaemon / icube route
  -> TinyMC
```

## Что это говорит про неудачные Debian-образы

Ранее варианты Debian-образов с измененными bootargs, вторым ext4-разделом или
минимальным FAT-набором не были приняты firmware. После успешного single-FAT
теста наиболее вероятно, что ломало не отсутствие конкретного boot-файла само по
себе, а один из факторов:

- измененный `dtb.bin`;
- добавленный второй раздел;
- недостаточно stock-like FAT runtime;
- отличие порядка/геометрии разметки;
- слишком ранняя попытка перейти на `root=/dev/mmcblk0p2`.

Пока не доказано, что второй ext4-раздел сам по себе ломает Stage 3. Это нужно
проверять отдельно на базе уже рабочего FAT-образа.

## Следующий безопасный план

1. Сохранить рабочий single-FAT образ как baseline.
2. Создать вариант с тем же первым FAT32-разделом и тем же stock `dtb.bin`, но
   добавить второй ext4-раздел.
3. Проверить, проходит ли такой образ Stage 3 и доходит ли до TinyMC.
4. Если проходит, использовать stock userspace/TinyMC/Shell для запуска Debian
   как chroot или pivot-test с `/dev/mmcblk0p2`.
5. Только после этого пробовать измененный DTB с
   `root=/dev/mmcblk0p2 rootfstype=ext4 rootwait`.

## Открытые вопросы

- Точная реализация helper'а, который в `external_files` открывает SD path,
  еще не полностью разобрана.
- Не доказано, принимает ли Stage 3 карту с двумя разделами.
- Не доказано, можно ли сразу грузить Debian rootfs без initramfs/vendor
  `/linuxrc`.
- Не доказано, какие минимальные stock runtime-файлы нужны после прохождения
  Stage 3; текущий рабочий образ специально оставляет достаточно широкий набор.
