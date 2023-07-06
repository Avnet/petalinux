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
#  Create Date:         May 15, 2023
#  Design Name:         ZUBoard-1CG M.2 BSP
#  Module Name:         make_zub1cg_sbc_m2_wifi.sh
#  Project Name:        ZUBoard-1CG M.2 WiFi BSP
#  Target Devices:      Xilinx Zynq UltraScale+ 1CG
#  Hardware Boards:     ZUBoard-1CG
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynqmp_fsbl

HDL_PROJECT_NAME=m2
HDL_BOARD_NAME=zub1cg_sbc

ARCH="aarch64"
SOC="zynqMP"

PETALINUX_BOARD_FAMILY=zub1cg
PETALINUX_BOARD_NAME=${HDL_BOARD_NAME}
PETALINUX_BOARD_PROJECT=${HDL_PROJECT_NAME}_wifi
PETALINUX_PROJECT_ROOT_NAME=${PETALINUX_BOARD_NAME}_${PETALINUX_BOARD_PROJECT}

# Use the PETALINUX_BUILD_IMAGE variable to build a different yocto image than the default petalinux-image-minimal
#PETALINUX_BUILD_IMAGE=avnet-image-full

KEEP_CACHE="true"
KEEP_WORK="true"
DEBUG="no"

#NO_BIT_OPTION can be set to 'yes' to generate a BOOT.BIN without bitstream
NO_BIT_OPTION='yes'

source ${MAIN_SCRIPT_FOLDER}/common.sh

create_petalinux_project_append()
{
    META_LAIRD_CP_URL="https://github.com/LairdCP/meta-laird-cp.git"
    META_LAIRD_CP_BRANCH="lrd-10.0.0.x"
    echo "Fetching meta-laird-cp ..."
    git clone -b ${META_LAIRD_CP_BRANCH} ${META_LAIRD_CP_URL} project-spec/meta-laird-cp
}

setup_project

BOOT_METHOD='EXT4'
build_bsp

# package_bsp
