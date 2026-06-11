# HOSTRPC Protocol v2: 16-bit Stream Upgrade

Этот документ описывает план апгрейда HOSTRPC stream transport с текущего
8-битного режима на 16-битные кадры. Цель v2 - ускорить обмен без возвращения
к отдельным command/address/result портам: все команды, адрес request block и
ответы по-прежнему идут через основной RPC stream port `E360h`.

## Цели

- Сохранить совместимость с текущим 8-bit stream protocol
  (`RPC_VERSION = 3`).
- Использовать тот же порт `E360h`, но разрешить 16-битный `IN/OUT`.
- Передавать больше полезных бит за один I/O frame.
- Сократить передачу 32-битного physical address request block с пяти кадров
  до трех.
- Не вводить дополнительный `CALL` слой и не возвращать отдельные
  `E362h/E364h..E369h` порты.

## Совместимость

8-bit режим остается обязательным базовым режимом. Любой гость должен уметь
работать через текущие 8-битные кадры:

| Режим | Порт | I/O width | Payload bits | Request address frames |
| --- | --- | --- | --- | --- |
| current | `E360h` | 8-bit | 7 | 5 |
| stream16 | `E360h` | 16-bit | 15 | 3 |

Гость выбирает stream16 только после успешного обнаружения capability. Если
host не поддерживает 16-битный режим, гость остается на 8-bit режиме.

## Capability Handshake

Нужно добавить явное обнаружение возможностей, не ломая старый `PING`.
Предлагаемый вариант:

| Команда | ID | Назначение |
| --- | --- | --- |
| `PROTO_CAPS` | `5` | Вернуть capability bitmap текущего HOSTRPC device |

ID `5` можно занять под 8-bit-safe capability query внутри зарезервированного
служебного диапазона `0..9`. Старый host, который не знает `PROTO_CAPS`,
вернет `PROTO_ERR` (`7Fh`) или не подтвердит команду ожидаемым образом; guest
обязан в этом случае использовать 8-bit режим.

Первый capability word можно читать через 8-bit режим, чтобы договориться о
stream16 без предварительного использования 16-битного I/O:

| Bit | Имя | Значение |
| --- | --- | --- |
| `0` | `RPC_CAP_STREAM16` | Host поддерживает 16-битные stream frames на `E360h` |
| `1` | `RPC_CAP_RESP_CONTINUE` | Host поддерживает многофреймовые stream responses |
| `2..14` | reserved | Должны быть нулями |

Так как 8-bit frame несет только 7 бит ответа, capability bitmap можно вернуть
двумя 8-bit frames: первый ответ на `PROTO_CAPS` содержит bits `0..6`, следующий
получается командой `CONTINUE` и содержит bits `7..13`. Если нужен полный
16-битный bitmap, третий `CONTINUE` возвращает bits `14..15`.

Минимальный guest algorithm:

1. Прочитать `E360h` и сохранить 8-bit sync bit.
2. Отправить `RESET`.
3. Отправить `PING`, проверить `RPC_VERSION >= 3`.
4. Отправить `PROTO_CAPS`.
5. Если ответ успешен и выставлен `RPC_CAP_STREAM16`, переключиться на stream16.
6. Иначе продолжить работу через 8-bit режим.

## 16-bit Frame Format

v2 использует те же смысловые кадры, но в 16-битном слове:

| Бит 15 | Биты 14..0 | Значение |
| --- | --- | --- |
| `1` | command id | Command frame |
| `0` | data payload | Data frame с 15 полезными битами |

Константы v2:

| Имя | Значение |
| --- | --- |
| `PROTO16_CMD_FLAG` | `8000h` |
| `PROTO16_DATA_MASK` | `7FFFh` |
| `PROTO16_ERR` | `7FFFh` |

Command frame отправляется как `8000h | command_id`. Data frame отправляется
как `payload & 7FFFh`.

## 16-bit Sync Bit

В v2 sync bit также является старшим битом читаемого latch, но теперь это bit
15. Guest перед началом v2-обмена читает `IN AX, DX` из `E360h` и сохраняет
bit 15. После каждого `OUT DX, AX` guest ожидает изменения bit 15 при чтении
`IN AX, DX`.

Нижние 15 бит ответа содержат result code или payload. Значение `7FFFh`
зарезервировано как protocol error marker.

## Команды

Командное пространство не меняется относительно текущего 8-bit ABI:
`0..9` зарезервированы для служебных команд протокола, а host commands
`CMD_*` начинаются с `10`. Отличается только ширина frame и количество
полезных бит.

В v2 гость по-прежнему вызывает host commands напрямую:

```text
OUT E360h, 8000h | CMD_READ
OUT E360h, request_addr bits 0..14
OUT E360h, request_addr bits 15..29
OUT E360h, request_addr bits 30..31
```

Дополнительной команды `CALL` нет.

## Адрес Request Block

32-битный physical address request block передается младшими битами вперед
тремя 15-битными data frames:

| Кадр | Биты адреса | Payload |
| --- | --- | --- |
| `0` | `0..14` | `addr & 7FFFh` |
| `1` | `15..29` | `(addr >> 15) & 7FFFh` |
| `2` | `30..31` | `(addr >> 30) & 0003h` |

После первых двух address frames host отвечает `0`. После третьего кадра host
выполняет команду, записывает ответ в request block и возвращает result code в
нижних 15 битах. Если команда не может быть выполнена на уровне протокола,
host возвращает `PROTO16_ERR`.

## Request Block ABI

Request block layout можно оставить совместимым с текущим 8-bit ABI:

| Offset | Поле | Размер |
| --- | --- | --- |
| `0` | `magic` | `word` |
| `2` | `version` | `word` |
| `4` | `command` | `word` |
| `6` | `flags` | `word` |
| `8` | `path_phys` | `dword` |
| `12` | `path2_phys` | `dword` |
| `16` | `data_phys` | `dword` |
| `20` | `data_len` | `dword` |
| `24` | `file_pos` | `dword` |
| `28` | `file_size` | `dword` |
| `32` | `handle` | `word` |
| `34` | `mode` | `word` |
| `36` | `attr` | `word` |
| `38` | `dos_error` | `word` |
| `40` | `result` | `word` |
| `42` | `reserved` | `word` |
| `44` | `bytes_done` | `dword` |
| `48` | Конец | |

Поле `version` остается `3`, если меняется только транспортный frame format.
Если позже появятся новые поля request block, тогда нужно увеличить именно
request-block ABI version.

## Host-side Changes

Нужные изменения на стороне эмулятора:

1. Научить CPU I/O layer различать byte и word access к порту `E360h`.
2. Оставить текущий byte handler как 8-bit режим.
3. Добавить word handler для stream16:
   - `r36sx_host_rpc_stream16_portout(uint16_t value)`;
   - `r36sx_host_rpc_stream16_portin(void)`;
   - отдельные `stream16_sync_bit`, `stream16_value`, `stream16_command`;
   - счетчик address chunks до `3`.
4. Использовать общий executor host commands после восстановления
   `request_addr`.
5. Не обслуживать `E362h/E364h..E369h`; весь v2 transport идет через `E360h`.

Важно: если текущий CPU core вызывает `portout(port, value)` без информации о
ширине I/O, перед реализацией v2 нужно протащить width до port layer. Иначе
host не сможет надежно отличить `OUT DX, AL` от `OUT DX, AX`.

## Guest-side Changes

В `hostrpc.inc` нужно добавить v2 constants и helpers:

```asm
PROTO16_CMD_FLAG  equ 08000h
PROTO16_DATA_MASK equ 07FFFh
PROTO16_ERR       equ 07FFFh

RPC_CAP_STREAM16  equ 0001h
```

Новые helper-процедуры:

- `rpc16_init_session`;
- `rpc16_send_command`;
- `rpc16_send_data`;
- `rpc16_send_phys`;
- `rpc16_wait_toggle`.

`HOSTDRV.COM` должен делать auto-detect:

1. Инициализировать v1.
2. Запросить capabilities.
3. Если есть `RPC_CAP_STREAM16`, переключить function pointers или dispatch
   flag на v2 helpers.
4. Если v2 недоступен или сломался во время старта, использовать v1.

## Error Handling

Рекомендуемое поведение:

- Timeout ожидания sync bit в v2 должен возвращать ту же ошибку, что timeout
  8-bit режима.
- `PROTO16_ERR` означает ошибку транспорта; подробность, если request block
  был принят, остается в `result` и `dos_error`.
- `ABORT` должен сбрасывать незавершенную stream16-передачу address frames.
- `RESET` должен сбрасывать и 8-bit, и stream16 state, а также закрывать stale
  host handles, как сейчас.
- Для host commands stream16 также должен валидировать размер адресной посылки:
  ровно три data frames. Любая команда вместо недостающего data frame или
  лишний data frame без активной команды должны возвращать `PROTO16_ERR`.

## Многофреймовые Ответы

v2 делает `CONTINUE` полезнее: один frame несет 15 бит вместо 7. Для будущих
ответов без request block можно использовать тот же порядок little-endian
битовых chunks:

| Chunk | Bits |
| --- | --- |
| `0` | `0..14` |
| `1` | `15..29` |
| `2` | `30..44` |

Текущие файловые команды могут не использовать stream responses, потому что
основные данные и metadata уже возвращаются через request block.

## Тестирование

Минимальный набор проверок:

1. v1-only host: новый `HOSTDRV.COM` должен корректно fallback-нуться на v1.
2. v2 host + новый guest: `HOSTRPC.COM` создает файл и копирует файл на host.
3. v2 host + `HOSTDRV.COM`: `DIR`, `COPY`, `DEL`, `MD`, `RD`, `REN`, `CD`.
4. MIPS WSL release build.
5. Windows build.
6. Негативные проверки:
   - отправить data frame без command frame;
   - оборвать address transfer через `ABORT`;
   - отправить неизвестную команду;
   - проверить timeout guest side.

## Ожидаемый Выигрыш

Для каждой host command текущий v1 transport делает:

- 1 command frame;
- 5 address frames;
- 6 ожиданий sync bit.

v2 делает:

- 1 command frame;
- 3 address frames;
- 4 ожидания sync bit.

То есть каждый request экономит два I/O кадра и два polling loop. Для
мелких файловых операций, где payload уже лежит в guest RAM и основная цена -
частые RPC вызовы, это должно дать самый заметный эффект.
