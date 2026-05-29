# Sopwith floppy payload

This directory contains the tiny DOS launcher and notes used to build `sopwith.img`.

The executable payload is taken from DOS Games Archive:

- Game page: <https://www.dosgamesarchive.com/download/sopwith-the-authors-edition/>
- Download page: <https://www.dosgamesarchive.com/file/sopwith-the-authors-edition/swae-box>
- ZIP file: `swae-box.zip`
- ZIP SHA256: `689BC2B80BFE9A7079F94C7A550BA2E0D7FA1842DC4F9C6BCED225A7424BAFBA`

The original DOSBox launcher from the archive uses DOSBox-only commands (`config`, `loadfix`),
so the R36SX image uses a minimal real-DOS launcher:

```bat
@ECHO OFF
SW.EXE
```
