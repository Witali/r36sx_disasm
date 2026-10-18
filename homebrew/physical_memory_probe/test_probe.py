"""Check decoder boundaries and report survival when launcher stdout closes."""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

program = Path(sys.argv[1]).resolve()
prefix = sys.argv[2:] + [str(program)]
cases = [("0x00800003", "DDR3", "128 MiB"),
         ("0x00800004", "DDR3", "256 MiB"),
         ("0x00000003", "DDR2", "128 MiB"),
         ("0x00000000", "DDR2", "16 MiB"),
         ("0x00800007", "DDR3", "2048 MiB")]
for value, kind, size in cases:
    result = subprocess.run(prefix + ["--decode-register", value], capture_output=True, text=True)
    assert result.returncode == 0, result.stderr
    assert "OFFLINE DECODE ONLY" in result.stdout
    assert f"DDR type: {kind}" in result.stdout
    assert f"TOTAL DDR configured in hardware: {size}" in result.stdout
for value, expected in [("0xffffffff", 1), ("0x100000000", 2), ("-1", 2),
                        ("not-a-number", 2), ("", 2)]:
    result = subprocess.run(prefix + ["--decode-register", value], capture_output=True, text=True)
    assert result.returncode == expected, (value, result.returncode)
    assert "TOTAL DDR configured in hardware:" not in result.stdout
result = subprocess.run(prefix + ["--unknown"], capture_output=True)
assert result.returncode == 2
if len(sys.argv) == 2:
    # Native host build only: QEMU may resolve /proc/self/exe to its own binary.
    with tempfile.TemporaryDirectory(prefix="physical-probe-", dir=program.parent) as folder:
        executable = Path(folder) / "physical_memory_probe"
        shutil.copy2(program, executable)
        for _ in range(2):
            child = subprocess.Popen([str(executable), "--no-hardware"], stdout=subprocess.PIPE)
            child.stdout.close()
            assert child.wait(timeout=8) == 0
        reports = list((Path(folder) / "diagnostics").glob("physical_memory_*.txt"))
        assert len(reports) == 2
        for report in reports:
            text = report.read_text()
            assert "physical capacity UNKNOWN" in text and "Done." in text
            assert "TOTAL DDR configured in hardware:" not in text
print("PASS: 5 capacity/type cases, 5 invalid values, CLI validation" +
      (", unique reports and closed stdout" if len(sys.argv) == 2 else ", emulated MIPS decoder"))
