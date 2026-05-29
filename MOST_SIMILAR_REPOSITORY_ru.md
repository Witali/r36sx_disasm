# Наиболее похожий найденный репозиторий

## Лучшее совпадение: goph-R/SF3000-RE

- URL: https://github.com/goph-R/SF3000-RE
- Локальная копия: `internet_sources\SF3000-RE`
- Тип: reverse-engineering notes / практические заметки по запуску своего кода, не исходники `rkgame`.

Почему это самый близкий репозиторий к нашему `disk_image`:

- Совпадает платформа: MIPS32 little-endian / MIPS32r2.
- Совпадает userspace: Buildroot `2021.05-rc2`.
- Совпадает раскладка SD/rootfs: `rootfs/` + `cubegm/`.
- Совпадает запуск игрового окружения через `/mnt/sdcard/cubegm/icube`.
- Совпадает модель emulator cores: MIPS32 `.so` в `cubegm/cores/`, конфиги `cores/config.xml` и `cores/filelist.xml`.
- Совпадает графический стек по смыслу: stock system без SDL/EGL/GLES, с DirectFB в `cubegm`.
- В репозитории заявлен и описан рабочий способ запуска собственного MIPS32 static binary через замену `cubegm/icube`.

Наши локальные подтверждения:

- `disk_image\rootfs\etc\os-release`: `PRETTY_NAME="Buildroot 2021.05-rc2"`.
- `disk_image\cubegm\rkgame`: 1 178 732 bytes.
- `disk_image\cubegm\icube`: 11 976 bytes.
- `disk_image\cubegm\driver.so`: 98 216 bytes.

Вывод:

`goph-R/SF3000-RE` сейчас выглядит как наиболее похожая публичная RE-база именно для этой ветки консолей SF3000 / GB350 / R36SX-like на MIPS + iCube/cubegm. Это лучший старт для самостоятельного homebrew/custom launcher.

## Второе место: LiamJ74/R36S-V2.6_Wiki

- URL: https://github.com/LiamJ74/R36S-V2.6_Wiki
- Локальная копия: `internet_sources\R36S-V2.6_Wiki`
- Тип: wiki/tools для структуры SD-карты, ROM lists, covers, UI resources.

Почему важно:

- Прямо описывает `iCube/cubegm`.
- Даёт ту же цепочку: `icube_start.sh` убивает `hcprojector`, запускает `cubegm/icube`, затем `icube` стартует `cubegm/rkgame`.
- Перечисляет ключевые файлы: `rkgame`, `icube`, `driver.so`, `UI_Res.cpd`, `resource.cpd`, `setting.xml`, `allfiles.lst`, `cores/config.xml`.
- Полезен для моддинга SD-карты и ресурсов, но не для компиляции собственного firmware.

## Близкий firmware backup/toolchain: game-de-it/testrepo + game-de-it/sf3000

- Stock OS release: https://github.com/game-de-it/testrepo/releases/tag/SF3000_StockOS
- Toolchain repo: https://github.com/game-de-it/sf3000
- Локальные копии:
  - `internet_sources\sf3000_stockos_repo`
  - `internet_sources\sf3000_toolchain`

Почему важно:

- Release `SF3000_StockOS` описан как внутренние файлы SD-карты SF3000 без ROM-файлов.
- `game-de-it/sf3000` сейчас почти пустой в git-дереве, но на GitHub есть release `sf3000_toolchain_v0.1`.

## Не найдено

Публичные исходники `cubegm/rkgame`, `icube` или `driver.so` пока не нашлись. Наиболее вероятная стратегия не "декомпилировать до исходников 1:1", а:

1. использовать `goph-R/SF3000-RE` как практический путь запуска своего MIPS32 кода;
2. использовать HCRTOS/Hichip SDK для понимания платформы HC16xx/rootfs/kernel/userspace;
3. оставить штатные kernel/rootfs/driver.so, а заменить или обойти `icube`/`rkgame` своим frontend;
4. для полной замены firmware сначала собрать минимальный MIPS32 static userspace launcher и проверить вывод через `/dev/fb0` или DirectFB.
