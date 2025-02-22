@echo off

::esptool --chip esp32s3 write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB  0 ../../Output/baremetal_esp32s3_nosdk.bin

start /B openocd -f openocd_esp32s3.cfg

xtensa-esp32s3-elf-gdb -x gdb_esp32s3.cfg ../../Output/baremetal_esp32s3_nosdk.elf
