import gdb
import subprocess
import re

###########################################################################################################################################
# Print the xtensa lx7 general purpose registers
###########################################################################################################################################
class XtensaRegs(gdb.Command):
    """Prints Xtensa LX7 registers A0-A15."""

    def __init__(self):
        super(XtensaRegs, self).__init__("_regs", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        registers = [f"a{i}" for i in range(16)]
        output = []

        for reg in registers:
            try:
                value = int(gdb.parse_and_eval(f"${reg}")) & 0xffffffff
                output.append(f"{reg.upper():>3} = 0x{value:08X}")
            except gdb.error:
                output.append(f"{reg.upper():>3} = <unavailable>")

        print("\n".join(output))

###########################################################################################################################################
# Print the xtensa lx7 program counter with the offset in the current executed function
###########################################################################################################################################
class XtensaPC(gdb.Command):
    """Prints the Xtensa Program Counter (PC) register."""

    def __init__(self):
        super(XtensaPC, self).__init__("_pc", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        pc_value = int(gdb.parse_and_eval("$pc"))
        # Run `info symbol` on the PC address to get function information
        result = gdb.execute(f"info symbol {pc_value}", to_string=True)
        
        # Check if it's a valid symbol (function)
        if "in" in result:
            function_info = result.split(" in")[0].strip()
            # Check if an offset exists (e.g., "main + 69")
            if " + " in function_info:
                function_name, offset_str = function_info.split(" + ")
                offset = int(offset_str)
            else:
                function_name = function_info
                offset = 0
            # Display the function name and offset in hexadecimal if exists
            if offset != 0:
                hex_offset = f"+0x{offset:X}"
            else:
                hex_offset = ""
            print(f"PC  = 0x{pc_value:08X} ({function_name}{hex_offset})")
        else:
            print(f"PC  = 0x{pc_value:08X}")


###########################################################################################################################################
# Print the xtensa lx7 processor status with detailed fields value and description
###########################################################################################################################################
class XtensaPS(gdb.Command):
    """Prints the Xtensa Program status (PS) register."""

    def __init__(self):
        super(XtensaPS, self).__init__("_ps", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        ps_value = int(gdb.parse_and_eval("$ps"))

        # Define the PS fields, their corresponding bit positions, and their descriptions
        fields = {
            "INTLEVEL": ("Interrupt-level", (ps_value >> 0) & 0xF),
            "EXCM":     ("Exception mode (0: normal operation, 1: exception operation)", (ps_value >> 4) & 0x1),
            "UM":       ("User vector mode (0: kernel vector mode, 1: user vector mode)", (ps_value >> 5) & 0x1),
            "RING":     ("Privilege level for MMU/MPU", (ps_value >> 6) & 0x1),
            "OWB":      ("Old window base", (ps_value >> 8) & 0xf),
            "CALLINC":  ("Call increment", (ps_value >> 16) & 0x3),
            "WOE":      ("Window overflow-detection (0: disabled, 1: enabled)", (ps_value >> 18) & 0x1),
        }

        # Print the PS register in the desired format
        print(f"PS  = 0x{ps_value:08X}")
        print("-" * 90)  # Divider for readability
        print(f"{'Field':<15}{'Value':<12}{'Description'}")
        print("-" * 90)
        
        # Iterate over each field and display it
        for field, (desc, value) in fields.items():
            print(f"{field:<15}{value:#x}          {desc}")
        
        print("-" * 90)  # Divider at the bottom


###########################################################################################################################################
# list all symbols using nm or list all sections using readelf
###########################################################################################################################################
class list_symbols(gdb.Command):
    """Prints either symbols or sections from the ELF binary."""

    def __init__(self):
        super(list_symbols, self).__init__("_list", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        if arg == "symbols":
            self.list_symbols()
        elif arg == "sections":
            self.list_sections()
        else:
            print("Usage: _list <symbols|sections>")
    
    def list_symbols(self):
        # Get the path to the ELF binary being debugged in GDB
        program = gdb.execute('info files', to_string=True)
        # Extract the ELF binary path from the output
        lines = program.splitlines()
        elf_path = None
        for line in lines:
            if 'Symbols from' in line:
                elf_path = line.split('Symbols from ')[1].strip('".')
                break
        
        if elf_path:
            # Run nm command on the ELF binary
            try:
                result = subprocess.check_output(['xtensa-esp32s3-elf-nm', '--defined-only', elf_path], stderr=subprocess.STDOUT)
                print(result.decode('utf-8'))
            except subprocess.CalledProcessError as e:
                print(f"Error calling nm: {e.output.decode('utf-8')}")
        else:
            print("Error: Could not find the ELF binary path.")
    
    def list_sections(self):
        # Get the path to the ELF binary being debugged in GDB
        program = gdb.execute('info files', to_string=True)
        # Extract the ELF binary path from the output
        lines = program.splitlines()
        elf_path = None
        for line in lines:
            if 'Symbols from' in line:
                elf_path = line.split('Symbols from ')[1].strip('".')
                break
        
        if elf_path:
            # Run readelf command on the ELF binary to list sections
            try:
                result = subprocess.check_output(['xtensa-esp32s3-elf-readelf', '-S', elf_path], stderr=subprocess.STDOUT)
                print(result.decode('utf-8'))
            except subprocess.CalledProcessError as e:
                print(f"Error calling readelf: {e.output.decode('utf-8')}")
        else:
            print("Error: Could not find the ELF binary path.")

###########################################################################################################################################
# Open source code in VS Code if symbol or address exists, if not argument specified then open source code of the current $pc
###########################################################################################################################################
class open_in_vs(gdb.Command):
    """Open the source file of the symbol or the address in VS code editor, if no argument the open source code of the current $pc."""

    def __init__(self):
        super(open_in_vs, self).__init__("_vs", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        if not arg:
            arg = int(gdb.parse_and_eval("$pc"))

        # Try to resolve the argument to an address if it's a symbol
        try:
            address_info = gdb.execute(f"info address {arg}", to_string=True)
            match = re.search(r'is at (0x[0-9a-fA-F]+)', address_info)
            if match:
                arg = match.group(1)  # Convert symbol to address
        except gdb.error:
            if arg == int(gdb.parse_and_eval("$pc")) or bool(re.fullmatch(r'0x[0-9a-fA-F]+', arg)):
                pass
            else:
                print("Unknown symbol, please use a defined symbol or hexadecimal address 0x... !")
                return

        result = gdb.execute(f"list *{arg}", to_string=True)
        match = re.search(r'\(([^)]+)\)', result)
        if match:
            file_with_line = match.group(1)
            try:
                subprocess.run([r"code.cmd", "--goto", file_with_line], check=True,stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            except FileNotFoundError:
                print("Error: VS Code (code.cmd) not found in PATH")
            except subprocess.CalledProcessError as e:
                print(f"Error opening file in VS Code: {e}")
        else:
            print("address not matching any code !")
            return

###########################################################
# Generic function
###########################################################
open_in_vs()
list_symbols()

###########################################################
# Target specific function
###########################################################
XtensaRegs()
XtensaPC()
XtensaPS()
