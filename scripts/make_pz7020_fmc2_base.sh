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
#  Module Name:         make_pz7020_fmc2_base.sh
#  Project Name:        PicoZed Base BSP
#  Target Devices:      Xilinx Zynq-7020
#  Hardware Boards:     PicoZed SOM + FMC2 Carrier
#
# ----------------------------------------------------------------------------

#!/bin/bash

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)

FSBL_PROJECT_NAME=zynq_fsbl

HDL_PROJECT_NAME=base
HDL_BOARD_NAME=pz7020_fmc2

ARCH="arm"
SOC="zynq"

PETALINUX_BOARD_FAMILY=pz

IMAGE_TYPES="initrd-minimal initrd-full ext4"

source ${MAIN_SCRIPT_FOLDER}/common.sh
make_board
