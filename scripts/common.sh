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
#  Design Name:         Common functions for BSP Generator
#  Module Name:         common.sh
#  Project Name:        Common functions for BSP Generator
#  Target Devices:      Xilinx Zynq Ultrascale
#
#  Tool versions:       Xilinx Vivado 2020.1
#
#  Description:         Common functions to generate PetaLinux BSP
#
#  Dependencies:        None
#
#  Revision:            Aug 03, 2020: 1.00 Initial version
#
# ----------------------------------------------------------------------------

# Required version of the Xilinx Tools
REQUIRED_VER=2020.1

#REPOSITORIES_FOLDER is the top level folder which should contain at least the 'bdf', 'hdl' amd 'petalinux' repositories
REPOSITORIES_FOLDER=$(readlink -f $MAIN_SCRIPT_FOLDER/../..)

HDL_FOLDER=${REPOSITORIES_FOLDER}/hdl
HDL_PROJECTS_FOLDER=${HDL_FOLDER}/Projects
HDL_SCRIPTS_FOLDER=${HDL_FOLDER}/Scripts

PETALINUX_FOLDER=${REPOSITORIES_FOLDER}/petalinux
PETALINUX_APPS_FOLDER=${PETALINUX_FOLDER}/apps
PETALINUX_CONFIGS_FOLDER=${PETALINUX_FOLDER}/configs
PETALINUX_PROJECTS_FOLDER=${PETALINUX_FOLDER}/projects
PETALINUX_SCRIPTS_FOLDER=${PETALINUX_FOLDER}/scripts

META_AVNET_URL="https://github.com/Avnet/meta-avnet.git"
META_AVNET_BRANCH="2020.1"

PAUSE_DELAY=5
BUILD_FROM_TAG="true"
TOOL_VER=$(echo $PETALINUX_VER | sed 's/\./p/g')
TAG_STAMP=$(cat ${PETALINUX_SCRIPTS_FOLDER}/tag_stamp.txt)
TAG_STRING=${TOOL_VER}_${HDL_PROJECT_NAME}_${HDL_BOARD_NAME}_${TAG_STAMP}

verify_repositories ()
{
  echo -e "\nVerifying repositories ...\n"

  if [ ! -d $HDL_FOLDER ]
  then
    echo "ERROR: can't find 'hdl' folder inside '$REPOSITORIES_FOLDER'"
    return 1
  fi

  if [ ! -d $PETALINUX_FOLDER ]
  then
    echo "ERROR: can't find 'petalinux' folder inside '$REPOSITORIES_FOLDER'"
    return 1
  fi
}

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

check_git_tag()
{
  # If TAG_STAMP is exists and if BUILD_FROM_TAG is true
  if [ ${BUILD_FROM_TAG} = "true" ]
  then
    # Verify the hdl repository is checked out with the correct ${TAG_STRING} tag
    cd ${HDL_FOLDER}
    echo -e "\nVerifying the hdl repository is checked out with the correct ${TAG_STRING} tag.\n"
    if [ ${TAG_STRING} = $(git status head | head -n1 | cut -d ' ' -f4) ]
    then
      echo -e "\nReported hdl tag matches ${TAG_STRING}.  Check petalinux tag next...\n"
    else
      echo -e "\nReported hdl tag is $(git status head | head -n1 | cut -d ' ' -f4).\n"
      echo -e "\nThis does not match requested ${TAG_STRING}.  Exiting now.\n"
      exit
    fi
      
    # Verify the petalinux repository is checked out with the correct ${TAG_STRING} tag
    cd ${PETALINUX_FOLDER}
    echo -e "\nVerifying the petalinux repository is checked out with the correct ${TAG_STRING} tag.\n"
    if [ ${TAG_STRING} = $(git status head | head -n1 | cut -d ' ' -f4) ]
    then
      echo -e "\nReported petalinux tag matches ${TAG_STRING}.  Build will continue...\n"
    else
      echo -e "\nReported petalinux tag is $(git status head | head -n1 | cut -d ' ' -f4).\n"
      echo -e "\nThis does not match ${TAG_STRING}.  Exiting now.\n"
      exit
    fi
  fi
}

reset_git_branch()
{
  # If BUILD_FROM_TAG is true
  if [ ${BUILD_FROM_TAG} = "true" ]
  then
    # Change to HDL projects folder.
    cd ${HDL_FOLDER}
    echo -e "\nCheckout git branch ${$PETALINUX_VER} from hdl repository...\n"
    git checkout ${PETALINUX_VER}

    cd ${PETALINUX_FOLDER}
    echo -e "\nCheckout git branch ${$PETALINUX_VER} from petalinux repository...\n"
    git checkout ${$PETALINUX_VER}
  fi
}

build_hw_platform ()
{
  echo -e "\nChecking '${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}' Vivado Project ...\n"

  # Change to HDL projects folder.
  cd ${HDL_PROJECTS_FOLDER}

  # Check to see if the Vivado hardware project has not been built.
  # If it hasn't then build it now.
  # If it has then fall through and build the PetaLinux BSP
  if [ ! -e ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}.xsa ]
  then
    echo -e "\nNo built Vivado HW project ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER} found."
    echo -e "Will build the hardware platform now.\n"

    # DEBUG
    if [ "$DEBUG" == "yes" ];
    then
      echo ""
      read -t 5 -p "Pause here for 5 seconds"
      echo ""
    fi

    # Change to HDL scripts folder.
    cd ${HDL_SCRIPTS_FOLDER}

    vivado -mode batch -source make_${HDL_PROJECT_NAME}.tcl -notrace -tclargs ${HDL_BOARD_NAME} ${HDL_PROJECT_NAME}

  else
    echo -e "\nFound Vivado HW project ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}."
    echo -e "Will build the PetaLinux BSP now.\n"

    # DEBUG
    if [ "$DEBUG" == "yes" ];
    then
      echo ""
      read -t 5 -p "Pause here for 5 seconds"
      echo ""
    fi
  fi
}

configure_cache_path ()
{
  CONF_FILE=${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/project-spec/meta-user/conf/petalinuxbsp.conf
  CACHE_DIR=${PETALINUX_PROJECTS_FOLDER}/cache
  SSTATE_CACHE=sstate_${PETALINUX_VER}/$ARCH/
  DOWNLOAD_CACHE=downloads_${PETALINUX_VER}/

  # Add the following paths to the end of ${CONF_FILE}
  # If the sstate cache has been downloaded and extracted into the PetaLinux
  # install folder this will significantly accelerate the build time
  # For more information see Xilinx AR #71240
  # https://www.xilinx.com/support/answers/71240.html

  echo -e "\nSetting cache (sstate and download) path ($CACHE_DIR) ...\n"

  mkdir -p ${CACHE_DIR}/${SSTATE_CACHE}
  mkdir -p ${CACHE_DIR}/${DOWNLOAD_CACHE}

  echo -e "\nPREMIRRORS_prepend = \"git://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ \n" \
  "ftp://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ \n"                              \
  "http://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ \n"                             \
  "https://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \"\n" >> ${CONF_FILE}

  echo "DL_DIR = \"${CACHE_DIR}/${DOWNLOAD_CACHE}\"" >> ${CONF_FILE}
  echo "SSTATE_DIR = \"${CACHE_DIR}/${SSTATE_CACHE}\"" >> ${CONF_FILE}

  bash ${PETALINUX_CONFIGS_FOLDER}/project/config.cache.sh $ARCH $CACHE_DIR
}

do_not_rm_work ()
{
  # This function will comment the rm_work line in the local.conf
  # This will make the build faster

  echo -e "\nSetting rm_work to false in local.conf ...\n"

  CONF_FILE=${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/build/conf/local.conf
  sed -i 's/\(INHERIT += "rm_work"\)/#\1/' ${CONF_FILE}
}

modify_initramfs_image ()
{
  # This function will modify the INITRAMFS_IMAGE in meta-user/conf/petalinuxbsp.conf
  # This needs to be done manually, because petalinux-config does not expose this option
  # If the BSP should not generate an INITRAMFS or INITRD image, this value has to be removed

  CONF_FILE=${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/project-spec/meta-user/conf/petalinuxbsp.conf

  # remove the line
  sed -i "/INITRAMFS_IMAGE = /d" ${CONF_FILE}

  if [ ${INITRAMFS_IMAGE} ];
  then
      echo -e "\nSetting '${INITRAMFS_IMAGE}' as INITRAMFS_IMAGE in petalinuxbsp.conf ...\n"
      echo -e "INITRAMFS_IMAGE = \"${INITRAMFS_IMAGE}\"" >> ${CONF_FILE}
  fi
}

create_petalinux_project ()
{
  # This function is responsible for creating a PetaLinux project import
  # hardware platform specified in HDL_PROJECT_NAME variable
  #
  PETALINUX_PROJECT_NAME=${PETALINUX_PROJECT_BASE_NAME}_${PLNX_VER}

  echo -e "\nCreating '$PETALINUX_PROJECT_NAME' Petalinux project ...\n"

  # Create the PetaLinux projects folder.
  mkdir -p ${PETALINUX_PROJECTS_FOLDER}

  # Change to PetaLinux projects folder.
  cd ${PETALINUX_PROJECTS_FOLDER}

  # Create the PetaLinux project.
  petalinux-create --type project --template ${SOC} --name ${PETALINUX_PROJECT_NAME} --force

  # Create the hardware definition folder.
  mkdir -p ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform

  # Import the hardware definition files and hardware platform bitstream from
  # implemented system products folder.
  cd ${HDL_PROJECTS_FOLDER}

  echo -e "\nImporting hardware definition ${HDL_BOARD_NAME}.xsa from HDL project folder ...\n"

  cp -f ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}.xsa \
  ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform/.

  echo -e "\nImporting hardware bitstream ${HDL_BOARD_NAME}_wrapper.bit from HDL project folder...\n"

  cp -f ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}.runs/impl_1/${HDL_BOARD_NAME}_wrapper.bit \
  ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform/system_wrapper.bit

  # Change directories to the hardware definition folder for the PetaLinux
  # project, at this point the .hdf file must be located in this folder
  # for the petalinux-config step to be successful.
  cd ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Import the hardware description into the PetaLinux project.
  petalinux-config --silentconfig --get-hw-description=./hw_platform/ -p ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}
}

configure_petalinux_project()
{
  if [ -f ${PETALINUX_CONFIGS_FOLDER}/project/config.board.${PETALINUX_BOARD_NAME}.sh ]
  then
    echo -e "\nPatching PetaLinux project config ...\n"
    bash ${PETALINUX_CONFIGS_FOLDER}/project/config.board.${PETALINUX_BOARD_NAME}.sh
  else
    echo -e "\nWARNING: No board specific PetaLinux project configuration files found, \n"
    echo -e "PetaLinux project config is not touched for this build ...\n"
  fi

  # If tag_stamp.txt file exists and if $BUILD_FROM_TAG is true
  if [ -f ${PETALINUX_SCRIPTS_FOLDER}/tag_stamp.txt ] && [ "${BUILD_FROM_TAG}" = "true" ]
  then
    # Then clone meta-avnet and checkout git tag ${TAG_STRING} and if that fails 
    # (tag_stamp.txt file may be empty or may not match available git tags) clone meta-avnet master branch
    echo -e "\nClone meta-avnet layer and checkout git tag ${TAG_STRING}\n"
    git clone -b ${TAG_STRING} ${META_AVNET_URL} project-spec/meta-avnet 
  else
    # No tag_stamp.txt file found or BUILD_FROM_TAG is set to "false"
    echo -e "\nTag ${TOOL_VER}_${PETALINUX_BOARD_NAME}_${TAG_STAMP} not found.  Cloning ${META_AVNET_BRANCH} branch instead.\n"
    echo -e "\n***WARNING:  This may result in build mismatch!***\n"
    echo -e "\nStop (<ctrl>-c) in the next ${PAUSE_DELAY} seconds if this is not OK. \n"

    read -t ${PAUSE_DELAY} -p "Pause here for ${PAUSE_DELAY} seconds" || :

    echo -e "\nClone meta-avnet layer and checkout ${META_AVNET_BRANCH} branch\n"
    git clone -b ${META_AVNET_BRANCH} ${META_AVNET_URL} project-spec/meta-avnet
  fi

  if [ "$KEEP_CACHE" = "true" ]
  then
    configure_cache_path
  fi

  if [ "$KEEP_WORK" = "true" ]
  then
    do_not_rm_work
  fi

  petalinux-config --silentconfig
}

configure_boot_method ()
{

  # Change PetaLinux project config to change the boot method
  echo -e "\nModifying project config for ${BOOT_METHOD} boot support...\n"

  bash ${PETALINUX_CONFIGS_FOLDER}/project/config.boot_method.${BOOT_METHOD}.sh ${PETALINUX_BOARD_NAME} ${PETALINUX_BOARD_FAMILY}

  petalinux-config --silentconfig

  modify_initramfs_image
}

build_bsp ()
{
  # Build project
  echo -e "\nBuilding project...\n"

  # Sometimes the build fails because of fetch or setscene issues, so we try another time
  petalinux-build -c ${PETALINUX_BUILD_IMAGE} || petalinux-build -c ${PETALINUX_BUILD_IMAGE}

  if [ "$NO_BIT_OPTION" = "yes" ]
  then
    # Create boot image which does not contain the bistream image.
    petalinux-package --boot --fsbl images/linux/${FSBL_PROJECT_NAME}.elf --uboot --force
    cp images/linux/BOOT.BIN BOOT_${BOOT_METHOD}_NO_BIT.BIN
  fi

  # Create boot image which DOES contain the bistream image.
  petalinux-package --boot --fsbl ./images/linux/${FSBL_PROJECT_NAME}.elf --fpga ./images/linux/system.bit --uboot --force
  cp images/linux/BOOT.BIN BOOT_${BOOT_METHOD}${BOOT_SUFFIX}.BIN

  cp images/linux/image.ub image_${BOOT_METHOD}${BOOT_SUFFIX}.ub
  
  cp images/linux/boot.scr boot_${BOOT_METHOD}${BOOT_SUFFIX}.scr

  # save wic images, if any (don't output messages if not found)
  cp images/linux/*.wic . > /dev/null  2>&1 || true
}

generate_loadable_bitstream ()
{
  # Create a temporary Vivado TCL script which take the standard bitstream
  # file format and modify it to allow u-boot to load it into the
  # programmable logic on the Zynq device via PCAP interface.

  echo "write_cfgmem -format bin -interface spix1 -loadbit \"up 0x0 ./images/linux/system.bit\" -force ./images/linux/system.bit.bin" > swap_bits.tcl

  # Launch vivado in batch mode to clean output products from the hardware platform.
  vivado -mode batch -source swap_bits.tcl

  # Copy the bit-swapped bitstream to the PetaLinux project folder
  cp ./images/linux/system.bit.bin .

  # Remove the temporary Vivado script.
  rm -f swap_bits.tcl
}

package_bsp ()
{
  # Build project
  echo -e "\nPackaging project...\n"

  if [ "$NO_BIT_OPTION" = "yes" ]
  then
    generate_loadable_bitstream
  fi

  # Package the bitstream within the PetaLinux pre-built folder.
  petalinux-package --prebuilt --fpga ./images/linux/system.bit --force

  # Rename the pre-built bitstream file to download.bit so that the default
  # format for the petalinux-boot command over jtag will not need the bit file
  # specified explicitly.
  mv -f pre-built/linux/implementation/system.bit \
  pre-built/linux/implementation/download.bit

  # Copy all image.ub to the pre-built images folder.
  cp image_* pre-built/linux/images/

  # Copy all BOOT.BIN to the pre-built images folder.
  cp BOOT_* pre-built/linux/images/

  # Copy all wic images, if any (don't output messages if not found)
  cp *.wic pre-built/linux/images/ > /dev/null  2>&1 || true

  # Copy all boot scripts to the project folder and pre-built images folder.
  if [ -d ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_NAME}/ ] && [ "$(ls -A ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_NAME}/)" ];
  then
    cp ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_NAME}/* .
    cp ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_NAME}/* pre-built/linux/images/
  fi

  # Copy all family boot scripts to the project folder and pre-built images folder.
  if [ $PETALINUX_BOARD_FAMILY ] && [ -d ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_FAMILY}/ ] && [ "$(ls -A ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_FAMILY}/)" ];
  then
    cp ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_FAMILY}/* .
    cp ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_FAMILY}/* pre-built/linux/images/
  fi

  # Change to PetaLinux project folder.
  cd ${PETALINUX_PROJECTS_FOLDER}

  # Package the hardware source into a BSP package output.
  petalinux-package --bsp -p ${PETALINUX_PROJECT_NAME} \
  --hwsource ${HDL_PROJECTS_FOLDER}/${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}/ \
  --output ${PETALINUX_PROJECT_NAME} --force
}
