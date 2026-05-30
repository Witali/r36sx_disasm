// Dump linear disassembly for one or more address ranges.
// Usage: DumpDisasmRanges.java <out-file> <start> <length> [<start> <length> ...]
// @category Analysis

import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;

public class DumpDisasmRanges extends ghidra.app.script.GhidraScript {
    private String bytesToHex(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < bytes.length; i++) {
            if (i != 0) {
                sb.append(' ');
            }
            sb.append(String.format("%02x", bytes[i] & 0xff));
        }
        return sb.toString();
    }

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 3 || (args.length % 2) != 1) {
            println("Usage: DumpDisasmRanges.java <out-file> <start> <length> [<start> <length> ...]");
            return;
        }

        try (PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(args[0])))) {
            for (int i = 1; i < args.length; i += 2) {
                Address start = toAddr(args[i]);
                long length = Long.decode(args[i + 1]);
                Address end = start.add(length);

                w.println();
                w.println("### RANGE " + start + " +" + length);

                Address cur = start;
                while (cur.compareTo(end) < 0 && !monitor.isCancelled()) {
                    Instruction ins = currentProgram.getListing().getInstructionAt(cur);
                    if (ins == null) {
                        disassemble(cur);
                        ins = currentProgram.getListing().getInstructionAt(cur);
                    }
                    if (ins != null && ins.getAddress().equals(cur)) {
                        String bytes = "";
                        try {
                            bytes = bytesToHex(ins.getBytes());
                        } catch (Exception ignored) {
                        }
                        w.println(ins.getAddress() + ":\t" + bytes + "\t" + ins);
                        cur = ins.getMaxAddress().add(1);
                    } else {
                        byte b = getByte(cur);
                        w.println(cur + ":\t" + String.format("%02x", b & 0xff) + "\tdb 0x" + String.format("%02x", b & 0xff));
                        cur = cur.add(1);
                    }
                }
            }
        }
        println("Dumped disassembly ranges to " + args[0]);
    }
}
