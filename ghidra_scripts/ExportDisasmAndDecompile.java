// Export disassembly and decompiled C-like output for a program.
// @category Analysis

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class ExportDisasmAndDecompile extends ghidra.app.script.GhidraScript {
    private PrintWriter open(File dir, String name) throws Exception {
        return new PrintWriter(new BufferedWriter(new FileWriter(new File(dir, name))));
    }

    private String safe(String s) {
        return s == null ? "" : s.replace('\t', ' ').replace('\n', ' ').replace('\r', ' ');
    }

    private String bytesToHex(byte[] bytes) {
        if (bytes == null) {
            return "";
        }
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
        File outDir;
        if (args.length > 0) {
            outDir = new File(args[0]);
        } else {
            outDir = new File("ghidra_exports/" + currentProgram.getName());
        }
        outDir.mkdirs();

        Listing listing = currentProgram.getListing();

        try (PrintWriter w = open(outDir, "summary.txt")) {
            w.println("PROGRAM\t" + currentProgram.getName());
            w.println("LANGUAGE\t" + currentProgram.getLanguageID());
            w.println("COMPILER\t" + currentProgram.getCompilerSpec().getCompilerSpecID());
            w.println("IMAGE_BASE\t" + currentProgram.getImageBase());
            w.println("MIN_ADDRESS\t" + currentProgram.getMinAddress());
            w.println("MAX_ADDRESS\t" + currentProgram.getMaxAddress());
            w.println();
            w.println("MEMORY_BLOCKS");
            for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
                w.println(b.getName() + "\t" + b.getStart() + "\t" + b.getEnd()
                    + "\tR=" + b.isRead() + "\tW=" + b.isWrite() + "\tX=" + b.isExecute());
            }
        }

        try (PrintWriter w = open(outDir, "functions.tsv")) {
            w.println("entry\tname\tbody_start\tbody_end");
            FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
            while (fit.hasNext() && !monitor.isCancelled()) {
                Function f = fit.next();
                AddressSetView body = f.getBody();
                w.println(f.getEntryPoint() + "\t" + safe(f.getName()) + "\t"
                    + body.getMinAddress() + "\t" + body.getMaxAddress());
            }
        }

        try (PrintWriter w = open(outDir, "symbols.tsv")) {
            w.println("address\ttype\tname");
            SymbolTable st = currentProgram.getSymbolTable();
            SymbolIterator sit = st.getAllSymbols(true);
            while (sit.hasNext() && !monitor.isCancelled()) {
                Symbol s = sit.next();
                w.println(s.getAddress() + "\t" + s.getSymbolType() + "\t" + safe(s.getName(true)));
            }
        }

        try (PrintWriter w = open(outDir, "disassembly.s")) {
            InstructionIterator it = listing.getInstructions(true);
            Function current = null;
            while (it.hasNext() && !monitor.isCancelled()) {
                Instruction ins = it.next();
                Function f = getFunctionContaining(ins.getAddress());
                if (f != current) {
                    current = f;
                    w.println();
                    if (f != null && f.getEntryPoint().equals(ins.getAddress())) {
                        w.println("### FUNCTION " + f.getName() + " @ " + f.getEntryPoint());
                    } else if (f != null) {
                        w.println("### IN FUNCTION " + f.getName());
                    } else {
                        w.println("### NO FUNCTION");
                    }
                }
                String bytes = "";
                try {
                    bytes = bytesToHex(ins.getBytes());
                } catch (Exception ignored) {
                }
                w.println(ins.getAddress() + ":\t" + bytes + "\t" + ins.toString());
            }
        }

        DecompInterface ifc = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        ifc.setOptions(opts);
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor ctm = new ConsoleTaskMonitor();

        try (PrintWriter w = open(outDir, "decompiled_all.c")) {
            FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
            while (fit.hasNext() && !monitor.isCancelled()) {
                Function f = fit.next();
                w.println();
                w.println("/* ===== FUNCTION " + f.getName() + " @ " + f.getEntryPoint() + " ===== */");
                DecompileResults res = ifc.decompileFunction(f, 60, ctm);
                if (!res.decompileCompleted()) {
                    w.println("/* DECOMPILE FAILED: " + safe(res.getErrorMessage()) + " */");
                    continue;
                }
                w.println(res.getDecompiledFunction().getC());
            }
        }

        try (PrintWriter w = open(outDir, "decompiled_key_functions.c")) {
            String[] keyNames = {
                "retro_init", "retro_load_game", "retro_run", "retro_reset", "retro_unload_game",
                "retro_get_system_info", "retro_get_system_av_info",
                "retro_set_environment", "retro_set_video_refresh", "retro_set_input_state",
                "render", "SetFrameSkip", "check_encrypty", "CheckEncrypty", "wait_empty"
            };
            for (String name : keyNames) {
                SymbolIterator symbols = currentProgram.getSymbolTable().getSymbols(name);
                while (symbols.hasNext() && !monitor.isCancelled()) {
                    Symbol s = symbols.next();
                    Function f = getFunctionAt(s.getAddress());
                    if (f == null) {
                        f = getFunctionContaining(s.getAddress());
                    }
                    w.println();
                    w.println("/* ===== KEY " + name + " @ " + s.getAddress()
                        + " function=" + (f == null ? "<none>" : f.getName()) + " ===== */");
                    if (f == null) {
                        continue;
                    }
                    DecompileResults res = ifc.decompileFunction(f, 60, ctm);
                    if (!res.decompileCompleted()) {
                        w.println("/* DECOMPILE FAILED: " + safe(res.getErrorMessage()) + " */");
                        continue;
                    }
                    w.println(res.getDecompiledFunction().getC());
                }
            }
        }

        ifc.dispose();
        println("Exported analysis to " + outDir.getAbsolutePath());
    }
}
