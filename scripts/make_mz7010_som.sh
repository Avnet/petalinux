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
#     http://www.ultrazed.org/forum
#
#  Product information is available at:
#     http://zedboard.org/product/microzed
#
#  Disclaimer:
#     Avnet, Inc. makes no warranty for the use of this code or design.
#     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
#     any errors, which may appear in this code, nor does it make a commitment
#     to update the information contained herein. Avnet, Inc specifically
#     disclaims any implied warranties of fitness for a particular purpose.
#                      Copyright(c) 2017 Avnet, Inc.
#                              All rights reserved.
#
# ----------------------------------------------------------------------------
#
#  Create Date:         Sept 10, 2020
#  Design Name:         Avnet Microzed PetaLinux BSP Generator
#  Module Name:         make_mz7010_fmcc.sh
#  Project Name:        Avnet Microzed PetaLinux BSP Generator
#  Target Devices:      Xilinx Zynq
#  Hardware Boards:     Microzed mz7010 Eval Board with or without FMCC
#
#  Tool versions:       Xilinx Vivado 2020.1
#
#  Description:         Build Script for Microzed mz7010 PetaLinux BSP HW Platform
#
#  Dependencies:        Common Script 'common.sh'
#
#  Revision:            Sept 10, 2020: 1.00 Initial version
#
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynq_fsbl

HDL_PROJECT_NAME=mz_petalinux
HDL_BOARD_NAME=mz7010_som

ARCH="arm"
SOC="zynq"

PETALINUX_BOARD_FAMILY=mz
PETALINUX_BOARD_NAME=${HDL_BOARD_NAME}
PETALINUX_PROJECT_BASE_NAME=${PETALINUX_BOARD_NAME}
PETALINUX_BUILD_IMAGE=avnet-image-minimal

KEEP_CACHE="true"
KEEP_WORK="false"
DEBUG="no"

#NO_BIT_OPTION can be set to 'yes' to generate a BOOT.BIN without bitstream
NO_BIT_OPTION='yes'

source ${MAIN_SCRIPT_FOLDER}/common.sh

verify_repositories
verify_environment
check_git_tag

build_hw_platform
create_petalinux_project
configure_petalinux_project

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

package_bsp
