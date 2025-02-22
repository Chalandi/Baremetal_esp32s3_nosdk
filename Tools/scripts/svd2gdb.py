import os
from cmsis_svd.parser import SVDParser

# Path to your SVD file
SVD_FILE = "esp32s3.svd"  # Modify this to your actual SVD file

# Output GDB script filename
GDB_SCRIPT = "svd_gdb.gdb"  # Modify this if needed

# Parse the SVD file
parser = SVDParser.for_xml_file(SVD_FILE)

# Generate the GDB script
with open(GDB_SCRIPT, "w") as f:
    for peripheral in parser.get_device().peripherals:
        if peripheral.base_address is None:
            continue  # Skip peripherals without a base address

        f.write(f"define {peripheral.name}_regs\n")

        for register in peripheral.registers:
            # Check if it's a normal register or a register array
            if hasattr(register, "address_offset"):
                # Regular register
                reg_addr = peripheral.base_address + register.address_offset
                reg_name = register.name.replace(" ", "_")  # Ensure valid variable names
                f.write(f"  set ${reg_name} = *(volatile uint32_t *) {hex(reg_addr)}\n")
                f.write(f"  printf \"{reg_name} = 0x%08x\\n\", ${reg_name}\n")  # Print the value
            elif hasattr(register, "registers"):
                # Register array (multiple registers under one array)
                for reg in register.registers:
                    # Ensure the individual register has an address_offset
                    if hasattr(reg, "address_offset"):
                        reg_addr = peripheral.base_address + reg.address_offset
                        
                        # Create a unique name for each register in the array
                        reg_name = f"{register.name}_{reg.index}" if hasattr(reg, "index") else f"register_{hex(reg_addr)}"
                        
                        f.write(f"  set ${reg_name} = *(volatile uint32_t *) {hex(reg_addr)}\n")
                        f.write(f"  printf \"{reg_name} = 0x%08x\\n\", ${reg_name}\n")  # Print the value

        f.write("end\n\n")

print(f"GDB script '{GDB_SCRIPT}' generated successfully!")
