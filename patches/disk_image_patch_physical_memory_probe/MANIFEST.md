# Physical DDR capacity diagnostic overlay

Copy `MIPS_NATIVE/physical_memory_probe/` onto the console SD card beside
other native applications. Launch `physical_memory_probe` using Tiny MC or
Shell; read its `diagnostics/physical_memory_*.txt` report afterwards.

The hardware result comes from the DDR controller's capacity field, decoded
like the stock bootloader. Linux MemTotal is supplementary context. No NOR,
kernel, AVP, boot DTB, boot script or existing app needs replacement.

Source, build command, interpretation and limitations:
`homebrew/physical_memory_probe/README.md` and `BUILD_LOG.md`.
Two device runs received on 2026-10-18 successfully read `0x0d894084`: DDR3,
256 MiB configured controller capacity; Linux MemTotal is 42.15 MiB. This is
not a complete installed-capacity test. Reports are preserved under
`hardware_info/physical_memory_probe/`.
