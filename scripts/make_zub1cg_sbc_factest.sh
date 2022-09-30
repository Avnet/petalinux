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
#  Please direct any questions to the ZUBoard community support forum:
#     http://avnet.me/zuboard-1cg-forum
#
#  Product information is available at:
#     http://avnet.me/zuboard-1cg
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
#  Create Date:         Mar 11, 2022
#  Design Name:         ZUBoard-1CG Factory Acceptance Test BSP
#  Module Name:         make_zub1cg_sbc_factest.sh
#  Project Name:        ZUBoard-1CG Factory Acceptance Test BSP
#  Target Devices:      Xilinx Zynq UltraScale+ 1CG
#  Hardware Boards:     ZUBoard-1CG Board
#
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynqmp_fsbl

HDL_PROJECT_NAME=factest
HDL_BOARD_NAME=zub1cg_sbc

ARCH="aarch64"
SOC="zynqMP"

PETALINUX_BOARD_FAMILY=zub1cg
PETALINUX_BOARD_NAME=${HDL_BOARD_NAME}
PETALINUX_BOARD_PROJECT=${HDL_PROJECT_NAME}
PETALINUX_PROJECT_ROOT_NAME=${PETALINUX_BOARD_NAME}_${PETALINUX_BOARD_PROJECT}

PETALINUX_BUILD_IMAGE=avnet-image-minimal

KEEP_CACHE="true"
KEEP_WORK="true"
DEBUG="no"

#NO_BIT_OPTION can be set to 'yes' to generate a BOOT.BIN without bitstream
NO_BIT_OPTION='yes'

source ${MAIN_SCRIPT_FOLDER}/common.sh

setup_project

BOOT_METHOD='EXT4'
unset BOOT_SUFFIX
unset INITRAMFS_IMAGE
build_bsp

package_bsp
