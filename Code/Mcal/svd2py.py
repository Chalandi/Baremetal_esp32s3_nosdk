import os
from cmsis_svd.parser import SVDParser

# Path to your SVD file
SVD_FILE = "esp32s3.svd"

# Output GDB script filename
GDB_SCRIPT = "svd_gdb.gdb"

# Parse the SVD file
parser = SVDParser.for_xml_file(SVD_FILE)

# Generate the GDB script
with open(GDB_SCRIPT, "w") as f:
    for peripheral in parser.get_device().peripherals:
        if peripheral.base_address is None:
            continue  # Skip peripherals without a base address

        f.write(f"define {peripheral.name}_regs\n")

        for register in peripheral.registers:
            if register.address_offset is None:
                continue  # Skip registers without an address offset

            reg_addr = peripheral.base_address + register.address_offset
            reg_name = register.name.replace(" ", "_")  # Ensure valid variable names
            f.write(f"  set ${reg_name} = *(volatile uint32_t *) {hex(reg_addr)}\n")

        f.write("end\n\n")

print(f"GDB script '{GDB_SCRIPT}' generated successfully!")
