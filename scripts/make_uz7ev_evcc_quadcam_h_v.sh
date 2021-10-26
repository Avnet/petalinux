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
#     http://avnet.me/uzevforum
#
#  Product information is available at:
#     http://avnet.me/ultrazed-ev
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
#  Create Date:         Mar 25, 2021
#  Design Name:         UltraZed-EV QUADCAM (+HDMI +VCU) BSP
#  Module Name:         make_uz7ev_evcc_quadcam_h_v.sh
#  Project Name:        UltraZed-EV QUADCAM_H_V BSP
#  Target Devices:      Xilinx Zynq UltraScale+ 7EV
#  Hardware Boards:     UltraZed-EV SOM + EV Carrier + Quad Camera FMC
#
# ----------------------------------------------------------------------------

#!/bin/bash

# MAIN_SCRIPT_FOLDER is the folder where this current script is
MAIN_SCRIPT_FOLDER=$(realpath $0 | xargs dirname)
HDL_PROJECT_NAME=quadcam_h_v
HDL_BOARD_NAME=uz7ev_evcc
IMAGE_TYPES="ext4"
source ${MAIN_SCRIPT_FOLDER}/conf/uz.sh
source ${MAIN_SCRIPT_FOLDER}/common.sh
make_board
