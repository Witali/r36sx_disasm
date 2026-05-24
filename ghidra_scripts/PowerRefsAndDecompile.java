// Ghidra headless helper: list references to power/timing imports and decompile named functions.
// @category Analysis

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.util.task.ConsoleTaskMonitor;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public class PowerRefsAndDecompile extends ghidra.app.script.GhidraScript {
    private static final Set<String> REF_NAMES = new HashSet<>(Arrays.asList(
        "usleep", "sleep", "nanosleep", "SDL_Delay", "gettimeofday", "clock_gettime",
        "ioctl", "open", "open64", "system", "mmap", "mmap64", "write", "read",
        "hudi_standby_enter", "hudi_display_suspend", "hudi_display_onoff_set",
        "cube_set_backlight_brightness", "cube_get_backlight_brightness",
        "set_backlight_value2", "set_mipi_onoff", "OS_Delay"
    ));

    private static final Set<String> DECOMP_NAMES = new HashSet<>(Arrays.asList(
        "main", "run_game", "PlayFrame", "DrawFrame", "PauseMenu", "PauseMenuIn",
        "control_set", "XintiaoThread", "DisplayThread", "SoundPlayer",
        "cube_set_backlight_brightness", "cube_get_backlight_brightness",
        "OS_Delay", "set_backlight_value2", "set_mipi_onoff",
        "joystick_key_task", "receive_hdmi_tx_event_func", "cube_get_msg_pthread_cb"
        , "check_exit_game", "exit_game", "enter_multios_app", "set_bootup_slot",
        "api_osd_show_onoff", "set_backlight_value", "fbdev_backlight_init"
    ));

    @Override
    protected void run() throws Exception {
        println("PROGRAM: " + currentProgram.getName());
        println("LANGUAGE: " + currentProgram.getLanguageID());
        println("COMPILER: " + currentProgram.getCompilerSpec().getCompilerSpecID());

        SymbolTable st = currentProgram.getSymbolTable();
        println("\n== References to timing/power-ish symbols ==");
        SymbolIterator it = st.getAllSymbols(true);
        while (it.hasNext() && !monitor.isCancelled()) {
            Symbol s = it.next();
            String name = s.getName();
            if (!REF_NAMES.contains(name)) {
                continue;
            }
            Address addr = s.getAddress();
            println("\nSYMBOL " + name + " @ " + addr);
            Reference[] refs = getReferencesTo(addr);
            if (refs.length == 0) {
                println("  no refs");
            }
            for (Reference r : refs) {
                Function f = getFunctionContaining(r.getFromAddress());
                println("  from " + r.getFromAddress() + " in " + (f == null ? "<no function>" : f.getName()));
            }
        }

        DecompInterface ifc = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        ifc.setOptions(opts);
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor ctm = new ConsoleTaskMonitor();

        println("\n== Decompile selected functions ==");
        FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
        while (fit.hasNext() && !monitor.isCancelled()) {
            Function f = fit.next();
            if (!DECOMP_NAMES.contains(f.getName())) {
                continue;
            }
            println("\n----- FUNCTION " + f.getName() + " @ " + f.getEntryPoint() + " -----");
            DecompileResults res = ifc.decompileFunction(f, 60, ctm);
            if (!res.decompileCompleted()) {
                println("DECOMPILE FAILED: " + res.getErrorMessage());
                continue;
            }
            println(res.getDecompiledFunction().getC());
        }
        ifc.dispose();
    }
}
