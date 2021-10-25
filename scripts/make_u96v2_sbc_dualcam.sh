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
#     http://avnet.me/Ultra96_Forum
#
#  Product information is available at:
#     http://avnet.me/ultra96-v2
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
#  Create Date:         Nov 4, 2020
#  Design Name:         Ultra96v2 Dual Camera Mezzanine BSP
#  Module Name:         make_u96v2_sbc_dualcam.sh
#  Project Name:        Ultra96v2 Dual Camera Mezzanine BSP
#  Target Devices:      Xilinx Zynq UltraScale+ 3EG
#  Hardware Boards:     Ultra96v2 Board + Dual Camera Mezzanine
#
# ----------------------------------------------------------------------------

#!/bin/bash

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynqmp_fsbl

HDL_PROJECT_NAME=dualcam
HDL_BOARD_NAME=u96v2_sbc

ARCH="aarch64"
SOC="zynqMP"

PETALINUX_BOARD_FAMILY=u96v2

IMAGE_TYPES="ext4"

source ${MAIN_SCRIPT_FOLDER}/common.sh

post_create_petalinux_project()
{
    META_ON_SEMI_URL="https://github.com/Avnet/meta-on-semiconductor.git"
    META_ON_SEMI_BRANCH="2021.1"
    echo "Fetching meta-on-semi ..."
    git clone -b ${META_ON_SEMI_BRANCH} ${META_ON_SEMI_URL} project-spec/meta-on-semiconductor
}

make_board
