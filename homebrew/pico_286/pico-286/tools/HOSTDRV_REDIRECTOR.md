# HOSTDRV network redirector

Этот документ описывает текущую реализацию `HOSTDRV.COM` и связанную с ним
часть эмулятора. `HOSTDRV.COM` живет внутри DOS как TSR network redirector, а
эмулятор предоставляет ему приватный HOSTRPC-порт для доступа к каталогу хоста.

## Основная идея

Раньше в Pico-286 была C-реализация DOS network redirector прямо на стороне
эмулятора. В текущей схеме обработчик DOS-редиректора перенесен в гостевую DOS:

- `hostdrv.asm` устанавливает TSR, помечает выбранный DOS-диск как network
  redirected drive и обслуживает `INT 2Fh/AH=11h`.
- `hostrpc.inc` задает номера портов, команды RPC и формат общего request block.
- `r36sx_host_rpc.c.inl` выполняет файловые операции на хосте.
- `r36sx_ports.c` прокидывает I/O-порты `E360h..E36Bh` в HOSTRPC, trace и
  emergency dump обработчики.

Так DOS сама вызывает TSR через стандартный редиректорный интерфейс, а эмулятор
не внедряется напрямую в `INT 21h`/`INT 2Fh` поток процессора.

## Установка HOSTDRV.COM

`HOSTDRV.COM` принимает букву диска, например:

```dos
HOSTDRV H:
```

Во время установки TSR:

1. Разбирает букву диска. Если аргумент не задан, используется диск `H:`.
2. Проверяет наличие HOSTRPC через `PROTO_RESET` и `PROTO_PING` на порту `E360h`.
3. Получает указатель на DOS SDA через `INT 21h AX=5D06h`.
4. Находит CDS-таблицу и помечает выбранный диск флагами network/physical
   redirector (`CDSFLAG_NET_PHY = C000h`).
5. Перехватывает `INT 2Fh`.
6. Остается резидентным через `INT 21h AH=31h`.

После этого обращения DOS к выбранному диску начинают приходить в
`INT 2Fh/AH=11h` callbacks.

## Как HOSTDRV понимает, что запрос его

У редиректора есть несколько проверок владения.

Для path-based операций (`mkdir`, `delete`, `getattr`, `rename`, `chdir`,
`find first`) путь берется из SDA filename buffer. HOSTDRV проверяет, что первая
строка указывает на назначенную букву диска. Если это другой диск, запрос
передается предыдущему обработчику `INT 2Fh`.

Для SFT-based операций (`read`, `write`, `close`, `commit`) HOSTDRV проверяет
sidecar-таблицу открытых SFT. Таблица хранит `ES:DI` SFT, RPC handle, счетчик
ссылок и маркер, что этот SFT принадлежит HOSTDRV. Есть также fallback-проверка
по device info, но основное владение фиксируется в sidecar.

Для `find next` используется DTA marker. При успешном `find first` HOSTDRV
записывает в DTA свой magic, RPC find handle и букву диска. Следующий
`find next` обслуживается только если marker совпадает.

`process cleanup` (`111Dh`/`1122h`) отправляет `CMD_CLOSE_ALL` только если у
HOSTDRV реально есть открытые файлы или find handles.

## INT 2Fh/AH=11h callbacks

Текущая карта основных callbacks:

| AX | Назначение | Поведение HOSTDRV |
| --- | --- | --- |
| `1100h` | install check | Возвращает `AX=50FFh`. |
| `1101h` | remove directory | `CMD_RMDIR`. |
| `1103h` | make directory | `CMD_MKDIR`. |
| `1105h` | change directory | `CMD_CHDIR`; текущий каталог хранится на стороне эмулятора. |
| `1106h` | close file | `CMD_CLOSE`; учитывается SFT reference count. |
| `1107h` | commit file | `CMD_COMMIT`. |
| `1108h` | read file | `CMD_READ`; буфер берется из текущей DTA, размер из `CX`. |
| `1109h` | write file | `CMD_WRITE`; нулевая запись используется как truncate/extend. |
| `110Ah` | lock/unlock | Сейчас возвращает успех без реального lock manager. |
| `110Ch` | disk info | Возвращает фиктивную геометрию/свободное место. |
| `110Eh` | set attribute | Сейчас возвращает успех без изменения атрибутов. |
| `110Fh` | get attribute | `CMD_GETATTR`; возвращает attr, size, DOS time/date. |
| `1111h` | rename | `CMD_RENAME`. |
| `1113h` | delete | `CMD_DELETE`. |
| `1116h` | open | `CMD_OPEN_RO` или `CMD_OPEN_RW` по access mode со стека DOS. |
| `1117h` | create | `CMD_CREATE`; учитывает create attrs. |
| `111Bh` | find first | `CMD_FIND_FIRST`; записывает FreeDOS-compatible result в DTA. |
| `111Ch` | find next | `CMD_FIND_NEXT`; использует find handle из DTA marker. |
| `111Dh`/`1122h` | process cleanup | Закрывает ресурсы HOSTDRV на стороне эмулятора. |
| `1120h` | flush all disk buffers | Возвращает успех. |
| `1121h` | seek from EOF | Обновляет позицию в SFT локально. |
| `1123h` | qualify filename | Копирует путь `DS:SI` в `ES:DI` для своего диска. |
| `112Eh` | extended open/create | Использует action/mode/attr из SDA и возвращает action в `CX`. |

Если callback не реализован или не принадлежит HOSTDRV, управление передается
предыдущему `INT 2Fh` handler.

## Request block и HOSTRPC stream

HOSTDRV и эмулятор обмениваются request block в памяти гостя. Формат описан в
`hostrpc.inc` и `HOST_RPC.md`:

```c
uint16_t magic;      /* "HR" */
uint16_t version;    /* 1 */
uint16_t command;
uint16_t flags;
uint32_t path_phys;
uint32_t path2_phys;
uint32_t data_phys;
uint32_t data_len;
uint32_t file_pos;
uint32_t file_size;
uint16_t handle;
uint16_t mode;
uint16_t attr;
uint16_t dos_error;
uint16_t result;
uint16_t reserved;
uint32_t bytes_done;
```

Потоковый протокол использует один 8-битный порт `E360h`:

- command frame имеет bit 7 = 1;
- data frame имеет bit 7 = 0 и несет 7 бит полезных данных;
- после команды HOSTDRV отправляет физический адрес request block пятью
  7-битными data frames;
- эмулятор выполняет команду, записывает результат обратно в request block и
  переключает sync bit в readable latch.

Команды файловой системы начинаются с `6`, то есть являются обычными командами
того же RPC-протокола, а не отдельным вложенным namespace:

| Команда | Значение |
| --- | --- |
| `CMD_PING` | `6` |
| `CMD_OPEN_RO` | `7` |
| `CMD_OPEN_RW` | `8` |
| `CMD_CREATE` | `9` |
| `CMD_CLOSE` | `10` |
| `CMD_READ` | `11` |
| `CMD_WRITE` | `12` |
| `CMD_DELETE` | `13` |
| `CMD_MKDIR` | `14` |
| `CMD_RMDIR` | `15` |
| `CMD_GETATTR` | `16` |
| `CMD_RENAME` | `17` |
| `CMD_COMMIT` | `18` |
| `CMD_FIND_FIRST` | `19` |
| `CMD_FIND_NEXT` | `20` |
| `CMD_FIND_CLOSE` | `21` |
| `CMD_CLOSE_ALL` | `22` |
| `CMD_CHDIR` | `23` |

## Обработка путей на стороне эмулятора

Эмулятор читает DOS-строку по физическому адресу из `path_phys` или
`path2_phys`. Путь нормализуется относительно `host_drive_path` и текущего
каталога `r36sx_host_rpc_cwd`.

Правила безопасности:

- `.` игнорируется;
- `..` разрешен только пока итоговый путь остается внутри `host_drive_path`;
- разделители DOS конвертируются в разделители хостовой ОС;
- все операции должны оставаться внутри назначенного корня хостового диска.

Если путь не удается разрешить или он выходит за корень, эмулятор возвращает DOS
ошибку вместо выполнения операции.

## Файлы, позиции и SFT

При `open/create` эмулятор выделяет slot в `r36sx_host_rpc_files[]` и возвращает
RPC handle. HOSTDRV записывает этот handle в SFT и в sidecar-таблицу.

Для `read/write` HOSTDRV передает:

- RPC handle из SFT sidecar;
- физический адрес текущего DTA как буфер;
- `CX` как размер операции;
- текущую позицию файла из SFT.

Эмулятор делает `seek` на `file_pos`, выполняет `read` или `write`, возвращает
`bytes_done`, а HOSTDRV обновляет позицию файла и возвращает DOS число байт в
`CX`.

При `close` HOSTDRV уменьшает локальный reference count. Реальный `CMD_CLOSE`
уходит в эмулятор, когда счетчик достигает нуля.

## FindFirst/FindNext

Результат поиска на стороне эмулятора имеет 20 байт:

```c
char     name[11];  /* DOS 8.3, padded spaces */
uint8_t  attr;
uint16_t time;
uint16_t date;
uint32_t size;
```

HOSTDRV записывает этот блок в DTA в формате, который ожидает FreeDOS:

- service-private область получает magic, drive letter и find handle;
- `SearchDir`/FAT dirent пишется по смещению `DTA+21`;
- на успешном `find first/find next` `CX` обязательно очищается в `0`.

Последний пункт важен для FreeDOS 1.4: `network_redirector_mx` возвращает `CX`
в C-код `DosFindFirst/DosFindNext`. Если `CX` не равен нулю, FreeDOS может
считать `INT 21h` успешным, но не скопировать временный DTA в caller `ffblk`.
Из-за этого команды вроде `DEL` получают старое имя файла.

## Windows/Linux side ports

`r36sx_ports.c` обрабатывает связанные порты:

| Порт | Назначение |
| --- | --- |
| `E360h` | Основной HOSTRPC stream. |
| `E361h..E369h` | Зарезервированный HOSTRPC диапазон. |
| `E36Ah` | HOSTDRV trace port. |
| `E36Bh` | Emergency dump port. |

Trace port используется для коротких событий TSR: вход в callback, chain,
success/fail, проверка SFT ownership и строки `1123h`. Лог включается debug
флагами сборки/конфигурации.

Emergency dump port нужен для аварийной остановки. При записи в него эмулятор
создает каталог `emergency_dump_NNN` с регистрами, RAM, видеопамятью и
текстовым снимком `B800h`, после чего основной цикл останавливает выполнение.

## Ограничения

- Поддерживается один mapped drive за установку TSR.
- Свободное место и геометрия диска пока фиктивные.
- Lock/unlock и set attribute реализованы как успешные no-op.
- Нет LFN; поиск возвращает DOS 8.3 имена.
- Resource tracking резидентный и глобальный для HOSTDRV, без полноценного
  per-process ownership model.
- Это `.COM` TSR, а не `.SYS` драйвер, поэтому он занимает обычную DOS память.

## Практический тест

Для проверки редиректора используется BAT-сценарий, который загружается через
`FDAUTO.BAT` после `HOSTDRV.COM`:

```dos
ECHO hello>C:\NETSRC.TXT
MKDIR H:\R36TEST
COPY C:\NETSRC.TXT H:\R36TEST\NETSRC.TXT
COPY H:\R36TEST\NETSRC.TXT C:\NETBACK.TXT
DEL H:\R36TEST\NETSRC.TXT
RMDIR H:\R36TEST
```

Ожидаемый результат:

- файл копируется на хост и обратно;
- `DEL` и `RMDIR` возвращают успех;
- каталог `host\R36TEST` исчезает после теста;
- в trace-логе видны `CMD_DELETE` и `CMD_RMDIR` с `result=0`.
