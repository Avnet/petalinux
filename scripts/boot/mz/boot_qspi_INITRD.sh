#!/bin/bash

# This script will generate a BOOT.BIN file and program the qspi
# This BOOT.BIN file will contain uboot, a kernel with INITRD and a boot.scr

# Stop the script whenever we had an error (non-zero returning function)
set -e

petalinux-package --boot --fsbl ./images/linux/zynq_fsbl.elf --fpga ./images/linux/system.bit --uboot --kernel ./image_INITRD.ub -o BOOT_LINUX_QSPI.BIN --force --boot-device flash --add ./images/linux/avnet-boot/avnet_qspi.scr --offset 0xFC0000

program_flash -f ./BOOT_LINUX_QSPI.BIN -offset 0 -flash_type qspi-x4-single -fsbl ./images/linux/zynq_fsbl.elf
