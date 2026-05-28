# disk_image_patch_005

Date: 2026-05-27

Purpose: test whether `rkgame` ignores/does not support the custom `.demo` extension.

## Why

`rkgame` contains a built-in default extension table. It includes `GB`, `GBC`, `GBA`, `NES`, etc., but no `DEMO`. The previous test launched `GB/button.demo`, which can appear in `GB/filelist.csv`, but may fail core selection and return to the launcher.

This patch adds a second dummy launch file with a normal Game Boy extension:

```text
GB/button.gb
```

and explicitly maps it to the homebrew core:

```xml
<file name="GB/button.gb" core="libemu_buttondemo.so" />
```

## Changed files

```text
cubegm/cores/libemu_buttondemo.so
cubegm/cores/config.xml
cubegm/cores/filelist.xml
GB/button.gb
GB/button.demo
GB/filelist.csv
```

## Test

After copying this patch contents into the SD/image root, launch:

```text
Button Demo GB
```

from the Game Boy list.

Expected result if `cores/filelist.xml` override works:

```text
The static button demo frame stays on screen.
```

If it still briefly black-screens and returns to launcher, then either:

- `rkgame` ignores `cores/filelist.xml` for the GB list path;
- `GetFileCore` does not match the exact path/name being launched;
- the homebrew core ABI still misses a launcher-specific expectation.

## Current SO

```text
SHA256: B9279DFB25510373527466A8287AA51A9CE56B547BB63A0DFAED70FDFF092493
```
