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
#  Please direct any questions to the PicoZed community support forum:
#     http://avnet.me/picozed_forum
#
#  Product information is available at:
#     http://avnet.me/picozed
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
#  Create Date:         Sep 16, 2020
#  Design Name:         PicoZed Base BSP
#  Module Name:         rebuild_pz7010_fmc2_base.sh
#  Project Name:        PicoZed Base BSP
#  Target Devices:      Xilinx Zynq-7010
#  Hardware Boards:     PicoZed SOM + FMC2 Carrier
#
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

FSBL_PROJECT_NAME=zynq_fsbl

HDL_PROJECT_NAME=base
HDL_BOARD_NAME=pz7010_fmc2

PETALINUX_BOARD_FAMILY=pz
PETALINUX_BOARD_NAME=${HDL_BOARD_NAME}
PETALINUX_BUILD_IMAGE=avnet-image-full

source ./rebuild_common.sh

verify_environment

BOOT_METHOD='INITRD'
BOOT_SUFFIX='_MINIMAL'
INITRAMFS_IMAGE="avnet-image-minimal"
configure_boot_method
build_bsp

BOOT_METHOD='INITRD'
BOOT_SUFFIX='_FULL'
INITRAMFS_IMAGE="avnet-image-full"
configure_boot_method
build_bsp

BOOT_METHOD='EXT4'
unset BOOT_SUFFIX
unset INITRAMFS_IMAGE
configure_boot_method
build_bsp
