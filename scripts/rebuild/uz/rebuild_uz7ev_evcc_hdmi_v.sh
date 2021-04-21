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
#     http://avnet.me/uzegforum and http://avnet.me/uzevforum
#
#  Product information is available at:
#     http://avnet.me/ultrazed-eg and http://avnet.me/ultrazed-ev
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
#  Create Date:         Jan 20, 2021
#  Design Name:         UltraZed-EV HDMI (+VCU) BSP
#  Module Name:         rebuild_uz7ev_evcc_hdmi_v.sh
#  Project Name:        UltraZed-EV HDMI_V BSP
#  Target Devices:      Xilinx Zynq UltraScale+ 7EV
#  Hardware Boards:     UltraZed-EV SOM + EV Carrier
#
# ----------------------------------------------------------------------------

#!/bin/bash

# Stop the script whenever we had an error (non-zero returning function)
set -e

FSBL_PROJECT_NAME=zynqmp_fsbl

HDL_PROJECT_NAME=hdmi_v
HDL_BOARD_NAME=uz7ev_evcc

PETALINUX_BOARD_FAMILY=uz
PETALINUX_BOARD_NAME=${HDL_BOARD_NAME}
PETALINUX_BUILD_IMAGE=avnet-image-full

source ./rebuild_common.sh

verify_environment

BOOT_METHOD='EXT4'
configure_boot_method
build_bsp

