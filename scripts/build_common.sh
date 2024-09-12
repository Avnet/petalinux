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
#  Create Date:         August 03, 2020
#  Design Name:         Common functions for BSP builds
#  Module Name:         common.sh
#  Project Name:        Common functions for BSP builds
#  Target Devices:      Xilinx Zynq Ultrascale
#  Hardware Boards:     
#
# ----------------------------------------------------------------------------

# Required version of the Xilinx Tools
REQUIRED_VER=2024.1

verify_environment ()
{
  # Check if the Xilinx tools (PETALINUX and XILINX_VIVADO) are sourced
  echo -e "\nChecking Environment (Xilinx tools sourced) ...\n"

  if [ -z $XILINX_VIVADO ]
  then
    echo -e "ERROR: Variable 'XILINX_VIVADO' not set in environment\n" \
             "\nPlease source the Vivado environment with: '$ source <path-to-installed-Vivado>/settings64.sh'\n" \
             "\t(path by default is: /tools/Xilinx/Vivado/<VER>/settings64.sh)\n" \
             "Consult Xilinx UG973 documentation to get more help.\n"
    return 1
  fi

  if [ -z $PETALINUX ] || [ -z $PETALINUX_VER ]
  then
    echo -e "ERROR: Variable 'PETALINUX' or 'PETALINUX_VER' not set in environment\n" \
             "\nPlease source the Vivado environment with: '$ source <path-to-installed-PetaLinux>/settings.sh'\n" \
             "\t(path by default is: /tools/petalinux-<VER>-final/settings.sh)\n" \
             "Consult Xilinx UG1144 documentation to get more help.\n"
    return 1
  fi

  if [ "$REQUIRED_VER" != "$PETALINUX_VER" ]
  then
    echo -e "ERROR: Wrong Petalinux Version (required: '$REQUIRED_VER', got: '$PETALINUX_VER')\n"
    return 1
  fi

  PLNX_VER=$(echo $PETALINUX_VER | sed 's/\./_/g')
}

configure_boot_method ()
{

  # Change PetaLinux project config to change the boot method
  echo -e "\nModifying project config for ${BOOT_METHOD} boot support...\n"

  bash ${PETALINUX_CONFIGS_FOLDER}/project/config.boot_method.${BOOT_METHOD}.sh ${PETALINUX_BOARD_NAME} ${PETALINUX_BOARD_FAMILY} ${INITRAMFS_IMAGE}

  petalinux-config --silentconfig

}

build_bsp ()
{
  configure_boot_method

  # Build project
  echo -e "\nBuilding project...\n"

  if [ $PETALINUX_BUILD_IMAGE ];
  then
    petalinux-build -c ${PETALINUX_BUILD_IMAGE}
  else
    petalinux-build
  fi

  if [ "$NO_BIT_OPTION" = "yes" ]
  then
    # Create boot image which does not contain the bistream image.
    petalinux-package --boot --fsbl images/linux/${FSBL_PROJECT_NAME}.elf --u-boot --force
    cp images/linux/BOOT.BIN BOOT_${BOOT_METHOD}_NO_BIT.BIN
  fi

  # Create boot image which DOES contain the bistream image.
  petalinux-package --boot --fsbl ./images/linux/${FSBL_PROJECT_NAME}.elf --fpga ./images/linux/system.bit --u-boot --force
  cp images/linux/BOOT.BIN BOOT_${BOOT_METHOD}${BOOT_SUFFIX}.BIN

  cp images/linux/image.ub image_${BOOT_METHOD}${BOOT_SUFFIX}.ub
  
  cp images/linux/boot.scr boot_${BOOT_METHOD}${BOOT_SUFFIX}.scr

  # save wic images, if any (don't output messages if not found)
  cp images/linux/*.wic . > /dev/null  2>&1 || true
}
