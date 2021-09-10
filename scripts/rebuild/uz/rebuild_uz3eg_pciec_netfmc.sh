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
#     http://avnet.me/uzegforum and http://avnet.me/uzevforum
#
#  Product information is available at:
#     http://avnet.me/ultrazed-eg and http://avnet.me/ultrazed-ev
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
#  Create Date:         Aug 19, 2020
#  Design Name:         UltraZed-EG Base BSP
#  Module Name:         rebuild_uz3eg_pciec_base.sh
#  Project Name:        UltraZed-EG Base BSP
#  Target Devices:      Xilinx Zynq UltraScale+ 3EG
#  Hardware Boards:     UltraZed-EG SOM + PCIe Carrier
#
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

FSBL_PROJECT_NAME=zynqmp_fsbl

HDL_PROJECT_NAME=netfmc
HDL_BOARD_NAME=uz3eg_pciec

PETALINUX_BOARD_FAMILY=uz
PETALINUX_BOARD_NAME=${HDL_BOARD_NAME}
PETALINUX_BUILD_IMAGE=avnet-image-full

source ./rebuild_common.sh

verify_environment

BOOT_METHOD='INITRD'
BOOT_SUFFIX='_MINIMAL'
INITRAMFS_IMAGE="avnet-image-minimal"
configure_boot_method
build_bsp

BOOT_METHOD='EXT4'
unset BOOT_SUFFIX
unset INITRAMFS_IMAGE
configure_boot_method
build_bsp

