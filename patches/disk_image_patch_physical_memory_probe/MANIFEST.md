# Physical DDR capacity diagnostic overlay

Copy `MIPS_NATIVE/physical_memory_probe/` onto the console SD card beside
other native applications. Launch `physical_memory_probe` using Tiny MC or
Shell; read its `diagnostics/physical_memory_*.txt` report afterwards.

The hardware result comes from the DDR controller's capacity field, decoded
like the stock bootloader. Linux MemTotal is supplementary context. No NOR,
kernel, AVP, boot DTB, boot script or existing app needs replacement.

Source, build command, interpretation and limitations:
`homebrew/physical_memory_probe/README.md` and `BUILD_LOG.md`.
Physical operation and the actual DDR register value await a device run.
