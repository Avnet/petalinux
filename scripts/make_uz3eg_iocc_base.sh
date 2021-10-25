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
#  Please direct any questions to the UltraZed community support forum:
#     http://avnet.me/uzegforum and 
#
#  Product information is available at:
#     http://avnet.me/ultrazed-eg
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
#
#  Create Date:         Aug 10, 2020
#  Design Name:         UltraZed-EG Base BSP
#  Module Name:         make_uz3eg_iocc_base.sh
#  Project Name:        UltraZed-EG Base BSP
#  Target Devices:      Xilinx Zynq UltraScale+ 3EG
#  Hardware Boards:     UltraZed-EG SOM + I/O Carrier
#
# ----------------------------------------------------------------------------

#!/bin/bash

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynqmp_fsbl

HDL_PROJECT_NAME=base
HDL_BOARD_NAME=uz3eg_iocc

ARCH="aarch64"
SOC="zynqMP"

PETALINUX_BOARD_FAMILY=uz

IMAGE_TYPES="initrd-minimal ext4"

source ${MAIN_SCRIPT_FOLDER}/common.sh
make_board
