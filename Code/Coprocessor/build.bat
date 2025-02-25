@echo off

echo +++ Compiling:
riscv32-unknown-elf-gcc -march=rv32imc -mabi=ilp32 -msmall-data-limit=0 -falign-functions=4 -O2 -x c -std=c11 -g3 -c Startup.c -o Startup.o
riscv32-unknown-elf-gcc -march=rv32imc -mabi=ilp32 -msmall-data-limit=0 -falign-functions=4 -O2 -x c -std=c11 -g3 -c coprocessor_main.c -o coprocessor_main.o
riscv32-unknown-elf-gcc -march=rv32imc -mabi=ilp32 -msmall-data-limit=0 -falign-functions=4 -O2 -x assembler -std=c11 -g3 -c coprocessor_start.s -o coprocessor_start.o

:: Link
echo +++ Linking:
riscv32-unknown-elf-g++ -nostartfiles -nostdlib -e _start -Wl,--print-memory-usage  -Wl,--print-map -Wl,--no-warn-rwx-segments -Wl,-dT coprocessor.ld --specs=nano.specs --specs=nosys.specs -Wl,-Map=coprocessor.map Startup.o coprocessor_main.o coprocessor_start.o -o coprocessor.elf 

:: Generate
echo +++ Generating:
riscv32-unknown-elf-objcopy coprocessor.elf -O ihex coprocessor.hex
riscv32-unknown-elf-objcopy coprocessor.elf -O binary coprocessor.bin
py bin2asm.py -i coprocessor.bin -o coprocessor_binary.s -s ".coprocessor" -l 16 -g coprocessor_bin

pause