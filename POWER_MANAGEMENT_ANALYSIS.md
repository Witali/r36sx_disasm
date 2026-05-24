# Анализ энергосбережения в игровой части

## Краткий вывод

В `cubegm` / игровом эмуляторном frontend я не нашел признаков CPU halt, MIPS `wait` или динамического снижения частоты CPU.

Что есть:

- обычные `usleep(...)` / `SDL_Delay`-подобные задержки;
- polling input каждые ~16 ms;
- frame pacing через задержки, frame skip и audio/video timing;
- управление backlight/MIPI/HDMI;
- переключение boot slot / reboot при выходе или переходе между game/projector UI.

Что не найдено:

- `cpufreq`;
- `scaling_governor`;
- `scaling_cur_freq`;
- `scaling_setspeed`;
- `scaling_min_freq`;
- `scaling_max_freq`;
- `/sys/devices/system/cpu/cpu*/cpufreq`;
- `/sys/power/state`;
- MIPS instruction `wait` (`0x42000020`) в основных бинарниках.

## Проверка MIPS wait/halt

Искал little-endian encoding MIPS `wait`: bytes `20 00 00 42`.

Результат по основным бинарникам:

- `disk_image\cubegm\rkgame`: `0`
- `disk_image\cubegm\icube`: `0`
- `disk_image\cubegm\MyExecutable`: `0`
- `disk_image\cubegm\usr\bin\icube`: `0`
- `disk_image\rootfs\usr\bin\cubevol`: `0`
- `disk_image\rootfs\usr\bin\hcprojector`: `0`

По MIPS cores из `disk_image\cubegm\cores` размером до 15 MB также не найдено попаданий.

Вывод: в userspace-бинарниках не видно прямого использования MIPS CPU idle/halt instruction.

## `rkgame`

`disk_image\cubegm\rkgame` — главный frontend/loader эмуляторных cores.

Ghidra показала:

- `run_game()` выбирает core, грузит ROM/ZIP, вызывает `FBA_Load`, `Gpsp_Load`, `PCSX_Load` или `Core_Load`.
- `DrawFrame()` делает rotation/skipframe/displayfps и вызывает `dispFlip(...)`.
- `PlayFrame()` фактически вызывает `PlaySound()`.
- В pause/menu/UI много `usleep(...)`, но это обычные задержки, не управление CPU.
- Есть frame skip:
  - `skipframe_count = 0x14`
  - `skipframe_count = 0x1e`
  - `skipframe_count = 0x3c`
- Есть audio-buffer based поведение через строки/символы вроде `sound_driver_buffering_percent`, `retro_audio_buff_occupancy`.

Не найдено:

- управление CPU governor/frequency;
- запись в cpufreq sysfs;
- прямой CPU halt/wait;
- standby через `/dev/standby` внутри основного игрового цикла.

## `MyExecutable`

`disk_image\cubegm\MyExecutable` — helper для железа: кнопки, HDMI hotplug, MIPI, backlight.

Важная декомпиляция:

```c
void main(void) {
    ...
    do {
        key = key_getvalue();
        if (key == 0x100) {
            if (screen_off) {
                screen_off = false;
                set_mipi_onoff(1);
                set_backlight_value2(0xff);
                gpio_set_output(0x13, 0);
                *cubegame_state &= ~2;
                is_joykey_enabled = 1;
            } else {
                screen_off = true;
                set_backlight_value2(0);
                gpio_set_output(6, 0);
                gpio_set_output(0x13, 1);
                is_joykey_enabled = 0;
                *cubegame_state |= 2;
            }
        }
        usleep(16000);
    } while (true);
}
```

Это похоже на выключение/включение экрана и подсветки, а не на halt CPU.

`set_backlight_value2()`:

```c
open64("/dev/backlight", O_RDWR);
write(fd, &brightness, 4);
fsync(fd);
close(fd);
```

`set_mipi_onoff()`:

- открывает `/dev/backlight`;
- открывает `/dev/mipi`;
- делает несколько `ioctl(...)`;
- при выключении ставит backlight в `0`;
- делает `usleep(500000)` и `usleep(200000)`;
- при включении восстанавливает яркость через `/dev/persistentmem` при необходимости.

Вывод: энергосбережение в этой части — гашение дисплея/MIPI/backlight, не снижение частоты CPU.

## `driver.so`

`disk_image\cubegm\driver.so` динамически грузится из `rkgame`.

Там есть строка `/dev/standby`, но Ghidra показывает, что она используется в `set_bootup_slot()`:

```c
set_bootup_slot(slot) {
    fd = open64("/dev/standby", O_RDWR);
    ioctl(fd, 0x20002709, slot);
    close(fd);
}
```

`exit_game()`:

```c
set_bootup_slot(0x11);
if (exists("/mnt/sdcard/cubegm/avexit")) {
    system("/mnt/sdcard/cubegm/avexit");
}
system("reboot -f");
```

`enter_multios_app()`:

```c
fbdev_deinit();
sound_driver_deinit();
set_multios_app(...);
set_bootup_slot(0x21);
if (exists("/mnt/sdcard/cubegm/avexit")) {
    system("/mnt/sdcard/cubegm/avexit");
}
fbdev_aspect_ratio_wrok(0);
api_osd_show_onoff(0);
system("/mnt/sdcard/cubegm/icubemp_start.sh &");
```

Вывод: `/dev/standby` здесь используется как интерфейс boot slot / multi-OS switching, а не как suspend/halt в игровом цикле.

## Где настоящий standby

Настоящий standby/sleep найден не в `rkgame`, а в системной/projector части:

- `disk_image\rootfs\usr\bin\hcprojector`
  - строки: `MSG_TYPE_STANDBY_SLEEP`, `MSG_TYPE_WAKE_UP`, `api_system_standby`, `api_dis_suspend`, `/dev/standby`, `Enter standby?`, `Auto Sleep`.

- `disk_image\rootfs\usr\bin\cubevol`
  - строки/символы: `standby_sleep.c`, `mp5_enter_standby`, `enter_sleep_mode_countdown`, `power_handle_proc`, `battery_monitor`, `Low battery`.

- `disk_image\rootfs\usr\lib\libhudi.so`
  - функции: `hudi_standby_enter`, `hudi_standby_ddr_pwroff_set`, `hudi_standby_saradc_wakeup_set`, `hudi_standby_gpio_wakeup_set`, `hudi_standby_ir_wakeup_set`.
  - устройство: `/dev/standby`.

В HCRTOS SDK есть пример bootloader standby:

- `internet_sources\hcrtos\components\applications\apps-bootloader\source\cmd\standby.c`

Там логика:

```c
fd = open("/dev/standby", O_RDWR);
ioctl(fd, STANDBY_GET_BOOTUP_MODE, &mode);
usleep(500000);
ioctl(fd, STANDBY_ENTER, 0);
close(fd);
```

Это уже настоящий standby-путь платформы.

## Вероятная модель энергосбережения

1. В обычной игре CPU не останавливается и частота не снижается из userspace.
2. Эмулятор держит скорость через `usleep`, frame skip, audio buffer и `dispFlip`.
3. При паузе/menu CPU тоже не halt-ится, но часть потоков ждут через `usleep`.
4. В helper-коде можно погасить экран/backlight/MIPI — это экономит заметно больше, чем idle frontend, но CPU продолжает работать.
5. Полный standby делает системная часть через `/dev/standby` и Hichip/HUDI API, вероятно с участием драйвера/AVP/RTOS.

## Что можно попробовать для собственного frontend

Если цель — экономия энергии:

- Для idle/menu использовать `usleep(16000)` или больше, если не нужен 60 Hz polling.
- Не делать busy-wait как `OS_Delay()` из `MyExecutable`; это крутит CPU впустую.
- Гасить подсветку через `/dev/backlight`.
- Гасить MIPI/LCD через тот же ioctl-путь, который использует `set_mipi_onoff()`.
- Для полного сна изучать `/dev/standby` ioctl из `libhudi.so`, `hcprojector`, `cubevol` и HCRTOS `standby.c`.
- Не рассчитывать на стандартный Linux cpufreq: в дампе не видно обычного cpufreq sysfs/userspace-интерфейса.

## Проверка goph-R/SF3000-RE

Репозиторий: https://github.com/goph-R/SF3000-RE

Локальная копия: `internet_sources\SF3000-RE`

Состояние на момент проверки: в git-дереве есть только `README.md` и `LICENSE`; исходников C/C++/asm, Makefile, драйверов или launcher-кода нет.

Поиск по `README.md` и дереву репозитория:

- `halt`: не найдено.
- `wait`: не найдено.
- `powersave` / `power saving`: не найдено.
- `cpufreq` / `frequency`: не найдено.
- `standby`: не найдено.
- `suspend`: не найдено.
- `sleep`: не найдено.
- `idle`: не найдено.

Что в репозитории полезно для нашей темы:

- подтверждает MIPS32 little-endian / MIPS32r2;
- подтверждает Buildroot `2021.05-rc2`;
- подтверждает модель `rootfs/` + `cubegm/`;
- подтверждает `cubegm/icube` как рабочую точку запуска своего MIPS32 static binary;
- упоминает DirectFB в `cubegm` и отсутствие SDL/EGL/GLES в stock system.

Вывод: `goph-R/SF3000-RE` полезен как заметка по homebrew/custom launcher, но не даёт исходных файлов, по которым можно подтвердить `halt`, MIPS `wait`, cpufreq или powersaving mode. Для ответа на вопрос об энергосбережении всё ещё важнее локальный бинарный анализ `rkgame`, `driver.so`, `hcprojector`, `cubevol`, `libhudi.so` и HCRTOS standby-кода.
