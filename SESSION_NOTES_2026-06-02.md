# Итоги чата от 2026-06-02

Этот документ фиксирует, что было сделано в ходе текущего чата: разбор DTB/DTS,
оценка загрузки Linux/Debian, проверка ABI/FPU и добавление native-программы
`Shell` для устройства.

## DTB/DTS

- Найден DTB устройства: `disk_image/cubegm/dtb.bin`.
- DTB декомпилирован через Linux/WSL `dtc` в:
  `disk_image/cubegm/dtb.dts`.
- DTS был проверен обратной компиляцией в DTB. Остались только исходные
  предупреждения по `reg` у `/hcrtos/sfspi/*`.
- В `dtb.dts` добавлены комментарии по устройствам на основе локального анализа
  и открытых источников: CPU, SoC, UART, AVP/hcRTOS, SPI NOR, USB, MMC, GPIO,
  ADC, display/panel.
- Эти DTS-комментарии уже закоммичены:
  `c46e0ba Add annotated HC16xx device tree source`.

## Образ диска и загрузка Linux

- `disk_image` сейчас является распакованной структурой SD-карты/прошивки, а не
  единым raw disk image.
- Основные boot-артефакты в `disk_image/cubegm`:
  - `vmlinux.uImage` - U-Boot legacy Linux/MIPS kernel image.
  - `advapi32.dll` - тоже U-Boot legacy Linux/MIPS kernel image с похожим
    назначением, несмотря на расширение DLL.
  - `avp.uImage` - firmware image для AVP/hcRTOS.
  - `dtb.bin` - device tree.
  - `Bubbles.scr` - тоже DTB-подобный blob с отличающейся memory map.
- В DTS `external_files` указывают на:
  - `cubegm/dtb.bin`
  - `cubegm/avp.uImage`
  - `cubegm/vmlinux.uImage`
  - `cubegm/xgame-logo.bmp`
- Текущие `bootargs` используют ramfs:
  `root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc console=tty1 ...`
- В ядре/модулях найдена поддержка FAT/VFAT/MSDOS, MMC/DW-MMC и ext2/ext3/ext4.
  Поэтому SD-карта с небольшим FAT boot-разделом и ext4 root-разделом технически
  выглядит реализуемой.
- Для загрузки с ext4 root потребуется менять bootargs/DTS или initramfs-сценарий:
  стоковая схема всё ещё ожидает `root=/dev/ram0`.

## Debian и ABI

- Стоковые ELF-бинарники и собранные нами native-программы имеют ABI:
  - ELF32 little-endian MIPS.
  - MIPS32r2.
  - O32.
  - hard-float.
  - dynamic linker `/lib/ld.so.1`.
- В стоковой rootfs используется glibc, ранее наблюдался уровень около
  GLIBC 2.20.
- Debian потенциально возможен как `mipsel` hard-float rootfs/chroot, особенно
  на базе Debian Bookworm. Более новые Debian-релизы нужно проверять отдельно:
  поддержка `mipsel` менялась/сокращалась.
- Для полноценного Debian-boot нужны отдельные работы:
  - rootfs под правильный MIPS ABI;
  - bootargs под ext4 root;
  - корректные `/dev`, `/proc`, `/sys`, init;
  - сохранение доступа к vendor `driver.so`/AVP/hcRTOS-части, если нужен экран,
    звук и кнопки.

## Плавающая точка

- Проверены локальные бинарники (`linuxrc`, `rkgame`, `cubevol`, `driver.so`):
  они собраны как hard-float.
- В kernel strings есть признаки работы с FPU, включая строки про FPU revision.
- Вывод: прошивка рассчитана на hard-float ABI; аппаратная FPU весьма вероятна,
  но окончательно подтвердить на железе нужно через:
  - `cat /proc/cpuinfo`
  - `dmesg | grep -i fpu`
- DTS называет CPU как MIPS 74Kc-подобный, а у этой линейки FPU зависит от
  конкретной реализации SoC.

## Shell

Добавлена новая native-программа `Shell` без префикса `r36sx`.

Исходники и сборка:

```text
homebrew/Shell/Shell.c
homebrew/Shell/build_Shell_wsl.sh
homebrew/Shell/README.md
```

Собранные/установленные копии:

```text
homebrew/Shell/Shell
disk_image/MIPS_NATIVE/Shell/Shell
patches/disk_image_patch_Shell/MIPS_NATIVE/Shell/Shell
```

Patch overlay:

```text
patches/disk_image_patch_Shell/MANIFEST.md
patches/disk_image_patch_Shell/MIPS_NATIVE/Shell/README.md
patches/disk_image_patch_Shell/MIPS_NATIVE/common/fonts/
```

Что делает `Shell`:

- Открывает `driver.so` и выводит RGB565 framebuffer через
  `video_driver_disp_frame`.
- Использует общую экранную клавиатуру из `homebrew/common`.
- Рендерит терминал через FreeType и monospace-шрифты из
  `MIPS_NATIVE/common/fonts`.
- Запускает настоящий `/bin/sh -i` через pseudo-terminal.
- Позволяет выполнять обычные Linux-команды, pipes и редиректы, например:

```sh
ls / > /mnt/sdcard/root.txt
dmesg | tail -40 > /mnt/sdcard/dmesg_tail.txt
cat /mnt/sdcard/root.txt
```

- Поддерживает простую VT100/ANSI-отрисовку: цвета, cursor movement, clear
  screen/line, printable UTF-8 через FreeType-глифы.
- Если на прошивке не работает `/dev/ptmx`/`devpts`, `Shell` теперь пытается
  создать `/dev/ptmx`, смонтировать `devpts` на `/dev/pts`, повторить запуск
  PTY, а затем переходит в pipe-backed fallback. В fallback-режиме команды и
  редиректы работают, но полноценного TTY/job-control нет.
- Управление через встроенные кнопки:
  - `FN` - показать/скрыть экранную клавиатуру.
  - `FN + D-pad Up` - сохранить скриншот framebuffer.
  - `FN + X` - выйти.
  - `SELECT` - показать клавиатуру или expand/collapse.
  - При скрытой клавиатуре D-pad отправляет terminal arrow keys.
- Скриншоты добавлены по той же комбинации, что в Pico 286:
  `FN + D-pad Up`. В `Shell` они сохраняются как 24-bit BMP:
  `/mnt/sdcard/MIPS_NATIVE/Shell/screenshots/Shell_YYYYMMDD_HHMMSS_NNN.bmp`.
  Если SD-путь недоступен, используется локальный каталог `screenshots`.

## Физическая USB-клавиатура

- В DTS найдены два USB host-контроллера `hichip,hc16xx-musb` с
  `dr_mode = "host"`.
- В `modules.builtin` и kernel strings найдены:
  - `usbcore`
  - `musb`
  - `hid`
  - `hid-generic`
  - `usbhid`
  - `input-core`
  - `evdev`
- `mdev` включён через `/proc/sys/kernel/hotplug`, значит USB HID-клавиатура
  должна появляться как `/dev/input/event*`, если физический USB host-порт и
  питание работают.
- В `Shell` добавлен evdev-мост:
  - периодически сканирует `/dev/input/event*`;
  - фильтрует keyboard-like устройства по `EV_KEY`, `KEY_A`, `KEY_ENTER`;
  - мапит клавиши физической USB-клавиатуры в PTY `/bin/sh`;
  - поддерживает буквы, цифры, знаки, Enter, Backspace, Tab, Esc, стрелки,
    Home/End/Page, Insert/Delete, F1-F12 и Ctrl-комбинации.
- На железе стоит проверить:

```sh
dmesg
ls /dev/input/event*
```

При успешном подключении `Shell` должен вывести строку вида:

```text
[usb keyboard: /dev/input/eventX]
```

Если клавиатура не определяется, вероятные причины:

- нужен OTG-переходник;
- USB-порт не выдаёт 5V в нужном режиме;
- нужен powered USB hub;
- конкретный порт устройства физически разведён не как host.

## Изменения в общих компонентах

- В `homebrew/common/r36sx_screen_keyboard.c` добавлены пути к common
  monospace-шрифтам:
  `MIPS_NATIVE/common/fonts/*.ttf`.
- Это позволяет экранной клавиатуре использовать те же шрифты, что `Shell`,
  `Tiny MC` и другие native-программы.

## Текущее состояние рабочей копии

На момент создания этого документа есть незакоммиченные изменения:

```text
homebrew/common/r36sx_screen_keyboard.c
homebrew/Shell/
patches/disk_image_patch_Shell/
```

Также в рабочей копии были несвязанные/старые untracked-артефакты, которые этот
чат не менял намеренно:

```text
disasm/
homebrew/pico_286/pico_286.hardfloat.gcc
```

Старые директории/пути `r36sx_shell` были переименованы в `Shell`.
