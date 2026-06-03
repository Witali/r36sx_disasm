# usleep/nanosleep findings

Target: `disk_image/rootfs/lib/libc.so.6`

Ghidra import:
- Language: `MIPS:LE:32:default:default`
- Export folder: `ghidra_exports/libc`

System:
- Kernel modules are under `disk_image/rootfs/lib/modules/4.4.186-release`, so the firmware rootfs matches Linux `4.4.186-release`.
- `disk_image/cubegm/advapi32.dll` is a U-Boot `uImage` named `vmlinux`, MIPS, gzip-compressed.
- Extracted kernel payload:
  - `ghidra_exports/kernel/advapi32_payload.gz`
  - `ghidra_exports/kernel/advapi32_vmlinux_decompressed.bin`
- The decompressed kernel contains: `Linux version 4.4.186-release (linsen.chen@hichip01) (gcc version 6.3.0 (Codescape GNU Tools 2018.09-02 for MIPS MTI Linux) ) #21 PREEMPT Thu Dec 18 18:13:45 CST 2025`.
- The decompressed kernel also contains `cpu_wait` and `wait instruction` strings, so the CPU idle path likely can use the MIPS wait instruction. That is separate from the userspace `usleep()` wrapper.

Important symbols from `functions.tsv`:
- `nanosleep` at `000bfb84`
- `usleep` at `000f5ed0`
- `clock_nanosleep` at `0010ea90`

Decompiled `usleep`:

```c
int usleep(__useconds_t __useconds)
{
  int iVar1;
  timespec local_10;

  local_10.tv_sec = __useconds / 1000000;
  local_10.tv_nsec = (__useconds % 1000000) * 1000;
  iVar1 = nanosleep(&local_10,(timespec *)0x0);
  return iVar1;
}
```

MIPS syscall details:
- `nanosleep` loads `v0 = 0x1046` before `syscall`.
- `clock_nanosleep` loads `v0 = 0x10a9` before `syscall`.
- Error reporting follows the MIPS/Linux convention where `a3 != 0` means syscall error, then libc converts it to `errno`/return value.

Conclusion:
- There is no kernel-side function named `usleep` in normal Linux userspace flow.
- `usleep()` here is a libc wrapper: microseconds -> `struct timespec` -> `nanosleep()`.
- The kernel-side sleep path is the `nanosleep` syscall, which in Linux 4.4 goes through high-resolution timer/scheduler code (`hrtimer_nanosleep`/`do_nanosleep` conceptually), deschedules the calling task, and lets the scheduler run something else or enter idle if nothing is runnable.
- This is not a direct CPU `halt`/MIPS `wait` instruction in the application. Any real low-power CPU wait would happen later in the kernel idle path, not inside `usleep()` itself.
