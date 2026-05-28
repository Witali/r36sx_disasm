# Важные факты по disk_image

## Общая идентификация

- `disk_image` похож не на один монолитный firmware image, а на содержимое SD/FAT-раздела: ROM-коллекции, медиа, ресурсы интерфейса, плюс прошивочные артефакты в `cubegm` и извлеченный/сопутствующий `rootfs`.
- Устройство относится к платформе Hichip HC16xx / HC1600A.
- Архитектура основного Linux/userspace: ELF32 little-endian MIPS, O32 ABI.
- CPU из DTB: `MIPS 74Kc`.
- Плата/вариант из DTB: `hc1600a@dbE3100v20`.
- `rootfs` основан на Buildroot 2021.05-rc2.

## Важные маскировки

- `disk_image\cubegm\advapi32.dll` не Windows DLL, а U-Boot legacy `uImage` с именем `vmlinux`.
- `disk_image\cubegm\ApplicationFrame.dll` не Windows DLL, а U-Boot legacy `uImage` с именем `avp`.
- `disk_image\cubegm\Bubbles.scr` не скринсейвер, а DTB/FDT device tree.
- Это похоже на намеренную маскировку загрузочных артефактов в FAT-разделе под безобидные Windows-имена.

## Загрузочные артефакты

- `disk_image\cubegm\advapi32.dll`
  - Тип: U-Boot legacy uImage
  - Name: `vmlinux`
  - Data size: `3905915`
  - Load: `0x80000000`
  - Entry: `0x803337C0`
  - Arch code: `5` = MIPS
  - SHA256: `8DB1F56E59A05C58BCCE15EE3B2927A4E9118ED02FE3ED8094AB1890D773DD61`

- `disk_image\cubegm\vmlinux.uImage`
  - Тип: U-Boot legacy uImage
  - Name: `vmlinux`
  - Data size: `3905906`
  - Load: `0x80000000`
  - Entry: `0x803337C0`
  - Arch code: `5` = MIPS
  - SHA256: `53B3E0B3D57FCDBEF40D448AE2D3A00159BC84F2FD5BE4C10A826827C7F2E01E`

- `disk_image\cubegm\ApplicationFrame.dll`
  - Тип: U-Boot legacy uImage
  - Name: `avp`
  - Data size: `1381415`
  - Load: `0x83DA4000`
  - Entry: `0x83DA4000`
  - Arch code: `5` = MIPS
  - SHA256: `7FE578665A9AA9707FDE83CAE04A798CA332C7164E56C531C143E3136D88CF5C`

- `disk_image\cubegm\avp.uImage`
  - Тип: U-Boot legacy uImage
  - Name: `avp`
  - Data size: `1381540`
  - Load: `0x8BDA4000`
  - Entry: `0x8BDA4000`
  - Arch code: `5` = MIPS
  - SHA256: `A9788995DB80197DB6D743B8B237EADBFD3C424FCF1AB448FCD99C003B0950B2`

- `disk_image\cubegm\dtb.bin`
  - Тип: DTB/FDT
  - SHA256: `1258F1EBA809E43540C581B815C87815540A9E5897A4E8584363AB7DE5CC27BB`

- `disk_image\cubegm\Bubbles.scr`
  - Тип: DTB/FDT
  - SHA256: `220BE381988651E73F0538D41B446762AA5ABD7FD72A808F2F26036970A992CB`

## DTB/device tree факты

В `dtb.bin` и `Bubbles.scr` найдены строки:

- `Hichip hc16xx`
- `Hichip,1600`
- `MIPS 74Kc`
- `hichip,hc16xx`
- `soc-16xx`
- `hc1600a@dbE3100v20`
- `root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc`
- `external_files`
- `cubegm`
- `dtb.bin`
- `avp.uImage`
- `vmlinux.uImage`

Это подтверждает, что `cubegm` содержит не только игровой frontend, но и внешние загрузочные файлы для платформы.

## Основные исполняемые файлы

- `disk_image\rootfs\usr\bin\hcprojector`
  - MIPS ELF32 LE EXEC
  - Размер: `14737552`
  - Основное projector-приложение.
  - SHA256: `95B89D1503CBD281A1C3F7C1C14D87763F046CD29D217DCEE0BC7FBD51D70801`

- `disk_image\rootfs\usr\bin\cubevol`
  - MIPS ELF32 LE EXEC
  - Размер: `268544`
  - Системный контроллер питания, standby, backlight, HDMI/MIPI, input.
  - SHA256: `930288B2DBB8D3B5E723701AC365D7180DD8E747279693BAC0D3240A4224B5A0`

- `disk_image\cubegm\rkgame`
  - MIPS ELF32 LE EXEC
  - Размер: `1178732`
  - Главный игровой frontend / loader эмуляторных ядер.
  - SHA256: `57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9`

- `disk_image\cubegm\icube`
  - MIPS ELF32 LE EXEC
  - Размер: `11976`
  - Маленький launcher, запускает `rkgame`.
  - SHA256: `21EADC68EDC6298137E941D4C50DADB4314FAC0FC68F9856649284308B000522`

- `disk_image\cubegm\usr\bin\icube`
  - MIPS ELF32 LE EXEC
  - Размер: `63616`
  - Альтернативный/старый `icube`, по строкам больше похож на GBA/gpsp-oriented app.
  - SHA256: `2146C4C61C6D5AE1A7C3B5F53E05445A8D3913952C9130EC113BF6D5F57828F7`

- `disk_image\cubegm\MyExecutable`
  - MIPS ELF32 LE EXEC
  - Размер: `24672`
  - Helper для железа: MIPI, HDMI hotplug, backlight, joystick/status.
  - SHA256: `B1F8C1D3EBA5882D6528F503E6EC73C2382783BBB1CD82A106C17A4BAF5F9418`

## ELF-статистика по firmware-части

По `disk_image\cubegm` и `disk_image\rootfs`:

- `430` ELF32 LE MIPS shared object / DYN
- `342` ELF32 LE MIPS executable / EXEC
- `24` ELF32 LE MIPS relocatable / REL
- `5` ELF32 LE ARM shared object / DYN

ARM `.so` выбиваются из общей архитектуры и, скорее всего, являются чужими/ошибочно положенными/неиспользуемыми файлами:

- `disk_image\cubegm\cores\libemu_fbalpha.so`
- `disk_image\cubegm\lib\libcrypto.so`
- `disk_image\cubegm\lib\libcrypto.so.1.0.0`
- `disk_image\cubegm\lib\libfreetype.so.6`
- `disk_image\cubegm\lib\libz.so.1`

Основная целевая архитектура для своих бинарников все равно MIPS32 little-endian, не ARM.

## Старт и переключение frontend/app

`disk_image\rootfs\etc\inittab` через BusyBox init запускает:

- `/etc/init.d/rcS`
- `/usr/bin/cubevol > /dev/console 2>&1` в respawn

В `cubegm` есть скрипты переключения:

- `disk_image\cubegm\icube.sh`
  - Добавляет `cubegm\lib` и `cubegm\usr\lib` в `LD_LIBRARY_PATH`.
  - Запускает `/mnt/sdcard/cubegm/icube`.
  - Делает `init -q`.

- `disk_image\cubegm\icube_start.sh`
  - Убивает `hcprojector`.
  - Запускает `/mnt/sdcard/cubegm/icube`.

- `disk_image\cubegm\icubemp_start.sh`
  - Убивает `rkgame` и `icube`.
  - Запускает `/usr/bin/hcprojector`.

Это удобная точка для мягкой кастомизации без перепаковки ядра.

## Эмуляторы и игровой frontend

- `disk_image\cubegm\rkgame` использует `dlopen/dlsym` и грузит `.so` cores из `disk_image\cubegm\cores`.
- `disk_image\cubegm\cores\config.xml` сопоставляет расширения ROM-ов и эмуляторные ядра.
- В строках `rkgame` есть признаки libretro-like API: `retro_*`, save states, screenshots, настройки, PCSX/PSX, FBA/MAME/SNES/GBA/MD/FC и т.д.
- `disk_image\cubegm\setting.xml` содержит пользовательские настройки, hotkeys и пути ресурсов.

### Возможная замена frontend / файловый менеджер

`disk_image\cubegm\icube` содержит прямую строку
`/mnt/sdcard/cubegm/rkgame` и `killall rkgame`, поэтому мягкая замена
frontend возможна через подмену `cubegm\rkgame`: оригинальный бинарник можно
переименовать, а на его место положить свой MIPS ELF launcher/file manager,
который при необходимости запускает оригинальный `rkgame`.

При такой схеме нужно учесть:

- `icubemp_start.sh` сейчас убивает только `rkgame` и `icube`; если оригинал
  будет переименован, скрипт возврата надо расширить на новое имя процесса.
- Перед запуском другой программы файловый менеджер должен корректно освободить
  framebuffer/audio/input или запускать дочерний процесс так, чтобы не было
  конфликта за display/sound.
- Настоящий Midnight Commander (`mc`) является полноэкранным text-mode
  приложением для терминала на базе ncurses/S-Lang. Его можно попытаться
  кросс-собрать, но для показа на экране консоли потребуется рабочая
  терминальная среда/fbcon/tty и перевод кнопок устройства в клавиатурные
  события. Практичнее сначала сделать небольшой MC-like двухпанельный
  framebuffer-файловый менеджер с управлением теми же кнопками, что и наши
  homebrew-модули.

### Кнопка Fn и hotkeys

Fn явно фигурирует в пользовательских настройках:

- `disk_image\cubegm\setting.xml`
  - `<gamemenuhotkey>SELECT+START</gamemenuhotkey>`
  - `<quicksavehotkey>FN+A</quicksavehotkey>`
  - `<quickloadhotkey>FN+B</quickloadhotkey>`
  - `<quicksnaphotkey>FN+START</quicksnaphotkey>`
  - `<savestatehotkey>DISABLE</savestatehotkey>`

Fn также есть в device tree:

- `disk_image\cubegm\dtb.bin` и маскированный дубликат `disk_image\cubegm\Bubbles.scr`
  - `/panel/key-fn = 0x00000002`
  - рядом описаны `/panel/key-do = 0x00000009`, `key-clk = 0x00000008`,
    `key-tl1 = 0x0000000b`, `key-tr1 = 0x00000007`,
    `key-shoulder-left = 0x0000000b`, `key-shoulder-right = 0x00000007`,
    `key-volume-down = 0x0000000c`, `key-volume-up = 0x0000000d`.

Строки `rkgame` содержат имена `quicksavehotkey`, `quickloadhotkey`,
`quicksnaphotkey`, `savestatehotkey`, `GameMenuHotKey`, `KeyMapping`,
`KeyName`, `SetKeyMap` и `gpio_get_input`. Это похоже на обработку Fn как
frontend/system-модификатора, а не как обычной libretro-кнопки. `driver.so` и
`MyExecutable` также содержат `/tmp/joy_key`/`gpio_get_input`; `MyExecutable`
читает несколько `/proc/device-tree/panel/key-*` свойств, но строка
`/proc/device-tree/panel/key-fn` в нем не нашлась.

Практический вывод: Fn аппаратно описана и используется настройками launcher-а
для quicksave/quickload/screenshot. Для homebrew cores ее надо проверять
отдельным raw-scanner модулем: возможно, launcher перехватывает Fn-комбинации
до вызова `retro_input_state()`.

## `rkgame` display path: что пропустил первый Tiny MC

После первого теста Tiny MC на устройстве был белый экран на несколько секунд,
потом черный экран с видимым значком батареи. Декомпиляция `rkgame` и
`driver.so` показала, что stock launcher не рисует в `/dev/fb0` напрямую как
обычное Linux framebuffer-приложение.

Важная последовательность `rkgame`:

1. `main` вызывает `InitDisplay()` до `InitSound()` и `InitJoystick()`.
2. `InitDisplay()` делает `dlopen("%s/driver.so", work_path, RTLD_NOW)`.
3. Затем `dlsym()` ищет `video_driver_setting`, `video_drivers_init`,
   `video_driver_disp_frame`, `video_driver_setmode`,
   `video_driver_get_size`, `api_cube_malloc`, `api_cube_free`,
   `check_exit_game`, `api_cube_send_msg` и `cube_ioctl`.
4. `video_driver_setting()` вызывается с блоком из пяти int, фактически
   `{1, 1, 1, 0x356, 0x1e0}`.
5. После этого запускается `video_drivers_init()`, а кадры выводятся через
   `video_driver_disp_frame()`.

Что делает `driver.so`:

- `video_drivers_init()` создает frame-структуру и вызывает `fbdev_init()`.
- `fbdev_init()` открывает `/dev/dis` и `/dev/fb0`, делает blank/unblank ioctl
  (`0x4611`), читает/меняет framebuffer geometry, открывает HCGE, настраивает
  rotate/aspect/fullscreen state и запускает render/painter threads.
- `video_driver_disp_frame(ptr, w, h, pitch)` при смене размера может заново
  инициализировать framebuffer, затем вызывает `fbdev_draw_frame()`.
- `ReadJoystickProc()` в `rkgame` тоже завязан на `driver.so`: если shared key
  memory не подключена, он вызывает `cube_ioctl(0x40050209)`, затем всегда
  `cube_ioctl(0x40050208, &Cube_State_Reg)`.

Практический вывод: standalone-замена `rkgame` должна либо пользоваться
`driver.so`, либо почти полностью повторять его `/dev/dis` + `/dev/fb0` +
HCGE/rotation/blanking инициализацию. Первый Tiny MC пропустил этот слой, что
хорошо объясняет белый/черный экран при живом overlay батареи.
`disk_image_patch_022` пересобирает Tiny MC так, чтобы кадры 640x480 отдавались
через `video_driver_disp_frame()`. Следующий риск после исправления display
path - input, потому что stock `rkgame` читает кнопки не только через Linux
evdev/js, но и через `cube_ioctl`.

## `rkgame` button masks and `/tmp/joy_key`

После теста Tiny MC с работающей картинкой, но без реакции на кнопки, стало
понятно, что встроенный контроллер надо читать как `rkgame`, а не через
обычный `/dev/input/js*` первым делом.

`ReadJoystickProc()` в `rkgame` делает следующее:

1. Если `KEY_VALUE_Addr == NULL`, вызывает `cube_ioctl(0x40050209,
   &KEY_VALUE_Addr)`. В `driver.so` эта команда делает
   `cube_shmget_init("/tmp/joy_key", 8)` и возвращает адрес shared memory.
2. Читает `KEY_VALUE_Addr[0]` в `joy_key`.
3. Читает `KEY_VALUE_Addr[1]` во второй joy word.
4. Вызывает `cube_ioctl(0x40050208, &Cube_State_Reg)`.

`joykey_explaned()` показывает такие маски:

```text
SELECT 0x00000001
START  0x00000008
UP     0x00000010
RIGHT  0x00000020
DOWN   0x00000040
LEFT   0x00000080
X      0x00001000
A      0x00002000
B      0x00004000
Y      0x00008000
FN     0x00010000
```

`disk_image_patch_024` меняет Tiny MC: теперь он `dlsym()`ит `cube_ioctl`,
получает `/tmp/joy_key` через `0x40050209`, читает raw masks каждый кадр,
логирует `raw0/raw1/state/buttons` в `tiny_mc.log`, и только затем дополняет
результат fallback-источниками `/dev/input/js*` и evdev.

## `icube` перезапускает `rkgame`, если нет heartbeat

Ghidra-анализ `cubegm/icube` показал, что это userspace-супервизор штатного
лаунчера, а не просто вспомогательная утилита:

1. `icube` создает/подключает SysV shared memory:
   `shmget(0x4d2, 0x1c4, IPC_CREAT | 0666)`.
2. Запускает `/mnt/sdcard/cubegm/rkgame` через `fork()` + `execl()`.
3. После первого `sleep(6)` раз в секунду проверяет `shm[1]`.
4. Если `shm[1]` не меняется или равен нулю, вызывает
   `system("killall rkgame")` и запускает `rkgame` снова.

У штатного `rkgame` это обслуживает `XintiaoThread`: он каждые 20 мс вызывает
`xintiao()`, где выставляется `shm[0] = 1` и увеличивается `shm[1]`.

Следствие: любой replacement для `cubegm/rkgame` должен повторять этот
heartbeat, иначе `icube` будет перезапускать его примерно через 5-6 секунд.
Tiny MC делает это начиная с `disk_image_patch_026`.

## Найденные публичные исходники/SDK

В `internet_sources\hcrtos` лежит найденный публичный SDK/исходники HCRTOS.

Особенно важные части:

- `internet_sources\hcrtos\components\applications\apps-projector\source\hcprojector_app`
  - Исходники projector-приложения.
  - В `projector.c` есть строка `Welcome to Hichip world!`, совпадающая со строками из `hcprojector`.

- `internet_sources\hcrtos\configs`
  - Есть множество defconfig для HC16xx projector.
  - Особенно близкие по названию: `hichip_hc16xx_*_d3100_*_projector_*`.

- `internet_sources\hcrtos\board\hc16xx\projector`
  - DTS/board files для HC16xx projector variants.

- `internet_sources\hcrtos\components\hcfota`
  - OTA/update components.

- `internet_sources\hcrtos\components\liblvgl`
  - LVGL-related source.

Вывод: для собственной прошивки разумнее стартовать не с нуля, а от этого HCRTOS SDK/дерева.

## Практические варианты модификации

### Самый безопасный вариант

Оставить vendor kernel, DTB, AVP и rootfs как есть, а заменить/добавить userspace-приложение:

1. Собрать свой ELF под MIPS32 little-endian O32.
2. Линковаться с существующими библиотеками из `cubegm\lib`, `cubegm\usr\lib`, `rootfs\lib`, `rootfs\usr\lib`.
3. Запускать свой бинарник через `icube_start.sh`, заменить `rkgame`, либо заменить `icube` launcher.
4. Для UI/вывода использовать доступные framebuffer/SDL/DirectFB/LVGL/Hichip-библиотеки.

### Средний риск

Собрать свою версию `hcprojector` или игрового frontend на базе найденного HCRTOS SDK, сохраняя vendor kernel/AVP/DTB.

### Высокий риск

Менять загрузочные артефакты:

- `vmlinux.uImage` / `advapi32.dll`
- `avp.uImage` / `ApplicationFrame.dll`
- `dtb.bin` / `Bubbles.scr`

Для этого нужны корректные U-Boot legacy headers, правильные load/entry адреса и совместимый DTS. Перед любыми заменами обязательно сохранить оригиналы и их SHA256.

### Полностью своя прошивка

Теоретически возможна, но практически лучше идти через HCRTOS SDK. Mainline Linux будет сложным путем, потому что железо завязано на vendor-драйверы и Hichip AVP:

- `/dev/ge`
- `/dev/dis`
- `/dev/hdmi`
- `/dev/auddec`
- `/dev/backlight`
- AVP proxy devices

## Рекомендуемая следующая работа

1. Декомпилировать/разобрать `rkgame`, `icube`, `MyExecutable` в Ghidra.
2. Сравнить текущий `dtb.bin` с DTS из `internet_sources\hcrtos\board\hc16xx\projector`.
3. Поднять toolchain для MIPS32 little-endian O32.
4. Собрать минимальный тестовый ELF, который пишет в stdout/log и запускается через `icube_start.sh`.
5. Потом уже делать свой frontend или замену `rkgame`.
