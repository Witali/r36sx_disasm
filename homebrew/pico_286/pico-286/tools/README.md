# Pico-286 DOS Tools

This directory contains small DOS-side helpers used by the R36SX Pico-286 port.

- `hostdrv.asm` is the experimental resident DOS network redirector. It owns
  `INT 2Fh/AH=11h` and translates DOS redirector callbacks into the private
  HOSTRPC I/O-port protocol exposed by the emulator.
- `hostrpc_test.asm` is a non-resident HOSTRPC port probe.
- `mapdrive.asm` is the older helper for marking a DOS drive letter as a
  redirected host drive.

## HOSTDRV RBIL References

The DOS network redirector interface is undocumented by the normal MS-DOS API
manuals, so these Ralf Brown Interrupt List pages are the main per-callback
reference while implementing `hostdrv.asm`.

| RBIL page | HOSTDRV relevance |
| --- | --- |
| [`INT 2Fh AX=1100h` installation check](https://fd.lod.bz/rbil/interrup/network/2f1100.html) | Detect and announce the resident redirector. |
| [`INT 2Fh AX=1106h` close remote file](https://fd.lod.bz/rbil/interrup/network/2f1106.html) | Close the host-side handle stored in the DOS SFT. |
| [`INT 2Fh AX=1108h` read remote file](https://fd.lod.bz/rbil/interrup/network/2f1108.html) | Read through `ES:DI -> SFT`, `CX = byte count`, and the current SDA/DTA buffer. |
| [`INT 2Fh AX=1109h` write remote file](https://fd.lod.bz/rbil/interrup/network/2f1109.html) | Write through `ES:DI -> SFT`, `CX = byte count`, and the current SDA/DTA buffer. |
| [`INT 2Fh AX=1116h` open existing remote file](https://fd.lod.bz/rbil/interrup/network/2f1116.html) | Fill the uninitialized SFT for an opened host file, except for DOS-owned handle count. |
| [`INT 2Fh AX=1117h` create/truncate remote file](https://fd.lod.bz/rbil/interrup/network/2f1117.html) | Fill the uninitialized SFT for a newly created or truncated host file. |
| [`INT 2Fh AX=111Bh` findfirst](https://fd.lod.bz/rbil/interrup/network/2f111b.html) | Start directory enumeration and write the first result into the caller's DTA/search data. |
| [`INT 2Fh AX=111Ch` findnext](https://fd.lod.bz/rbil/interrup/network/2f111c.html) | Continue directory enumeration using redirector-maintained find state. |
| [`INT 2Fh AX=1122h` process termination hook](https://fd.lod.bz/rbil/interrup/network/2f1122.html) | DOS termination path. DOS may subsequently issue `111Dh`; correct cleanup needs per-PSP ownership tracking. |
| [`INT 2Fh AX=112Eh` extended open/create](https://fd.lod.bz/rbil/interrup/network/2f112e.html) | DOS 4+ extended open path. Read action/mode from the SDA, fill the SFT, and return `CX = 1/2/3` for opened/created/replaced. |
| [`INT 21h AX=5D06h` get SDA](https://fd.lod.bz/rbil/interrup/dos_kernel/215d06.html) | Get the Swappable Data Area pointer. HOSTDRV uses the SDA filename buffers and current DTA pointer. |

Practical notes for this project:

- Read and write callbacks use the current DTA pointer from the SDA as the data
  transfer buffer.
- Open and create callbacks receive an SFT at `ES:DI`; the redirector must fill
  the SFT, but DOS owns the SFT handle-count field until close.
- Close (`1106h`) must preserve `ES:DI` and update the SFT reference count.
  HOSTDRV closes the HOSTRPC handle only when the SFT count reaches its final
  reference.
- Extended open/create (`112Eh`) uses the same SFT fill path, but its
  open/create decision comes from the DOS 4+ SDA action and mode words.
- Find-first/find-next results must be written into the DTA search-data area;
  the first byte marks a remote search result by setting bit 7.
- `111Dh` is intentionally not handled yet in `hostdrv.asm`. A correct
  implementation must close only handles owned by the terminating PSP instead
  of blindly closing every host handle.
