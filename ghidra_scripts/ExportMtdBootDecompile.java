// Export annotated C-like decompile for the R36SX MTD boot partition.
// Usage: ExportMtdBootDecompile.java <out-dir> <first|stage2>
// @category Analysis

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

public class ExportMtdBootDecompile extends ghidra.app.script.GhidraScript {
    private static class Note {
        final String name;
        final String note;

        Note(String name, String note) {
            this.name = name;
            this.note = note;
        }
    }

    private PrintWriter open(File dir, String name) throws Exception {
        return new PrintWriter(new BufferedWriter(new FileWriter(new File(dir, name))));
    }

    private Address addr(long offset) {
        return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(offset);
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

    private boolean inRange(long value, long start, long end) {
        return Long.compareUnsigned(value, start) >= 0 && Long.compareUnsigned(value, end) <= 0;
    }

    private void addManualNotes(Map<Long, Note> notes, String profile) {
        if ("first".equals(profile)) {
            notes.put(0xbfe00800L, new Note("first_stage_entry_copy_and_init",
                "High confidence: NOR entry. Checks a boot magic at 0xbfe00014; if missing, copies code/data from the flash alias around 0xafc02000 to 0xbfe02000, then calls low-level init routines."));
            notes.put(0xbfe01278L, new Note("first_stage_spi_nor_command_path",
                "Medium confidence: talks to the SPI/SF controller registers around 0xb882e0c8 and prepares small command buffers on the stack."));
            notes.put(0xbfe015d4L, new Note("first_stage_peripheral_gate_for_usb_blocks",
                "Medium confidence: compares the argument with 0xb8844000 and 0xb8850000, matching the DTS USB host controller MMIO windows, then toggles bits in 0xb8800080."));
            notes.put(0xbfe01648L, new Note("first_stage_peripheral_reset_sequence",
                "Medium confidence: writes several control/status registers in the selected MMIO window after gating the peripheral clock/reset."));
            notes.put(0xbfe02d20L, new Note("first_stage_clock_or_pll_select",
                "Medium confidence: adjusts 0xb882e004/0xb882e094 and 0xb880007c based on a frequency-like argument."));
            notes.put(0xbfe02db4L, new Note("first_stage_sfspi_transfer_setup",
                "Medium confidence: programs the SPI/SF controller registers around 0xb882e058 and starts a transfer/status wait loop."));
        } else {
            notes.put(0x89eb0000L, new Note("stage2_entry_cp0_cache_and_handoff",
                "High confidence: RAM-resident stage2 entry. Saves boot arguments, edits CP0 config registers, clears a small BSS-like range, sets stack/return trampoline, and jumps onward."));
            notes.put(0x89eb0118L, new Note("stage2_jump_to_cached_alias",
                "Medium confidence: builds a KSEG0/cached alias using 0xa0000000 masking and jumps through it."));
            notes.put(0x89eb0130L, new Note("stage2_cache_probe_and_invalidate",
                "Medium confidence: reads CP0 Config1, derives cache geometry, and uses MIPS cache operations over the cache ways/sets."));
            notes.put(0x89eb032cL, new Note("stage2_main_handoff_wrapper",
                "Medium confidence: main wrapper after cache setup. Prepares stack locals and calls deeper loader/decompressor routines."));
            notes.put(0x89eb23a4L, new Note("stage2_stream_context_prepare",
                "Low/medium confidence: prepares a stream/context object used by the packed payload processing code."));
            notes.put(0x89eb23c8L, new Note("stage2_stream_copy_or_decode_loop",
                "Low/medium confidence: loop over input/output counters; appears to move or decode packed payload data."));
            notes.put(0x89eb2890L, new Note("stage2_stream_context_release",
                "Low/medium confidence: calls a function pointer from a context and clears a stream buffer pointer."));
            notes.put(0x89eb2a28L, new Note("stage2_payload_header_parse",
                "Medium confidence: parses a small packed-payload header into size/type fields and validates ranges."));
            notes.put(0x89eb2b98L, new Note("stage2_payload_decode_dispatch",
                "Low/medium confidence: higher-level packed-payload decode dispatcher; checks mode/status values and invokes decode helpers."));
            notes.put(0x89eb2cb4L, new Note("stage2_payload_size_probe",
                "Low/medium confidence: helper used to probe or return decoded payload size/status."));
        }
    }

    private Set<Long> collectSeeds(long start, long end, String profile) throws Exception {
        Set<Long> seeds = new TreeSet<>();
        if ("first".equals(profile)) {
            seeds.add(0xbfe00800L);
        } else {
            seeds.add(0x89eb0000L);
            seeds.add(0x89eb0118L);
            seeds.add(0x89eb0130L);
            seeds.add(0x89eb032cL);
        }

        for (long cur = start; Long.compareUnsigned(cur + 3, end) <= 0; cur += 4) {
            Address a = addr(cur);
            int word;
            try {
                word = currentProgram.getMemory().getInt(a);
            } catch (Exception e) {
                continue;
            }
            int op = (word >>> 26) & 0x3f;
            if (op == 2 || op == 3) {
                long target = ((cur + 4) & 0xf0000000L) | (((long) word & 0x03ffffffL) << 2);
                if (inRange(target, start, end)) {
                    seeds.add(target);
                }
            }
            if ((word & 0xffff0000) == 0x27bd0000) {
                // Common MIPS function prologue: addiu sp,sp,-N.
                seeds.add(cur);
            }
            if (word == 0x03e00008 && inRange(cur + 8, start, end)) {
                // After jr ra plus delay slot, a new function often starts.
                seeds.add(cur + 8);
            }
        }
        return seeds;
    }

    private void createSeedFunctions(Set<Long> seeds, Map<Long, Note> notes) throws Exception {
        for (long seed : seeds) {
            if (monitor.isCancelled()) {
                return;
            }
            Address a = addr(seed);
            try {
                disassemble(a);
            } catch (Exception ignored) {
            }
        }
        analyzeChanges(currentProgram);
        for (long seed : seeds) {
            if (monitor.isCancelled()) {
                return;
            }
            Address a = addr(seed);
            Function f = getFunctionAt(a);
            if (f == null) {
                String name = notes.containsKey(seed) ? notes.get(seed).name : "sub_" + Long.toHexString(seed);
                try {
                    f = createFunction(a, name);
                } catch (Exception ignored) {
                    f = null;
                }
            }
            if (f != null && notes.containsKey(seed)) {
                Note note = notes.get(seed);
                try {
                    f.setName(note.name, SourceType.USER_DEFINED);
                } catch (Exception ignored) {
                }
                f.setComment(note.note);
            }
        }
        analyzeChanges(currentProgram);
    }

    private String autoNote(Function f) {
        String manual = f.getComment();
        if (manual != null && !manual.isEmpty()) {
            return manual;
        }

        StringBuilder hints = new StringBuilder();
        Listing listing = currentProgram.getListing();
        InstructionIterator it = listing.getInstructions(f.getBody(), true);
        while (it.hasNext() && !monitor.isCancelled()) {
            Instruction ins = it.next();
            String text = ins.toString();
            if (text.contains("0xb882") || text.contains("b882")) {
                if (hints.indexOf("SPI/SF or clock-controller MMIO") < 0) {
                    hints.append("References 0xb882... registers, likely SPI/SF or clock-controller MMIO. ");
                }
            }
            if (text.contains("0xb884") || text.contains("b884")) {
                if (hints.indexOf("USB/MMC-side peripheral MMIO") < 0) {
                    hints.append("References 0xb884... registers, likely USB/MMC-side peripheral MMIO. ");
                }
            }
            if (text.contains("c0_config") || text.contains("c0_config1") || text.contains("cache")) {
                if (hints.indexOf("CP0/cache") < 0) {
                    hints.append("Touches CP0/cache state. ");
                }
            }
            if (text.contains("0x89da") || text.contains("89da")) {
                if (hints.indexOf("handoff") < 0) {
                    hints.append("References 0x89da..., likely handoff or external image entry/data. ");
                }
            }
        }
        if (hints.length() == 0) {
            return "No strong automatic classification; name and body are generated from raw MIPS control flow.";
        }
        return hints.toString().trim();
    }

    private void exportSummary(File outDir, long codeStart, long codeEnd, String profile) throws Exception {
        try (PrintWriter w = open(outDir, "summary.txt")) {
            w.println("PROGRAM\t" + currentProgram.getName());
            w.println("PROFILE\t" + profile);
            w.println("LANGUAGE\t" + currentProgram.getLanguageID());
            w.println("COMPILER\t" + currentProgram.getCompilerSpec().getCompilerSpecID());
            w.println("IMAGE_BASE\t" + currentProgram.getImageBase());
            w.println("CODE_RANGE\t" + addr(codeStart) + "\t" + addr(codeEnd));
            w.println("MIN_ADDRESS\t" + currentProgram.getMinAddress());
            w.println("MAX_ADDRESS\t" + currentProgram.getMaxAddress());
            w.println();
            w.println("MEMORY_BLOCKS");
            for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
                w.println(b.getName() + "\t" + b.getStart() + "\t" + b.getEnd()
                    + "\tR=" + b.isRead() + "\tW=" + b.isWrite() + "\tX=" + b.isExecute());
            }
        }
    }

    private void exportFunctions(File outDir) throws Exception {
        try (PrintWriter w = open(outDir, "functions.tsv")) {
            w.println("entry\tname\tbody_start\tbody_end\tnote");
            FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
            while (fit.hasNext() && !monitor.isCancelled()) {
                Function f = fit.next();
                AddressSetView body = f.getBody();
                w.println(f.getEntryPoint() + "\t" + safe(f.getName()) + "\t"
                    + body.getMinAddress() + "\t" + body.getMaxAddress() + "\t" + safe(autoNote(f)));
            }
        }
    }

    private void exportSymbols(File outDir) throws Exception {
        try (PrintWriter w = open(outDir, "symbols.tsv")) {
            w.println("address\ttype\tname");
            SymbolTable st = currentProgram.getSymbolTable();
            SymbolIterator sit = st.getAllSymbols(true);
            while (sit.hasNext() && !monitor.isCancelled()) {
                Symbol s = sit.next();
                w.println(s.getAddress() + "\t" + s.getSymbolType() + "\t" + safe(s.getName(true)));
            }
        }
    }

    private void exportDisassembly(File outDir, long codeStart, long codeEnd) throws Exception {
        try (PrintWriter w = open(outDir, "disassembly.s")) {
            AddressSet range = new AddressSet(addr(codeStart), addr(codeEnd));
            InstructionIterator it = currentProgram.getListing().getInstructions(range, true);
            Function current = null;
            while (it.hasNext() && !monitor.isCancelled()) {
                Instruction ins = it.next();
                Function f = getFunctionContaining(ins.getAddress());
                if (f != current) {
                    current = f;
                    w.println();
                    if (f != null && f.getEntryPoint().equals(ins.getAddress())) {
                        w.println("### FUNCTION " + f.getName() + " @ " + f.getEntryPoint());
                        w.println("### NOTE " + autoNote(f));
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
                w.println(ins.getAddress() + ":\t" + bytes + "\t" + ins);
            }
        }
    }

    private void exportDecompile(File outDir) throws Exception {
        DecompInterface ifc = new DecompInterface();
        ifc.setOptions(new DecompileOptions());
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor ctm = new ConsoleTaskMonitor();
        try (PrintWriter w = open(outDir, "decompiled_all.c")) {
            FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
            while (fit.hasNext() && !monitor.isCancelled()) {
                Function f = fit.next();
                w.println();
                w.println("/* ===== FUNCTION " + f.getName() + " @ " + f.getEntryPoint() + " =====");
                w.println(" * " + autoNote(f));
                w.println(" */");
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
    }

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("Usage: ExportMtdBootDecompile.java <out-dir> <first|stage2>");
            return;
        }

        File outDir = new File(args[0]);
        outDir.mkdirs();
        String profile = args[1];

        long codeStart;
        long codeEnd;
        if ("first".equals(profile)) {
            codeStart = 0xbfe00800L;
            codeEnd = 0xbfe02e4fL;
        } else if ("stage2".equals(profile)) {
            codeStart = 0x89eb0000L;
            codeEnd = 0x89eb2e2fL;
        } else {
            throw new IllegalArgumentException("Unknown profile: " + profile);
        }

        Map<Long, Note> notes = new TreeMap<>();
        addManualNotes(notes, profile);
        Set<Long> seeds = collectSeeds(codeStart, codeEnd, profile);
        seeds.addAll(notes.keySet());
        createSeedFunctions(seeds, notes);

        exportSummary(outDir, codeStart, codeEnd, profile);
        exportFunctions(outDir);
        exportSymbols(outDir);
        exportDisassembly(outDir, codeStart, codeEnd);
        exportDecompile(outDir);

        println("Exported MTD boot decompile to " + outDir.getAbsolutePath());
    }
}
