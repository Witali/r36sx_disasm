# Runtime CPU frequency selection: feasibility and integration requirements

Status on 2026-10-17: researched; not implemented or enabled in Tiny MC.

The actual console selects 1188 MHz. Vendor source and the device tree provide
lower fixed clock sources, so a frequency selector is feasible at the hardware
level. A normal runtime control mechanism on the stock Linux firmware has not
been established. See `hardware_info/clock_probe/README.md` and
`disasm/linux/hc16xx-clock/README.md` for device and kernel evidence.

## Intended user interaction

A future system-settings panel can present a small set of candidate profiles:

| Profile | Requested clock | Validation status |
|---|---:|---|
| Performance | 1188 MHz | Observed stock setting |
| Balanced | 900 MHz | Hardware source identified; runtime transition untested |
| Economy | 594 MHz | Hardware source identified; runtime transition untested |

The setting affects the whole Linux environment and launched applications,
including emulators. It is not an emulated-PC speed setting. Keep 1188 MHz as
the initial/default boot behavior. Do not persist a lower frequency at boot
until actual transition, timing and recovery tests pass. The 24 MHz standby
source belongs to the suspend sequence and is not a proposed interactive profile.

## Required kernel support

Tiny MC should request a frequency through a verified kernel interface, ideally
a CPUFreq driver. The kernel must coordinate clock-provider operations and all
affected timekeeping, timer-event, delay-calibration and dependent-clock state.
With the sole observed `MIPS` clocksource, writing the hardware selector behind
the kernel's back is not an established correct implementation.

Do not call the exported `hc16xx_set_cpu_clk()` as if it accepted a requested
runtime MHz value: it has no such parameter and replays boot device-tree values.
Do not use a userspace `/dev/mem` register write as the production backend.

The next implementation prerequisite is matching vendor kernel source/config
or a documented and verified existing vendor control interface. The currently
retained HCRTOS source describes the other execution environment and boot code;
it is insufficient by itself to build a correctly integrated Linux runtime driver.

A follow-up source search found
<https://github.com/ozkaoz/r36sx-hclinux/blob/main/docs/ONBOARDING.md>.
That project's own documentation identifies a separate 2.1 GB vendor SDK,
`hclinux-2024.02.y.2.tar.gz`, which is not included in its Git repository.
It gives SHA256
`e3211b41f8d649c7d7838f7f19b8cca5cf30ba6cb1ff9545be6943845fbf8d5d`.
This is a potentially useful source lead, not a verified matching kernel or
an existing CPU-frequency solution. The archive was not downloaded or tested;
no bootloader or replacement kernel from that project was deployed.

## Acceptance checks before Tiny MC integration

- Read back the selected hardware source and kernel-reported rate after every
  transition; handle rejected/unsupported rates without reporting success.
- Verify monotonic/wall time and sleeps against an independent reference,
  including several counter wraps, while changing down and back up.
- Check input repeat, display, audio, USB and SD I/O at every proposed rate.
- Compare real workload throughput and power use; frequency reduction alone
  does not establish a useful battery-life improvement.
- Exercise failure recovery. Any timed automatic restore must rely on a
  validated kernel timebase; a userspace sleep is not a dependable recovery
  mechanism if changing frequency invalidates that very timebase.
- Then add the UI control and test application launches/returns at each rate.

This investigation changed no Tiny MC executable, kernel, card or clock setting.
