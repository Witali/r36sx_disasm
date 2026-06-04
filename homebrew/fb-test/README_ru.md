# fb-test

`fb-test` - маленькая нативная MIPS-диагностика для штатной Linux-прошивки
R36SX. Программа специально не использует `driver.so`; она повторяет безопасные
и уже наблюдаемые части framebuffer-пути, найденного в дизассемблированном
`driver.so`.

Программа:

- проверяет `/dev/dis` через ioctl `0xc00c0e0c`;
- проверяет `/dev/ge`: открывает устройство, мапит регистры и запрашивает
  метаданные command queue через ioctl `0x20002305`;
- открывает `/dev/fb0`;
- читает стандартные Linux framebuffer параметры через `FBIOGET_FSCREENINFO` и
  `FBIOGET_VSCREENINFO`;
- мапит память `/dev/fb0`;
- рисует RGB test pattern напрямую в framebuffer;
- делает pan/flip через `FBIOPAN_DISPLAY`, если доступна вторая framebuffer
  страница.

GE-команды пока не отправляются. Формат command queue остается vendor-specific,
поэтому probe `/dev/ge` не кормит hardware queue новыми командами.

## Сборка

Из корня репозитория:

```sh
wsl bash -lc "cd /mnt/c/Work/r36sx_disasm && ./homebrew/fb-test/build_fb_test_wsl.sh --strip --install"
```

Пути установки:

```text
disk_image/MIPS_NATIVE/fb-test/fb-test
patches/disk_image_patch_fb_test/MIPS_NATIVE/fb-test/fb-test
```

## Запуск

На устройстве:

```sh
/mnt/sdcard/MIPS_NATIVE/fb-test/fb-test
```

Полезные опции:

```text
--hold SECONDS  держать test pattern на экране перед выходом
--no-ge         не проверять /dev/ge
--no-pan        рисовать только в текущую видимую framebuffer-страницу
--restore       вернуть прежнюю страницу перед выходом, если делался pan
```

Пример:

```sh
/mnt/sdcard/MIPS_NATIVE/fb-test/fb-test --hold 20 --restore
```

## Ожидаемый результат

Если прямые записи в framebuffer работают, LCD должен показать test pattern:
желтую рамку, цветные полосы и градиент. Если картинка неправильная, повернута,
невидима или появляется только после использования `driver.so`, это покажет, что
нам еще нужно повторить из vendor pipeline: GE blit/scale/rotate, настройку
страниц framebuffer или vendor display commit ioctls.

