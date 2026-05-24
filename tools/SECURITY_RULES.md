# Downloaded tools security rule

Rule: every downloaded tool, SDK, archive, executable, script bundle, or extracted tool directory must be scanned before use.

Minimum steps for every new download:

1. Save the original URL and source repository/release in the relevant notes file.
2. If the source provides SHA256/SHA512/GPG signature, verify it before extraction.
3. Scan the downloaded archive before extraction.
4. Scan the extracted directory before running any binary or script.
5. Record scan result in the task notes or final response.

Windows Defender command:

```powershell
.\tools\scan-download.ps1 .\tools\some-downloaded-file.zip
.\tools\scan-download.ps1 .\tools\some-extracted-tool
```

Direct Defender fallback:

```powershell
& "C:\ProgramData\Microsoft\Windows Defender\Platform\4.18.26040.7-0\MpCmdRun.exe" -Scan -ScanType 3 -File "PATH_TO_SCAN"
```

Do not execute newly downloaded binaries until the scan has completed successfully.
