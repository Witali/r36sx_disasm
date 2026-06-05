# Pico-286 Host RPC Device

Pico-286 is moving host-folder access away from emulator-owned `INT 2Fh`
hooks.  DOS drivers and TSRs should own `INT 2Fh`; the emulator should expose
only a small private hardware-like RPC device.

## Port Range

The R36SX host RPC device uses I/O ports `E360h..E36Fh`.

These ports are intentionally outside the common IBM PC/AT legacy range used by
DMA, PIC, PIT, keyboard, CMOS, VGA, serial, parallel, Sound Blaster, AdLib, and
NE2000-style cards.  They also avoid `80h`/`190h` POST logging and `E9h`
debug-console conventions.

## Port Layout

| Port | Direction | Meaning |
| --- | --- | --- |
| `E360h` | read | Signature byte `R` |
| `E361h` | read | Signature byte `H` |
| `E362h` | read | RPC version, currently `1` |
| `E363h` | read | Status |
| `E364h..E367h` | read/write | 32-bit physical request-block address, little-endian |
| `E368h` | write | Command port; write `1` to execute request block |
| `E369h..E36Ah` | read | Last RPC result, little-endian |

## Request Block

All multi-byte fields are little-endian and live in guest physical RAM.

```c
struct host_rpc_request {
    uint16_t magic;      /* "HR" as 0x5248 */
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
};
```

## Commands

| Command | Name | Notes |
| --- | --- | --- |
| `0` | Ping | Verifies the device and writes version info. |
| `1` | Open read-only | Opens `path_phys`, returns `handle` and `file_size`. |
| `2` | Open read/write | Opens or creates `path_phys`. |
| `3` | Create | Creates/truncates `path_phys`. |
| `4` | Close | Closes `handle`. |
| `5` | Read | Reads `data_len` bytes at `file_pos` into `data_phys`. |
| `6` | Write | Writes `data_len` bytes from `data_phys` at `file_pos`. |
| `7` | Delete | Deletes `path_phys`. |
| `8` | Mkdir | Creates directory `path_phys`. |
| `9` | Rmdir | Removes directory `path_phys`. |
| `10` | Getattr | Returns file size and DOS directory attr bit. |
| `11` | Rename | Renames `path_phys` to `path2_phys`. |
| `12` | Commit | Flushes the open file in `handle`. |
| `13` | Find first | Opens a host search for `path_phys`; writes a find result to `data_phys`. |
| `14` | Find next | Continues the search in `handle`; writes a find result to `data_phys`. |
| `15` | Find close | Closes a search handle from find first. |

Find commands write this fixed 20-byte result into `data_phys`, provided
`data_len >= 20`:

```c
struct host_rpc_find_result {
    uint8_t name[11]; /* DOS 8.3 padded name */
    uint8_t attr;
    uint16_t time;    /* currently deterministic placeholder 0x1000 */
    uint16_t date;    /* currently deterministic placeholder 0x1000 */
    uint32_t size;
};
```

The current implementation resolves all guest paths inside
`host_drive_path`.  `..` components are allowed only inside that mapped root,
so guest paths cannot escape above the configured host directory.

## Diagnostic COM

`homebrew/pico_286/pico-286/tools/hostrpc_test.asm` builds to `hostrpc.com`.
It checks the port signature, sends a Ping request, creates `HOSTRPC.TXT`,
writes one line, and closes it.

This is only a diagnostic program.  The next step is a resident DOS
redirector/driver that owns `INT 2Fh AH=11h` and translates DOS redirector
callbacks into these host RPC requests.

The resident redirector should be built as a DOS driver/TSR that can be loaded
high.  Prefer a `HOSTDRV.SYS` form loaded with `DEVICEHIGH=` when UMBs are
available, so the file bridge does not permanently consume conventional DOS
memory.
