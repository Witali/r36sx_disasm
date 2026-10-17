# R36SX documentary research and production log

Date: 2026-10-17. Language: English. Narrator: Microsoft Jenny
(`en-US-JennyNeural`, synthetic, rate `-4%`).

## Scope and evidence boundary

The film retells Vitaliy Rudik's R36SX / SF3000-style handheld investigation.
Its repository baseline is `ad37bb30` on `single-loop`: 689 commits from
2026-05-24 to 2026-09-27. The first Button Demo build log also contains earlier
2026-05-23 work. Commit dates describe recorded changes; they are not all dates
of successful physical-device tests. The three videos show early device work
in May; later development is illustrated with repository evidence panels.

Uncommitted CPU changes, the local patch configuration, and untracked loop
regression files present at the start of this task were excluded. No device
binary, patch configuration, disk image, or emulator code was changed.

The narration avoids conflicting historic RAM/FPU summaries. It distinguishes
an upstream emulator port from an emulator written from scratch, host tests from
device validation, and Debian image tooling from a confirmed Debian boot.

## Original footage

All three links were already recorded in the repository README. Metadata and
the first video's YouTube oEmbed response identify the uploader as Виталий Рудик
and the channel as `@vitalian1980`.

| ID | Title | Uploaded | Duration |
| --- | --- | --- | --- |
| `LYrICpUo6_A` | R36SX game console reverse engineering | 2026-05-27 | 95 s |
| `MHDpizjIZhw` | R36SX game console additional programs | 2026-05-28 | 236 s |
| `ZWJg3XiHGxE` | R36SX pico-286 emulator | 2026-05-30 | 315 s |

URLs:

- https://www.youtube.com/watch?v=LYrICpUo6_A
- https://www.youtube.com/watch?v=MHDpizjIZhw
- https://www.youtube.com/watch?v=ZWJg3XiHGxE

The sources were downloaded at up to 720p, as H.264 video and Opus audio merged
into MP4. Contact sheets sampled 16 positions throughout each file. Selected
intervals were chosen after viewing those sheets. The new film uses Jenny
narration throughout; original speech/audio is not mixed underneath it.

## Story evidence

| Scene | Supporting repository evidence |
| --- | --- |
| Original SD card / backup | `README.md`, original SD-card section and archived image link |
| Disguised boot files / MIPS | `README.md`, `FIRMWARE_FINDINGS.md` |
| Core loader / compatibility | `FIRMWARE_FINDINGS.md`, `homebrew/libretro_button_demo/BUILD_LOG.md`, patches 001–010 |
| Button Demo / input / sound | `dc8a7f43`, `4710baf3`, `c6c044aa`; first video |
| Tiny MC / hardware bridge | `221a58d8`, `d8500c50`, `DRIVER_SO_DECOMPILATION.md`, Tiny MC build log; second video |
| Cold boot route | `BOOT_ROUTE_FINDINGS.md`, `3490da66`, `a0630803`; quoted device log excerpt |
| Native Pong / app return | `ae7b6ff1`, `36a52b4b`, `d50dcc38`; second video |
| Pico-286 / DOS boot | `85ad7593`, `44990336`, `homebrew/pico_286/README.md`; second and third videos |
| Keyboard / disk menus / presets | May Pico-286 commit history; third video |
| VGA / real programs | Committed Supaplex and SuperScape framebuffer captures referenced by README; Pico-286 build notes |
| Later CPU / regression work | `28c46e23`, `f535f7fc`, `135b7201`, `d8292040`, `ad37bb30` |
| Shell / Debian research | `SESSION_NOTES_2026-06-02.md`, `DEBIAN_12_MIPSEL_INSTALL_PLAN.md`, tracked image scripts |

Experimental Fuse, PRBoom, Fake86, and DOSBox work exists in the history. The
film concentrates on the main firmware → native environment → Pico-286 path
and does not claim those other experiments all became working device releases.
Pico-286 upstream is credited to https://github.com/xrip/pico-286 (MIT).
Related platform research is credited to https://github.com/goph-R/SF3000-RE.

## Tooling, security, and speech authorization

- FFmpeg/ffprobe: existing WinGet installation, version 8.1.1.
- Python: bundled Codex runtime, version 3.12.14; Pillow and NumPy available.
- yt-dlp: official release `2026.08.19`, downloaded from
  https://github.com/yt-dlp/yt-dlp/releases/tag/2026.08.19. The executable's
  SHA256 was verified against the release `SHA2-256SUMS` before execution.
  `tools/scan-download.ps1` reported no threats for the executable and sources.
  The exact URL and hash are retained in the artifact `tools/download.json`.
- The existing edge-tts 7.3.1 dependency folder from the previous Z80 film was
  reused read-only after Defender scanned it successfully:
  `C:/Users/rudol/Downloads/Z80-Tester-Video/materials/community/tools`.
- PyYAML 6.0.3 was downloaded from official PyPI metadata for skill validation;
  its wheel hash matched PyPI, and both wheel and extracted directory passed
  Defender. Provenance is in `local_artifacts/skill-validation/download.json`.
- The first speech submission was blocked by automatic approval review because
  the locally prepared script's exact external disclosure was not yet explicit.
  The user then explicitly authorized the complete `narration.en.txt` payload
  for Microsoft Jenny. Only the narration text, with pronunciation expansions,
  was sent. Code, device logs, and source video files were not sent to that service.

The previous Z80 workflow was located in
`C:/Users/rudol/Downloads/Z80-Tester-Video/materials/project/render.py`.
No global video skill existed in the inspected skill catalog. At the user's
request, a new English global skill was created and installed at
`C:/Users/rudol/.codex/skills/narrated-project-video`. It includes cached Jenny
narration, word timings, production guidance, and a video/audio verification
helper. The standard `quick_validate.py` check passed after providing its PyYAML
dependency. The narration helper was exercised on all 14 actual film scenes.

## Reproduction

The project source is `story.json`, `narration.en.txt`, and `render.py` in this
directory. Sources, voice caches, layouts, intermediate shots, and deliverables
are under the ignored `local_artifacts/reverse_engineering_video/` directory.
The source manifest and final timeline retain source identities, cuts, hashes,
and exact integer frame counts.

Download sources with the verified yt-dlp executable, installed FFmpeg and the
available Node runtime:

```powershell
& .\local_artifacts\reverse_engineering_video\tools\yt-dlp.exe --no-playlist --js-runtimes 'node:C:\Users\rudol\.cache\codex-runtimes\codex-primary-runtime\dependencies\node\bin\node.exe' -f 'bv*[height<=720]+ba/b[height<=720]/best' --merge-output-format mp4 --write-info-json --output 'C:\Work\r36sx_disasm\local_artifacts\reverse_engineering_video\sources\%(id)s.%(ext)s' 'https://www.youtube.com/watch?v=LYrICpUo6_A' 'https://www.youtube.com/watch?v=MHDpizjIZhw' 'https://www.youtube.com/watch?v=ZWJg3XiHGxE'
```

Use the bundled Python executable if `python` is unavailable on PATH. The
following commands use `python` for readability:

```powershell
python C:/Users/rudol/.codex/skills/narrated-project-video/scripts/narrate.py video/r36sx-story/story.json --work-dir local_artifacts/reverse_engineering_video --dependencies C:/Users/rudol/Downloads/Z80-Tester-Video/materials/community/tools
python video/r36sx-story/render.py
python C:/Users/rudol/.codex/skills/narrated-project-video/scripts/verify_video.py local_artifacts/reverse_engineering_video/delivery/R36SX-From-Stock-Firmware-to-DOS-English.mp4 --timeline local_artifacts/reverse_engineering_video/timeline.json --reference-audio local_artifacts/reverse_engineering_video/soundtrack-normalized.wav --report local_artifacts/reverse_engineering_video/delivery/verification.json
```

The renderer disables filter reinitialization for archive video inputs and
checks each rendered shot's frame count. Audio is resampled to 48 kHz before
sample-based padding/trimming, assembled separately, normalized in two passes,
and then muxed.

## Completed output and verification

The finished MP4 is
`local_artifacts/reverse_engineering_video/delivery/R36SX-From-Stock-Firmware-to-DOS-English.mp4`:

- 489 seconds (8:09), 1920×1080, 30 fps, H.264 video, AAC English audio.
- 14,670 video frames, 14 chapter entries, 125 English subtitle cues.
- Size: 80,919,934 bytes.
- SHA256: `41e5f6594fddf7ff055f51d6cadb51648ea419adb42fd066047bc26cb269eb18`.
- Measured loudness: -16.02 LUFS; measured true peak: -1.25 dBTP.
- Full video/audio decoding completed without errors. Frame counts, durations,
  chapter positions, English metadata, and monotonic audio/video DTS passed.
- Final decoded audio matched the normalized reference in every scene. A
  separate comparison to all 14 original Jenny clips at their intended offsets
  passed; the lowest correlation was 0.9991245.
- Extracted embedded subtitle text matched the delivered SRT text for all cues.
- Representative final frames from all 14 scenes were inspected, including
  caption previews, device footage, research panels, framebuffer screenshots,
  and end credits. This was visual and signal verification, not a claimed
  real-time listening review of the complete film.
- Defender reported no threats in the final delivery directory or installed
  global skill directory.

The delivery directory also contains the SRT, English script, YouTube
description and chapter times, cover image, visual contact sheet, source hashes,
timeline, and verification reports. Nothing was uploaded to YouTube.

`PRODUCTION_REPORT.json` retains the compact final artifact and source hashes,
audio checks, and global skill file hashes. The installed skill's narration
helper and verifier have both been exercised on this actual documentary.

## 2026-10-17 text-row alignment revision

The user's screenshot exposed a baseline mismatch between the smaller Consolas
list markers and larger Segoe UI body text. Equal top coordinates put their
baselines at different heights. The renderer now uses one `numbered_point()`
helper: each marker is drawn with Pillow's `ls` baseline anchor at the first
text line's measured ascent. This applies to the full research cards and the
sidebars, including points that wrap onto a second line. A shared `aligned_row()`
helper also aligns the filename/arrow/description table, launcher and driver
diagrams, and date/commit/description history rows. It vertically centers their
combined visible glyph bounds within each row. The chapter label and date now
share an explicit baseline as well.

All nine card layouts and all 14 scene sidebars were regenerated for visual
review. The first intermediate render was stopped when the user's additional
table-alignment feedback arrived, then restarted with the complete correction.
The previous deliverable and its verification report were preserved
under `local_artifacts/reverse_engineering_video/revisions/before-number-alignment/`.

The final revision passed the complete MP4 verifier. Its encoded audio packets
and embedded subtitle text/timing are byte-identical to the previous film. The
489-second duration, 14,670 frames, and 14 chapters were retained. Final review
frames were regenerated from the new MP4 for all scenes.

At the user's request, the global `narrated-project-video` skill was updated in
English as part of this correction. Its production reference now describes
shared baselines, font-ascent alignment for list markers, centered glyph bounds
for boxed rows, and visual review of every card/sidebar variant in the final
encoded film. The entrypoint routes to those instructions and explicitly calls
for checking wrapped points and mixed-font rows. The standard skill validator
passed after updating the two documentation files. Skill file hashes in the
production report were refreshed.

The corrected delivery also has a fresh filename for opening alongside the
earlier review copy:
`local_artifacts/reverse_engineering_video/delivery/R36SX-From-Stock-Firmware-to-DOS-English-aligned.mp4`.
