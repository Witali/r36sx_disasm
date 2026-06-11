# HOSTRPC 16-bit Stream Protocol

Этот документ раньше был планом апгрейда HOSTRPC transport. 16-битный stream
теперь является базовым режимом, а обратная совместимость с 8-битным transport
удалена. Каноническое описание находится в `protocol.md`; здесь оставлена
короткая сводка решения.

## Решение

- Единственный RPC-порт данных: `E360h`.
- Гость использует только word I/O: `OUT DX, AX` и `IN AX, DX`.
- Отдельные command/address/result порты не используются.
- Capability handshake и fallback на 8-битные кадры не нужны.
- Версия stream/request-block ABI: `5`.

## Формат Кадра

| Бит 15 | Биты 14..0 | Значение |
| --- | --- | --- |
| `1` | command id | Command frame |
| `0` | data payload | Data frame с 15 полезными битами |

Константы:

| Имя | Значение |
| --- | --- |
| `PROTO_CMD_FLAG` | `8000h` |
| `PROTO_DATA_MASK` | `7FFFh` |
| `PROTO_ERR_TOO_LARGE` | `7FFDh` |
| `PROTO_ERR_MISMATCH` | `7FFEh` |
| `PROTO_ERR` | `7FFFh` |

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
нижних 15 битах.

## Отличия От Старого 8-битного Transport

| Свойство | Было | Стало |
| --- | --- | --- |
| I/O width | 8-bit | 16-bit |
| Sync bit | bit 7 | bit 15 |
| Payload bits | 7 | 15 |
| Address frames | 5 | 3 |
| RPC version | 4 | 5 |

Для каждой host command старый transport делал один command frame, пять address
frames и шесть ожиданий sync bit. Новый transport делает один command frame,
три address frames и четыре ожидания sync bit.
