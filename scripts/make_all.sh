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
#     http://zedboard.org/product/picozed
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
#  Create Date:         Oct 1, 2020
#  Design Name:         Avnet Picozed PetaLinux BSP Generator
#  Module Name:         make_pz7030_fmc2.sh
#  Project Name:        Avnet Picozed PetaLinux BSP Generator
#  Target Devices:      Xilinx Zynq
#  Hardware Boards:     Picozed pz7030 Eval Board with FMC2
#
#  Tool versions:       Xilinx Vivado 2020.1
#
#  Description:         Build Script for Picozed pz7030 PetaLinux BSP HW Platform
#
#  Dependencies:        Common Script 'common.sh'
#
#  Revision:            Oct 1, 2020: 1.00 Initial version
#
# ----------------------------------------------------------------------------

#!/bin/bash

./make_minized_sbc.sh
./make_mz7010_som.sh
./make_mz7020_som.sh
./make_pz7010_fmc2.sh
./make_pz7015_fmc2.sh
./make_pz7020_fmc2.sh
./make_pz7030_fmc2.sh
./make_u96v2_sbc.sh
./make_uz3eg_iocc.sh
./make_uz3eg_pciec.sh
./make_uz7ev_evcc.sh
