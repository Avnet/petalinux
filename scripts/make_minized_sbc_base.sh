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
# 
#  Create Date:         Sep 10, 2020
#  Design Name:         MiniZed Base BSP
#  Module Name:         make_minized_sbc_base.sh
#  Project Name:        MiniZed Base BSP
#  Target Devices:      Xilinx Zynq-7007
#  Hardware Boards:     MiniZed
# 
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynq_fsbl

HDL_PROJECT_NAME=base
HDL_BOARD_NAME=minized_sbc

ARCH="arm"
SOC="zynq"

PETALINUX_BOARD_FAMILY=minized
PETALINUX_BOARD_NAME=${HDL_BOARD_NAME}
PETALINUX_BOARD_PROJECT=${HDL_PROJECT_NAME}
PETALINUX_PROJECT_ROOT_NAME=${PETALINUX_BOARD_NAME}_${PETALINUX_BOARD_PROJECT}

PETALINUX_BUILD_IMAGE=avnet-image-full

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