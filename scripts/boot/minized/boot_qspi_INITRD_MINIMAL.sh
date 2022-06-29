# ----------------------------------------------------------------------------
#
#        ** **        **          **  ****      **  **********  ********** ®
#       **   **        **        **   ** **     **  **              **
#      **     **        **      **    **  **    **  **              **
#     **       **        **    **     **   **   **  *********       **
#    **         **        **  **      **    **  **  **              **
#   **           **        ****       **     ** **  **              **
#  **  .........  **        **        **      ****  **********      **
#     ...........
#                                     Reach Further™
#
# ----------------------------------------------------------------------------
#
#  This design is the property of Avnet.  Publication of this
#  design is not authorized without written consent from Avnet.
#
#  Please direct any questions to the MiniZed community support forum:
#     http://avnet.me/minized_forum
# 
#  Product information is available at:
#     http://avnet.me/minized
#
#  Disclaimer:
#     Avnet, Inc. makes no warranty for the use of this code or design.
#     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
#     any errors, which may appear in this code, nor does it make a commitment
#     to update the information contained herein. Avnet, Inc specifically
#     disclaims any implied warranties of fitness for a particular purpose.
#                      Copyright(c) 2021 Avnet, Inc.
#                              All rights reserved.
#
# ----------------------------------------------------------------------------
#!/bin/bash

# This script will generate a BOOT.BIN file and program the qspi
# This BOOT.BIN file will contain uboot, a kernel with INITRD and a boot.scr

# Stop the script whenever we had an error (non-zero returning function)
set -e

petalinux-package --boot --fsbl ./images/linux/zynq_fsbl.elf --fpga ./images/linux/system.bit --uboot --kernel ./image_INITRD_MINIMAL.ub -o BOOT_LINUX_QSPI.BIN --force --boot-device flash --add ./images/linux/avnet-boot/avnet_qspi.scr --offset 0xFC0000 --boot-script none

program_flash -f ./BOOT_LINUX_QSPI.BIN -offset 0 -flash_type qspi-x4-single -fsbl ./images/linux/zynq_fsbl.elf
