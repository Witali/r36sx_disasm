# Похожие проекты и полезные ссылки

Этот файл собирает ссылки, которые могут быть полезны для исследования `disk_image`, Hichip/HC16xx, iCube/cubegm, R36SX/GB350/SF3000 и родственных MIPS handheld-платформ.

## Самые близкие к нашему дампу

### HCRTOS / Hichip SDK

- https://git.maschath.de/ignatz/hcrtos
  - Публичное дерево HCRTOS/Hichip SDK.
  - Локальная копия уже есть в `internet_sources\hcrtos`.
  - Важные директории:
    - `components\applications\apps-projector\source\hcprojector_app`
    - `board\hc16xx\projector`
    - `configs\hichip_hc16xx_*_projector_*`
    - `components\hcfota`
    - `components\liblvgl`
  - Почему важно: содержит исходники projector-приложения и DTS/defconfig для HC16xx projector-платформ. В `projector.c` есть строка `Welcome to Hichip world!`, совпадающая со строками из нашего `hcprojector`.

- https://git.maschath.de/ignatz/hcrtos/-/blob/7356e416d0d07a6f844faf89f433fb106677e9d8/configs/hichip_hc16xx_linux_bl_tiny_defconfig
  - Пример HC16xx defconfig из того же HCRTOS-дерева.

### R36SX / GB350 / SF3000

- https://r36sx-hub.neocities.org/
  - R36SX / GB350 / SF3000 owner hub.
  - Полезен как агрегатор ссылок и текущего состояния сообщества.
  - Важные совпадения с нашим анализом: MIPS SoC, iCube/cubegm firmware, stock OS only, несовместимость с ArkOS/JELOS/EmuELEC.

- https://www.reddit.com/r/R36S/comments/1qdqocz/does_anyone_have_a_backup_of_this_gb350_r36sx/
  - Обсуждение R36SX/GB350 clone.
  - Утверждает, что R36SX использует HiChip HC16xx, MIPS32_R2, Linux 4.4.186, U-Boot и hichip_hc16xx-драйверы.
  - Хорошо совпадает с нашим `dtb.bin` и `rootfs`.

- https://github.com/LiamJ74/R36S-V2.6_Wiki
  - Wiki/tools для R36S-V2.6 / GB350 / R36SX-like карт.
  - По сообщениям сообщества содержит `sync_sd_card` и `download_covers`.
  - Полезно для восстановления списков игр, обложек и структуры SD-карты.

- https://www.reddit.com/r/R36S/comments/1td4nxu/guide_r36sv260712_gb350_r36sx_how_to_add_games/
  - Практический guide по добавлению игр и исправлению SD-карты для R36S-V2.6 / GB350 / R36SX.
  - Полезно для понимания `allfiles.lst`, `filelist.csv`, обложек и пользовательской части карты.

## Родственные Hichip/HCSEMI handheld-платформы

Эти проекты не обязательно совместимы напрямую, но полезны как референсы по подходам: патчинг bootloader/BIOS, работа с ресурсами, списками игр, темами, кастомными frontend и MIPS-бинарниками.

### SF2000

- https://github.com/vonmillhausen/sf2000
  - Большая база знаний по Data Frog SF2000.
  - Полезно как пример реверса дешевого Hichip/HCSEMI handheld.
  - Есть сведения по firmware, bootloader bug, tools, ресурсам и моддингу.

- https://vonmillhausen.github.io/sf2000/
  - Web-версия SF2000 documentation/tools.
  - Полезно для быстрого доступа к инструментам и описаниям.

- https://github.com/Dteyn/Datafrog_SF2000_Vanilla
  - Stripped-down / vanilla SF2000 OS project.
  - Полезно как пример раскладки firmware-only / complete OS files / full image.

- https://github.com/tzlion/frogtool
  - Game list rebuilding tool для SF2000.
  - Может быть полезен как пример форматов списков игр, хотя форматы R36SX/cubegm могут отличаться.

- https://q-ta-s.github.io/sf2000.html
  - Набор SF2000/GB300 tools: themes, boot logo и другие мелкие моды.

- https://grgadam.github.io/SF2000LinkTree/
  - Link tree по SF2000 firmware/tools.

### GB300

- https://github.com/nummacway/gb300
  - Информация по GB300 handheld.
  - Важно: обсуждает родство GB300 и SF2000, firmware versions, multicore, hardware.

- https://github.com/nummacway/gb300-sf2000-tool
  - All-in-one tool для GB300 и SF2000.
  - Может быть полезен как пример GUI-инструмента для тем, списков, bootloader patch и firmware/resource management.

- https://retromods.pl/blog/gb300-setup
  - SF2000/GB300 first steps и multicore setup.

## Кастомные frontend / CFW-инициативы для родственных устройств

- https://github.com/axgdev/UniFrog
  - UniFrog CFW для SF2000/GB300.
  - Прямо не подходит для нашего дампа, но полезен как референс идеи кастомного firmware/frontend на родственных дешевых handheld.

- https://github.com/axgdev/UniFrog/releases
  - Releases UniFrog.

- https://www.reddit.com/r/DataFrogCentral/comments/1t4wvn1/alpha_testers_unifrog_v040_cfw_released/
  - Обсуждение UniFrog v0.4.x.

- https://www.reddit.com/r/SBCGaming/comments/1oyqgwq/frogui_v010_released_a_modern_frontend_for_sf2000/
  - FrogUI: frontend/UI replacement для SF2000/GB300.
  - Полезно как пример направления: заменить stock UI, не обязательно весь kernel/rootfs.

## Эмуляторы / дополнительные cores

- https://murmulator.ru/zxmurmulator
  - Murmulator ZX Spectrum firmware / `pico-spec`.
  - Это богатый ZX Spectrum/Pentagon/Byte/ALF эмулятор для RP2040/RP2350,
    но он bare-metal Pico SDK firmware, а не Linux/libretro `.so`.
  - Подробная локальная оценка портирования: `MURMULATOR_PORTING_ANALYSIS.md`.

- https://github.com/DnCraptor/pico-spec
  - Исходники Murmulator/pico-spec.
  - Локальная копия скачана в `internet_sources\murmulator\pico-spec-main`
    и архив просканирован Defender без угроз.

- https://github.com/EremusOne/ESPectrum
  - Оригинальный ESPectrum, от которого унаследован pico-spec.
  - Может быть полезен для сравнения платформенного слоя и поиска менее
    embedded-зависимых частей.

- https://github.com/angree/sf2000-uae-amiga-emulator/releases
  - Amiga emulator для SF2000/GB300.
  - Полезно как пример добавления нового эмуляторного компонента в похожую экосистему.

- https://github.com/madcock/sf2000_multicore_cores
  - Multicore cores для SF2000.
  - Если репозиторий доступен, смотреть как пример сборки/адаптации cores.

## PC / DOS / x86 эмуляторы

- https://github.com/xrip/pico-286
  - Компактный PC 8086/8088/80186/286 эмулятор для RP2040/RP2350/Murmulator.
  - Важно для нас: есть Linux/Windows host frontends, поэтому ядро эмулятора
    можно пытаться портировать как MIPS native app с нашим framebuffer/input/audio.
  - Локальная копия: `internet_sources\pico-286`, commit
    `2ba88bf17de8e69c86f9256e22a241e8d31e8c96`, Defender scan без угроз.
  - Подробная оценка: `PC_EMULATION_RESEARCH.md`.

- https://github.com/hchunhui/tiny386
  - C99 i386 PC emulator: CPU core плюс PIC/PIT/keyboard/CMOS/VGA/IDE/NE2000/DMA,
    PC speaker, optional AdLib OPL2 и Sound Blaster 16.
  - Интересен как наиболее близкий 386-кандидат, но тяжелее `pico-286` по RAM,
    BIOS/VGABIOS, storage и общей сложности.
  - Локальная копия: `internet_sources\tiny386`, commit
    `5cad7585d7a9e3081e8c5f50e3865fdbf09f5ebb`, Defender scan без угроз.
  - Подробная оценка: `PC_EMULATION_RESEARCH.md`.

- https://github.com/schellingb/dosbox-pure
  - Libretro DOSBox fork with controller-friendly DOS game UX.
  - Может быть полезен как источник идей по gamepad mapper/on-screen keyboard,
    но прямой порт на наш stock/libretro путь выглядит рискованнее, чем native app.

- https://github.com/dosbox-staging/dosbox-staging
  - Современный DOSBox fork/continuation.
  - Хороший upstream для standalone DOSBox, но порт на нашу MIPS/Linux консоль
    будет существенно тяжелее `pico-286`.

- https://github.com/joncampbell123/dosbox-x
  - Очень полный DOSBox fork с большим охватом DOS/Windows 3.x/9x сценариев.
  - Полезен как reference, но слишком крупный и сложный для первого порта.

- https://github.com/copy/v86
  - Browser/WASM x86 PC emulator.
  - Полезен как reference, но не как прямой MIPS native port target.

- https://github.com/86Box/86Box
  - Полноценная эмуляция x86-машин с фокусом на точность.
  - Скорее reference-проект; для нашей консоли слишком большой первый шаг.

## Темы, ресурсы, музыка, картинки

- https://github.com/zerter555/SF2000_BGM
  - Примеры/ресурсы для BGM на SF2000.

- https://zerter555.github.io/sf2000-collection/
  - Коллекции тем/ресурсов SF2000.

- https://q-ta-s.github.io/sf2000_theme.html
  - Theme tools/resources для SF2000/GB300.

## Общие инструменты, полезные для нашего анализа

- https://github.com/NationalSecurityAgency/ghidra
  - Ghidra decompiler/disassembler.
  - В workspace уже есть `ghidra_12.0.4_PUBLIC`.
  - Используем для MIPS ELF: `hcprojector`, `rkgame`, `icube`, `cubevol`, `MyExecutable`.

- https://github.com/u-boot/u-boot
  - U-Boot source.
  - Полезно для формата legacy `uImage`, `mkimage`, load/entry/arch headers.

- https://github.com/dgibson/dtc
  - Device Tree Compiler.
  - Нужен для декомпиляции `dtb.bin` / `Bubbles.scr` в DTS и сравнения с HCRTOS DTS.

- https://github.com/ReFirmLabs/binwalk
  - Binwalk firmware analysis.
  - Полезно для поиска вложенных образов, сжатых payload и файловых систем.

## Что учитывать

- SF2000/GB300 проекты полезны как методические аналоги, но не являются прямой заменой прошивки для R36SX/GB350/SF3000-like `cubegm`.
- ArkOS, JELOS, EmuELEC и большинство firmware для настоящей R36S обычно рассчитаны на ARM/RK3326 или другие платформы и не подходят для этого MIPS/Hichip дампа.
- Самая перспективная линия для нашего устройства:
  1. HCRTOS SDK как база для понимания Hichip HC16xx.
  2. Локальный `cubegm` как точка мягкой кастомизации.
  3. Свой MIPS32 LE userspace binary/frontend вместо `rkgame` или через `icube_start.sh`.
  4. DTB/uImage менять только после полного backup и проверки load/entry/SHA256.
