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
#     http://www.ultrazed.org/product/ultra96
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
#  Create Date:         August 03, 2020
#  Design Name:         Avnet Ultra96v2 dual camera PetaLinux BSP Generator
#  Module Name:         make_ultra96v2_dualcam.sh
#  Project Name:        Avnet Ultra96v2 PetaLinux dual camera BSP Generator
#  Target Devices:      Xilinx Zynq Ultrascale MPSoC
#  Hardware Boards:     Ultra96v2 Eval Board + ON semiconductor camera mezzanine
#
#  Tool versions:       Xilinx Vivado 2020.1
#
#  Description:         Build Script for Ultra96v2 PetaLinux BSP HW Platform with ON semiconductor
#                       camera mezzanine
#
#  Dependencies:        Common Script 'common.sh'
#
#  Revision:            Aug 03, 2020: 1.00 Initial version
#
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynqmp_fsbl

HDL_HARDWARE_NAME=ultra96v2_dualcam_hw
HDL_PROJECT_NAME=ultra96v2_dualcam
HDL_BOARD_NAME=ULTRA96V2

ARCH="aarch64"
SOC="zynqMP"

PETALINUX_BOARD_NAME=ultra96v2_dualcam
PETALINUX_PROJECT_BASE_NAME=${PETALINUX_BOARD_NAME}
PETALINUX_BUILD_IMAGE=ultra96v2-dualcam-image

KEEP_CACHE="true"
DEBUG="no"

#NO_BIT_OPTION can be set to 'yes' to generate a BOOT.BIN without bitsteam
NO_BIT_OPTION='yes'

META_ON_SEMI_URL="https://github.com/Avnet/meta-on-semiconductor.git"
META_ON_SEMI_BRANCH="2020.1"

source ${MAIN_SCRIPT_FOLDER}/common.sh

verify_repositories
verify_environment

build_hw_platform
create_petalinux_project

echo "Fetching meta-on-semi ..."
git clone -b ${META_ON_SEMI_BRANCH} ${META_ON_SEMI_URL} project-spec/meta-on-semiconductor

configure_petalinux_project

BOOT_METHOD='EXT4'
configure_boot_method
build_bsp

package_bsp
