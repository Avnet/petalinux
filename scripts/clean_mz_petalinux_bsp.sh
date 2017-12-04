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
#     http://www.ultrazed.org/product/ultrazed-EG
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
#  Create Date:         November 03, 2017
#  Design Name:         Avnet MicroZed SOM PetaLinux BSP Generator
#  Module Name:         clean_mz_petalinux_bsp.tcl
#  Project Name:        Avnet MicroZed SOM  PetaLinux BSP Generator
#  Target Devices:      Xilinx Zynq-7000 
#  Hardware Boards:     Avnet MicroZed SOM and supported development
#                       carriers such as the FMC Carrier (FMCCC) and the
#                       I/O Carrier (IOCCC)
# 
#  Tool versions:       Xilinx Vivado 2017.2
# 
#  Description:         Clean Script for MicroZed PetaLinux BSPs and related 
#                       HW Platforms
# 
#  Dependencies:        None
#
#  Revision:            Nov 03, 2017: 1.00 Initial version
# 
# ----------------------------------------------------------------------------

#!/bin/bash

# Set global variables here.
HDL_HARDWARE_NAME=mz_petalinux_hw
HDL_PROJECT_NAME=mz_petalinux
HDL_PROJECTS_FOLDER=../../hdl/Projects
HDL_SCRIPTS_FOLDER=../../hdl/Scripts
PETALINUX_APPS_FOLDER=../../petalinux/apps
PETALINUX_PROJECTS_FOLDER=../../petalinux/projects
PETALINUX_SCRIPTS_FOLDER=../../petalinux/scripts
START_FOLDER=`pwd`

clean_petalinux_bsp ()
{ 
  # Remove the hardware platform project.
  rm -rf ${START_FOLDER}/${HDL_PROJECTS_FOLDER}/${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}

  # Remove the PetaLinux project.
  rm -rf ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Remove the PetaLinux BSP output.
  rm -rf ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}.bsp

  # Change to PetaLinux scripts folder
  cd ${START_FOLDER}/${PETALINUX_SCRIPTS_FOLDER}
}

main_clean_function ()
{
  # This function is responsible for cleaning all of the hardware
  # platforms followed by removal of all PetaLinux project associated with
  # the targets.

  #
  # Remove project files created for PetaLinux BSP on the MZ7010_FMCCC target.
  #
  HDL_BOARD_NAME=MZ7010_FMCCC
  PETALINUX_PROJECT_NAME=mz7020_fmccc_2017_2
  clean_petalinux_bsp

  #
  # Remove project files created for PetaLinux BSP on the MZ7020_FMCCC target.
  #
  HDL_BOARD_NAME=MZ7020_FMCCC
  PETALINUX_PROJECT_NAME=mz7020_fmccc_2017_2
  clean_petalinux_bsp
}

# Call the main_clean_function declared above to start cleaning up all 
# output products.
main_clean_function

