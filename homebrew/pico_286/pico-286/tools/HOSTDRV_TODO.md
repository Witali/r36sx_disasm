# HOSTDRV TODO

This list tracks gaps found by comparing the R36SX `HOSTDRV.COM` redirector
with FreeDOS SHSUCDX and RBIL `INT 2Fh/AH=11h` redirector notes.

- [x] Store `findfirst`/`findnext` state in the DOS DTA/Search Data Block instead
   of a single resident-global handle. SHSUCDX treats this state as per-DTA,
   which avoids nested or repeated searches trampling each other.
- [x] Implement `INT 2Fh AX=1116h` and `AX=1117h` stack parameters properly:
   access/share mode, create attributes, and create-new vs truncate behavior.
- [x] Add `INT 2Fh AX=112Eh` extended open/create support for DOS versions and
   shells that prefer the extended redirector path.
- [x] Make close semantics match DOS SFT expectations: decrement the SFT reference
   count or call the DOS helper where appropriate, instead of only marking the
   handle count as closed.
- [ ] Reintroduce process cleanup safely: handle `AX=1122h`/`AX=111Dh` only after
   HOSTDRV tracks file ownership per PSP/SFT, so interrupted copies do not leak
   host handles and TSR installation cannot corrupt the caller frame.
- [x] Implement real `chdir` state for the mapped drive by updating CDS/current
   path or keeping equivalent per-drive path state in the guest driver.
- [x] Return real host attributes and timestamps, translating them to DOS
   attribute bits and packed DOS date/time values.
- [x] Strengthen "not for us" checks for every path/SFT/DTA callback and chain to
   the previous `INT 2Fh` handler when the request belongs to another redirector.
- [x] Audit `clear_request`: it uses `rep stosw`, which writes through `ES:DI`.
   Ensure it always targets the resident request block in `CS`, not the caller's
   `ES` from DOS redirector callbacks.
