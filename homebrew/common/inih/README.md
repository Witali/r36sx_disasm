# inih

This directory vendors `inih` release `r62` from:

https://github.com/benhoyt/inih

`inih` is a small callback-based C INI parser.  The R36SX homebrew builds use it
for runtime `.conf` files instead of ad-hoc line parsing.

License: BSD-3-Clause, see `LICENSE.txt`.

Local build settings currently used by the native apps:

```text
INI_HANDLER_LINENO=1
INI_MAX_LINE=512
INI_ALLOW_MULTILINE=0
```
