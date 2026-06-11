# Pico-286 Host RPC Device

Pico-286 is moving host-folder access away from emulator-owned `INT 2Fh`
hooks.  DOS drivers and TSRs should own `INT 2Fh`; the emulator should expose
only a small private hardware-like RPC device.

## Stream Port

The R36SX host RPC device uses one bidirectional 8-bit stream port, `E360h`.

This port is intentionally outside the common IBM PC/AT legacy range used by
DMA, PIC, PIT, keyboard, CMOS, VGA, serial, parallel, Sound Blaster, AdLib, and
NE2000-style cards.  It also avoids `80h`/`190h` POST logging and `E9h`
debug-console conventions.

## Stream Protocol

Guest code reads `E360h` before starting a session and stores bit 7 as the
current sync bit.  Command frames are written with bit 7 set; data frames are
written with bit 7 clear and carry 7 payload bits.  Every host reply toggles
bit 7 in the readable latch, so the guest waits until that bit changes before
using the low 7 payload bits.

| Command | Value | Notes |
| --- | --- | --- |
| `RESET` | `0` | Resets HOSTRPC stream state and closes stale host handles. |
| `PING` | `1` | Returns RPC version `4` in the low 7 bits. |
| `CONTINUE` | `2` | Reserved for multi-frame host responses. |
| `ABORT` | `3` | Cancels the current stream transfer. |
| `END` | `4` | Ends the current stream transfer. |

Command ids `0..9` are reserved for protocol service commands.  Filesystem
commands use the same command-frame namespace starting at `10`.  After a
filesystem command frame, the guest sends a 32-bit physical request-block
address as exactly five 7-bit data frames.  The command frame selects the
operation; the request block carries arguments and response fields.  The
`command` field remains in the block as a staging/debug mirror, but the host
uses the command-frame value as the operation selector.

If a new command, `PING`, `CONTINUE`, or `END` arrives before all five address
frames, the host replies with `7Eh` (`PROTO_ERR_MISMATCH`) and drops the
partial request.  `RESET` and `ABORT` remain emergency resynchronization
commands.  An extra data frame immediately after the fixed-size address payload
returns `7Dh` (`PROTO_ERR_TOO_LARGE`).  A data frame without an active
filesystem command and without a preceding fixed-size request returns `7Eh`.

Transport error codes are reserved at the top of the 7-bit response range:

| Value | Name | Notes |
| --- | --- | --- |
| `7Dh` | `PROTO_ERR_TOO_LARGE` | Fixed-size request payload has extra data. |
| `7Eh` | `PROTO_ERR_MISMATCH` | Guest and host stream states disagree. |
| `7Fh` | `PROTO_ERR` | Generic protocol error. |

## Request Block

All multi-byte fields are little-endian and live in guest physical RAM.

```c
struct host_rpc_request {
    uint16_t magic;      /* "HR" as 0x5248 */
    uint16_t version;    /* 4 */
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
    uint16_t reserved;   /* GETATTR/open response: DOS packed time */
    uint32_t bytes_done; /* GETATTR/open low word: DOS packed date; read/write: byte count */
};
```

## Commands

| Command | Name | Notes |
| --- | --- | --- |
| `10` | Open read-only | Opens `path_phys`, returns `handle` and `file_size`. |
| `11` | Open read/write | Opens or creates `path_phys`. |
| `12` | Create | Creates/truncates `path_phys`. |
| `13` | Close | Closes `handle`. |
| `14` | Read | Reads `data_len` bytes at `file_pos` into `data_phys`. |
| `15` | Write | Writes `data_len` bytes from `data_phys` at `file_pos`. |
| `16` | Delete | Deletes `path_phys`. |
| `17` | Mkdir | Creates directory `path_phys`. |
| `18` | Rmdir | Removes directory `path_phys`. |
| `19` | Getattr | Returns file size, DOS attribute bits, and packed DOS date/time. |
| `20` | Rename | Renames `path_phys` to `path2_phys`. |
| `21` | Commit | Flushes the open file in `handle`. |
| `22` | Find first | Opens a host search for `path_phys`; writes a find result to `data_phys`. |
| `23` | Find next | Continues the search in `handle`; writes a find result to `data_phys`. |
| `24` | Find close | Closes a search handle from find first. |
| `25` | Close all | Closes all host handles owned by the redirector instance. |
| `26` | Chdir | Verifies a directory and updates HOSTRPC current-directory state for relative paths. |

Find commands write this fixed 20-byte result into `data_phys`, provided
`data_len >= 20`:

```c
struct host_rpc_find_result {
    uint8_t name[11]; /* DOS 8.3 padded name */
    uint8_t attr;
    uint16_t time;    /* DOS packed local time */
    uint16_t date;    /* DOS packed local date */
    uint32_t size;
};
```

The current implementation resolves all guest paths inside
`host_drive_path`.  `..` components are allowed only while they stay inside
that mapped root; attempts such as `..\..\file` are rejected as bad paths
instead of being clamped to the root or resolved outside the configured host
directory.

## Diagnostic COM

`homebrew/pico_286/pico-286/tools/hostrpc_test.asm` builds to `hostrpc.com`.
Without arguments it checks the stream protocol with RESET/PING, creates
`HOSTRPC.TXT`, writes one line, and closes it.  With one filename argument it
opens that DOS file and copies it to the mapped host drive through HOSTRPC.

Build it with the shared HOSTRPC include directory:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -i.\homebrew\pico_286\pico-286\tools\ -f bin .\homebrew\pico_286\pico-286\tools\hostrpc_test.asm -o .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\tools\HOSTRPC.COM
```

This is only a diagnostic program.  The next step is a resident DOS
redirector/driver that owns `INT 2Fh AH=11h` and translates DOS redirector
callbacks into these host RPC requests.

The resident redirector should be built as a DOS driver/TSR that can be loaded
high.  Prefer a `HOSTDRV.SYS` form loaded with `DEVICEHIGH=` when UMBs are
available, so the file bridge does not permanently consume conventional DOS
memory.
