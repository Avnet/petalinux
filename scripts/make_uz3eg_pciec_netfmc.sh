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
#     http://avnet.me/uzegforum
#
#  Product information is available at:
#     http://avnet.me/ultrazed-eg
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
#  Create Date:         Sep 8, 2021
#  Design Name:         UltraZed-EG NETFMC BSP
#  Module Name:         make_uz3eg_pciec_netfmc.sh
#  Project Name:        UltraZed-EG NETFMC BSP
#  Target Devices:      Xilinx Zynq UltraScale+ 3EG
#  Hardware Boards:     UltraZed-EG SOM + PCIe Carrier + Network FMC
#
# ----------------------------------------------------------------------------

#!/bin/bash

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)
HDL_PROJECT_NAME=netfmc
HDL_BOARD_NAME=uz3eg_pciec
IMAGE_TYPES="initrd-minimal"
source ${MAIN_SCRIPT_FOLDER}/conf/uz.sh
source ${MAIN_SCRIPT_FOLDER}/common.sh
make_board
