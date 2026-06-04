# План установки Debian 12 mipsel для R36SX / HC16xx

Дата: 2026-06-03.

Этот план рассчитан на Debian 12 Bookworm `mipsel` для устройства на HC16xx с
сохранением штатной MTD-цепочки загрузки. Цель: сначала запустить Debian rootfs
с ext4-раздела SD-карты, а уже потом пробовать прямую передачу root от ядра,
если штатное ядро подтвердит раннее монтирование этого раздела.

## Текущие факты

- CPU: `MIPS 74Kc V4.12`, MIPS32r2, little-endian, O32.
- FPU есть по сохранённому `/proc/cpuinfo`: `FPU V0.0`.
- ABI штатных бинарников и наших native-сборок: `mipsel`, hard-float,
  dynamic linker `/lib/ld.so.1`.
- Штатное ядро: Linux `4.4.186-release`, vendor BSP Hichip HC16xx.
- Видимая Linux-память из `dtb.bin`: `0x0af91e50`, примерно 175.6 MiB.
- Текущая kernel command line:
  `root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc console=tty1 ...`
- Runtime mounts показывают `/dev/mmcblk0p1` как VFAT на `/mnt/sdcard`,
  `/media/mmc`, `/bin`, `/sbin`, `/etc`, `/lib`, `/usr`.
- MTD-дизассемблирование доказывает только раннюю цепочку NOR -> stage2 ->
  `0x89da0000`. Linux kernel, DTB, AVP firmware и boot logo лежат на SD/FAT и
  перечислены в DTS-блоке `external_files`.
- SD boot-файлы, ожидаемые штатным загрузчиком:
  - `cubegm/dtb.bin`
  - `cubegm/avp.uImage`
  - `cubegm/vmlinux.uImage`
  - `cubegm/xgame-logo.bmp`
- `cubegm/vmlinux.uImage` - U-Boot legacy Linux/MIPS image с load address
  `0x80000000` и entry point `0x803337c0`.

## Выбор версии Debian

Первой целью берём Debian 12 Bookworm `mipsel`.

Причины:

- У Debian 12 ещё есть официальная документация релиза/установки для `mipsel`.
- Debian 13 Trixie уже удалил `mipsel` и `mips64el`.
- ABI устройства хорошо совпадает с Debian `mipsel`, лучше чем soft-float или
  big-endian MIPS.
- Штатное ядро старое, но достаточно новое для минимального Debian 12 userspace.

Запасные варианты:

- Debian 11 Bullseye `mipsel`, если Bookworm окажется слишком тяжёлым.
- Debian 10 Buster `mipsel` только как архивный fallback.
- Buildroot, если компактный embedded-rootfs важнее пакетной базы Debian.

Официальные ссылки:

- <https://www.debian.org/releases/bookworm/mipsel/>
- <https://www.debian.org/releases/bookworm/debian-installer/>
- <https://www.debian.org/releases/trixie/release-notes.en.pdf>

## Принцип безопасности

Для bring-up Debian не трогаем MTD.

Безопасный путь:

```text
stock MTD loader
  -> stock SD/FAT boot files in cubegm/
  -> stock Linux kernel
  -> mount Debian rootfs from SD ext4 partition
```

Так откат остаётся простым: вернуть старые SD-файлы или поставить оригинальную
SD-карту.

## Этап 0: backup и baseline logs

Перед изменением boot-файлов:

1. Сохраняем уже полученные MTD dumps из `hardware_info/mtd-dump/`.
2. Копируем штатные SD/FAT-файлы в backup на хосте:
   - `cubegm/dtb.bin`
   - `cubegm/dtb.dts`
   - `cubegm/avp.uImage`
   - `cubegm/vmlinux.uImage`
   - `cubegm/Bubbles.scr`
   - `cubegm/icube.sh`
   - `cubegm/icube_start.sh`
   - `cubegm/icubemp_start.sh`
3. На устройстве через нашу `shell` снимаем свежие логи:

```sh
cat /proc/cpuinfo > /mnt/sdcard/debian_cpuinfo.txt
cat /proc/cmdline > /mnt/sdcard/debian_cmdline_stock.txt
cat /proc/filesystems > /mnt/sdcard/debian_filesystems.txt
cat /proc/mounts > /mnt/sdcard/debian_mounts_stock.txt
dmesg > /mnt/sdcard/debian_dmesg_stock.txt
ls -l /dev/mmcblk* > /mnt/sdcard/debian_mmc_devices.txt
```

Важная проверка:

```sh
grep -w ext4 /proc/filesystems
```

Если `ext4` отсутствует, прямой ext4-root со штатным ядром не заработает, если
только ext4-модуль нельзя загрузить до передачи root.

## Этап 1: SD-карта с двумя разделами

Целевая разметка:

| Раздел | Тип | Размер | Назначение |
| --- | --- | ---: | --- |
| `p1` | FAT32 | 512 MiB - 2 GiB | Штатные boot/userspace-файлы, `cubegm/`, `MIPS_NATIVE/`, rescue tools. |
| `p2` | ext4 | Остальное место | Debian 12 `mipsel` rootfs. |

Штатный loader ожидает чтение boot-файлов с FAT, поэтому все boot-файлы остаются
на `p1`.

В Ubuntu/WSL, если инструментов нет, нужно попросить установить:

```sh
sudo apt install debootstrap qemu-user-static binfmt-support parted dosfstools e2fsprogs rsync u-boot-tools device-tree-compiler
```

Пример форматирования на хосте, где `/dev/sdX` нужно заменить на реальное
устройство SD:

```sh
lsblk
sudo umount /dev/sdX1 /dev/sdX2 2>/dev/null || true
sudo parted -s /dev/sdX mklabel msdos
sudo parted -s /dev/sdX mkpart primary fat32 1MiB 1025MiB
sudo parted -s /dev/sdX set 1 boot on
sudo parted -s /dev/sdX mkpart primary ext4 1025MiB 100%
sudo mkfs.vfat -F 32 -n R36SXBOOT /dev/sdX1
sudo mkfs.ext4 -L R36SXROOT /dev/sdX2
```

Монтируем и копируем текущую SD-структуру:

```sh
sudo mkdir -p /mnt/r36sx-boot /mnt/r36sx-root
sudo mount /dev/sdX1 /mnt/r36sx-boot
sudo mount /dev/sdX2 /mnt/r36sx-root
sudo rsync -a --info=progress2 /mnt/c/Work/r36sx_disasm/disk_image/ /mnt/r36sx-boot/
sync
```

## Этап 2: Debian 12 mipsel rootfs

Создаём минимальный Debian rootfs на ext4-разделе:

```sh
sudo debootstrap --arch=mipsel --foreign --variant=minbase bookworm /mnt/r36sx-root http://deb.debian.org/debian
sudo cp /usr/bin/qemu-mipsel-static /mnt/r36sx-root/usr/bin/
sudo chroot /mnt/r36sx-root /debootstrap/debootstrap --second-stage
```

Если Bookworm `mipsel` переедет с обычных зеркал, используем archive mirror:

```sh
http://archive.debian.org/debian
```

Настраиваем `/etc/apt/sources.list` внутри rootfs:

```text
deb http://deb.debian.org/debian bookworm main
deb http://deb.debian.org/debian bookworm-updates main
deb http://security.debian.org/debian-security bookworm-security main
```

Для archive fallback:

```text
deb http://archive.debian.org/debian bookworm main
```

Заходим в rootfs и ставим минимальный набор пакетов:

```sh
sudo mount -t proc proc /mnt/r36sx-root/proc
sudo mount -t sysfs sysfs /mnt/r36sx-root/sys
sudo mount --bind /dev /mnt/r36sx-root/dev
sudo chroot /mnt/r36sx-root /bin/sh

apt-get update
apt-get install --no-install-recommends \
  sysvinit-core \
  busybox-static \
  udev \
  ifupdown \
  netbase \
  iproute2 \
  procps \
  kmod \
  mount \
  e2fsprogs \
  ca-certificates \
  dropbear \
  nano
```

Для первых boot-попыток лучше `sysvinit-core`, а не systemd: памяти мало, а
штатная схема ядра/userspace простая.

## Этап 3: настройка Debian rootfs

Внутри `/mnt/r36sx-root` настраиваем базовые файлы.

`/etc/fstab`:

```text
proc            /proc       proc    defaults                    0 0
sysfs           /sys        sysfs   defaults                    0 0
devtmpfs        /dev        devtmpfs mode=0755                  0 0
devpts          /dev/pts    devpts  gid=5,mode=620,ptmxmode=0666 0 0
tmpfs           /run        tmpfs   mode=0755,nosuid,nodev      0 0
/dev/mmcblk0p2  /           ext4    defaults,noatime            0 1
/dev/mmcblk0p1  /mnt/sdcard vfat    defaults,utf8              0 0
```

Создаём mount points:

```sh
mkdir -p /mnt/sdcard /dev/pts /proc /sys /run
```

Настраиваем hostname:

```sh
echo r36sx-debian > /etc/hostname
```

Ставим root password или добавляем пользователя:

```sh
passwd root
adduser r36sx
```

Проверяем `/dev/ptmx`:

```sh
ln -sf /dev/pts/ptmx /dev/ptmx
```

Чистим rootfs:

```sh
apt-get clean
rm -rf /var/lib/apt/lists/*
```

## Этап 4: первый тест на устройстве - ручной mount и chroot

Грузим устройство со штатными SD boot-файлами. Запускаем нашу `shell` и
проверяем:

```sh
ls /dev/mmcblk*
mkdir -p /mnt/debian
mount -t ext4 /dev/mmcblk0p2 /mnt/debian
mount -t proc proc /mnt/debian/proc
mount -t sysfs sysfs /mnt/debian/sys
mount --bind /dev /mnt/debian/dev
mount -t devpts devpts /mnt/debian/dev/pts
chroot /mnt/debian /bin/sh
```

Внутри Debian:

```sh
uname -a
cat /etc/debian_version
dpkg --print-architecture
ldd --version
mount
ls /mnt/sdcard
```

Критерии успеха:

- `mount -t ext4 /dev/mmcblk0p2 /mnt/debian` работает.
- `chroot /mnt/debian /bin/sh` работает.
- Debian показывает architecture `mipsel`.
- Команды читают и пишут файлы на ext4.
- `/proc`, `/sys`, `/dev`, `/dev/pts` пригодны к работе.

Этот milestone доказывает совместимость Debian userspace без изменения kernel
bootargs.

## Этап 5: FAT-side скрипт входа в Debian

Добавляем rescue-friendly скрипт на FAT, например:

```text
/mnt/sdcard/MIPS_NATIVE/debian/enter_debian.sh
```

Форма скрипта:

```sh
#!/bin/sh
set -eu

ROOT=/mnt/debian
mkdir -p "$ROOT"

mountpoint -q "$ROOT" || mount -t ext4 /dev/mmcblk0p2 "$ROOT"
mountpoint -q "$ROOT/proc" || mount -t proc proc "$ROOT/proc"
mountpoint -q "$ROOT/sys" || mount -t sysfs sysfs "$ROOT/sys"
mountpoint -q "$ROOT/dev" || mount --bind /dev "$ROOT/dev"
mkdir -p "$ROOT/dev/pts"
mountpoint -q "$ROOT/dev/pts" || mount -t devpts devpts "$ROOT/dev/pts"

exec chroot "$ROOT" /bin/sh
```

Его можно запускать из нашей native `shell` или из пропатченного frontend script.
Это ещё не полноценная загрузка Debian, потому что Debian init не PID 1.

## Этап 6: опциональный autostart из штатной загрузки

Когда ручной chroot работает, добавляем защищённый autostart в `cubegm/icube.sh`
на FAT-разделе:

```sh
if [ -x /mnt/sdcard/MIPS_NATIVE/debian/enter_debian.sh ]; then
    /mnt/sdcard/MIPS_NATIVE/debian/enter_debian.sh >/mnt/sdcard/debian_boot.log 2>&1 &
fi
```

Штатный frontend fallback нужно оставить в том же скрипте. Неудачный Debian
mount не должен блокировать запуск `icube` или `shell`.

Этот этап полезен для сервисов и экспериментов, но Debian всё ещё работает под
штатным init.

## Этап 7: тест прямого ext4-root

Пробовать только после успеха этапа 4 и подтверждения ext4 в `/proc/filesystems`.

Создаём модифицированный DTS из `disk_image/cubegm/dtb.dts` и меняем `bootargs`:

```text
root=/dev/mmcblk0p2 rootfstype=ext4 rootwait rw init=/sbin/init console=tty1 earlycon= no_console_suspend noirqdebug
```

Для первого прямого теста безопаснее init:

```text
init=/bin/sh
```

Компилируем:

```sh
dtc -I dts -O dtb -o dtb.debian-ext4.bin dtb.debian-ext4.dts
```

Кладём на FAT-раздел с обратимой заменой:

```text
cubegm/dtb.bin.stock
cubegm/dtb.bin
```

Критерии успеха:

- Kernel ждёт `/dev/mmcblk0p2`.
- ext4 root монтируется.
- `/sbin/init` или `/bin/sh` стартует из Debian.

Типичные ошибки:

- Kernel panic: root device unavailable.
- Kernel panic: cannot mount root filesystem.
- Чёрный экран или отсутствие логов из-за ограничений console/input.

Откат:

- Вернуть stock `cubegm/dtb.bin` из backup на FAT-разделе.
- При необходимости загрузиться с оригинальной SD-карты.

## Этап 8: лучший PID1 без direct root

Если direct root не работает, но штатный initramfs можно распаковать и собрать
заново, добавляем ранний путь в `/linuxrc`:

```sh
mount -t devtmpfs devtmpfs /dev
mount -t proc proc /proc
mount -t sysfs sysfs /sys
mount -t ext4 /dev/mmcblk0p2 /newroot
exec switch_root /newroot /sbin/init
```

Это правильнее, чем запускать Debian init из `icube.sh`, потому что
`switch_root` передаёт Debian процесс PID 1. Но для этого нужно точно понимать,
где упакован штатный ramfs, и безопасно пересобирать `vmlinux.uImage`, поэтому
это поздний этап.

## Этап 9: новое ядро - отдельный проект

Замена ядра намного рискованнее Debian rootfs:

- HC16xx display, buttons, USB, MMC, audio и AVP-интеграция зависят от vendor
  BSP.
- У штатного `vmlinux.uImage` известные load/entry значения, и он уже работает
  со штатной boot-chain.
- Текущий MTD-анализ пока не доказывает все ограничения third-stage SD/FAT
  loader.

Перед заменой ядра:

1. Собрать и загрузить маленький `.ko` под
   `4.4.186-release preempt MIPS32_R2 32BIT`.
2. Найти ближайший HCRTOS/HC16xx kernel config.
3. Повторить формат stock uImage: compression, load address, entry.
4. Тестировать только заменой `cubegm/vmlinux.uImage` на backup SD-карте.

## Основные риски

- Debian 12 может быть тяжёлым для 175 MiB RAM, особенно с systemd.
- Direct ext4 root требует MMC и ext4 до root mount.
- Stock boot path может уметь читать только первый FAT-раздел, но это нормально,
  если kernel image и DTB остаются на FAT.
- В stock firmware слабая настройка PTY/devpts; Debian должен явно монтировать
  devpts.
- Console/input на ранней direct boot стадии могут быть неудобны; нужны file
  logs и stock fallback.

## Рекомендуемый первый порядок реализации

1. Собрать SD с двумя разделами.
2. Скопировать stock `disk_image` на FAT `p1`.
3. Создать Debian 12 `mipsel` minbase rootfs на ext4 `p2`.
4. Загрузить stock kernel и вручную сделать mount/chroot в Debian.
5. Добавить `enter_debian.sh` на FAT.
6. Добавить guarded autostart из `icube.sh` только после ручного chroot.
7. Пробовать модифицированный `dtb.bin` с `root=/dev/mmcblk0p2 rootwait` только
   после доказанного ext4 на stock kernel.
8. Декодировать/импортировать third stage `0x89da0000`, если поведение
   bootloader станет блокером.
