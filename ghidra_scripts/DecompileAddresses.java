// Decompile specific addresses after forcing function creation.
// Usage: DecompileAddresses.java <out-file> <addr> [addr...]
// @category Analysis

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;

public class DecompileAddresses extends ghidra.app.script.GhidraScript {
    private String safe(String s) {
        return s == null ? "" : s.replace('\n', ' ').replace('\r', ' ');
    }

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("Usage: DecompileAddresses.java <out-file> <addr> [addr...]");
            return;
        }

        DecompInterface ifc = new DecompInterface();
        ifc.setOptions(new DecompileOptions());
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor ctm = new ConsoleTaskMonitor();

        try (PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(args[0])))) {
            for (int i = 1; i < args.length; i++) {
                Address addr = toAddr(args[i]);
                disassemble(addr);
                Function f = getFunctionAt(addr);
                if (f == null) {
                    f = createFunction(addr, "forced_" + args[i]);
                }
                if (f == null) {
                    w.println("/* FAILED TO CREATE FUNCTION @ " + addr + " */");
                    continue;
                }
                w.println();
                w.println("/* ===== FORCED FUNCTION " + f.getName() + " @ " + f.getEntryPoint() + " ===== */");
                DecompileResults res = ifc.decompileFunction(f, 120, ctm);
                if (!res.decompileCompleted()) {
                    w.println("/* DECOMPILE FAILED: " + safe(res.getErrorMessage()) + " */");
                    continue;
                }
                w.println(res.getDecompiledFunction().getC());
            }
        } finally {
            ifc.dispose();
        }
        println("Decompiled requested addresses to " + args[0]);
    }
}
