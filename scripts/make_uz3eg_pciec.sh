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
#     http://zedboard.org/product/ultrazed
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
#  Create Date:         August 19, 2020
#  Design Name:         Avnet UZ3EG_PCIEC PetaLinux BSP Generator
#  Module Name:         make_uz3eg_pciec.sh
#  Project Name:        Avnet UZ3EG_PCIEC PetaLinux BSP Generator
#  Target Devices:      Xilinx Zynq Ultrascale MPSoC
#  Hardware Boards:     UZ3EG_PCIEC Board
#
#  Tool versions:       Xilinx Vivado 2020.1
#
#  Description:         Build Script for UZ3EG_PCIEC PetaLinux BSP HW Platform
#
#  Dependencies:        Common Script 'common.sh'
#
#  Revision:            Aug 19, 2020: 1.00 Initial version
#
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynqmp_fsbl

HDL_HARDWARE_NAME=uz_petalinux_hw
HDL_PROJECT_NAME=uz_petalinux
HDL_BOARD_NAME=UZ3EG_PCIEC

ARCH="aarch64"
SOC="zynqMP"

PETALINUX_BOARD_FAMILY=uz
PETALINUX_BOARD_NAME=uz3eg_pciec
PETALINUX_PROJECT_BASE_NAME=${PETALINUX_BOARD_NAME}

KEEP_CACHE="true"
KEEP_WORK="false"
DEBUG="no"

#BOOT METHODS: compatible methods are 'EXT4' or 'INITRD'
BOOT_METHODS=('EXT4' 'INITRD')

#NO_BIT_OPTION can be set to 'yes' to generate a BOOT.BIN without bitsteam
NO_BIT_OPTION='yes'

source ${MAIN_SCRIPT_FOLDER}/common.sh

verify_repositories
verify_environment

build_hw_platform
create_petalinux_project

for BOOT_METHOD in ${BOOT_METHODS[@]}; do
   configure_boot_method
   build_bsp
done

package_bsp
