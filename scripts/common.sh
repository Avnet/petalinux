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
#  Please direct any questions to the ZedBoard community support forum:
#     http://avnet.me/zed-forums
#
#  Product information is available at:
#     http://avnet.me/zed-community
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

#REPOSITORIES_FOLDER is the top level folder which should contain at least the 'bdf', 'hdl' amd 'petalinux' repositories
REPOSITORIES_FOLDER=$(readlink -f $MAIN_SCRIPT_FOLDER/../..)

HDL_FOLDER=${REPOSITORIES_FOLDER}/hdl
HDL_PROJECTS_FOLDER=${HDL_FOLDER}/projects
HDL_SCRIPTS_FOLDER=${HDL_FOLDER}/scripts

PETALINUX_FOLDER=${REPOSITORIES_FOLDER}/petalinux
PETALINUX_APPS_FOLDER=${PETALINUX_FOLDER}/apps
PETALINUX_CONFIGS_FOLDER=${PETALINUX_FOLDER}/configs
PETALINUX_PROJECTS_FOLDER=${PETALINUX_FOLDER}/projects
PETALINUX_SCRIPTS_FOLDER=${PETALINUX_FOLDER}/scripts
PETALINUX_DOCS_FOLDER=${PETALINUX_FOLDER}/documentation

META_AVNET_URL="https://github.com/Avnet/meta-avnet.git"
META_AVNET_BRANCH="2024.1"

PAUSE_DELAY=5
BUILD_FROM_TAG="false"
TOOL_VER=$(echo $PETALINUX_VER | sed 's/\./p/g')
TAG_STAMP=$(cat ${PETALINUX_SCRIPTS_FOLDER}/tag_stamp.txt)
TAG_STRING=${TOOL_VER}_${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_${TAG_STAMP}

source $MAIN_SCRIPT_FOLDER/build_common.sh

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
  echo -e "\nChecking '${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_${PLNX_VER}' Vivado Project ...\n"

  # Check to see if the Vivado hardware project has not been built.
  # If it hasn't then build it now.
  # If it has then fall through and build the PetaLinux BSP
  if [ ! -e ${HDL_PROJECTS_FOLDER}/${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}.xsa ]
  then
    echo -e "\nNo built Vivado HW project ${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_${PLNX_VER} found."
    echo -e "Will build the hardware platform now.\n"

    # DEBUG
    if [ "$DEBUG" == "yes" ];
    then
      echo ""
      read -t ${PAUSE_DELAY} -p "Pause here for ${PAUSE_DELAY} seconds"
      echo ""
    fi

    # Change to HDL scripts folder.
    cd ${HDL_SCRIPTS_FOLDER}

    vivado -mode batch -source make_${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}.tcl -notrace -tclargs ${HDL_BOARD_NAME} ${HDL_PROJECT_NAME} -nolog -nojournal

  else
    echo -e "\nFound Vivado HW project ${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_${PLNX_VER}."
    echo -e "Will build the PetaLinux BSP now.\n"

    # DEBUG
    if [ "$DEBUG" == "yes" ];
    then
      echo ""
      read -t ${PAUSE_DELAY} -p "Pause here for ${PAUSE_DELAY} seconds"
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

  echo -e "\nPREMIRRORS:prepend = \"git://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ \n" \
  "ftp://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ \n"                              \
  "http://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ \n"                             \
  "https://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \"\n" >> ${CONF_FILE}

  echo "DL_DIR = \"${CACHE_DIR}/${DOWNLOAD_CACHE}\"" >> ${CONF_FILE}
  echo "SSTATE_DIR = \"${CACHE_DIR}/${SSTATE_CACHE}\"" >> ${CONF_FILE}

  # Set the CONFIG_PRE_MIRROR_URL and CONFIG_YOCTO_LOCAL_SSTATE_FEEDS_URL settings to the local cahce path
  bash ${PETALINUX_CONFIGS_FOLDER}/project/config.cache.sh $ARCH $CACHE_DIR
}

unconfigure_cache_path ()
{
  CONF_FILE=${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/project-spec/meta-user/conf/petalinuxbsp.conf
  CACHE_DIR=${PETALINUX_PROJECTS_FOLDER}/cache

  # Comment out the local cache paths at the end of ${CONF_FILE}
  # to make it easier for users to rebuild this BSP

  echo -e "\nUn-setting cache (sstate and download) path ($CACHE_DIR) ...\n"

  sed -i "/PREMIRRORS/d" ${CONF_FILE}
  sed -i "/ftp:/d" ${CONF_FILE}
  sed -i "/http:/d" ${CONF_FILE}
  sed -i "/https:/d" ${CONF_FILE}

  sed -i 's/\(DL_DIR\)/#\1/'  ${CONF_FILE}
  sed -i 's/\(SSTATE_DIR\)/#\1/'  ${CONF_FILE}
  
  # Reset the CONFIG_PRE_MIRROR_URL and CONFIG_YOCTO_LOCAL_SSTATE_FEEDS_URL settings to their defaults
  bash ${PETALINUX_CONFIGS_FOLDER}/project/unconfig.cache.sh
}

do_not_rm_work ()
{
  # This function will comment the rm_work line in the local.conf
  # This will make the build faster

  echo -e "\nSetting rm_work to false in local.conf ...\n"

  CONF_FILE=${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/build/conf/local.conf
  sed -i 's/\(INHERIT += "rm_work"\)/#\1/' ${CONF_FILE}
}

create_petalinux_project ()
{
  # This function is responsible for creating a PetaLinux project import
  # hardware platform specified in HDL_PROJECT_NAME variable
  #
  PETALINUX_PROJECT_NAME=${PETALINUX_PROJECT_ROOT_NAME}_${PLNX_VER}

  echo -e "\nCreating '$PETALINUX_PROJECT_NAME' Petalinux project ...\n"

  # Create the PetaLinux projects folder.
  mkdir -p ${PETALINUX_PROJECTS_FOLDER}

  # Change to PetaLinux projects folder.
  cd ${PETALINUX_PROJECTS_FOLDER}

  # Create the PetaLinux project.
  petalinux-create project --template ${SOC} -n ${PETALINUX_PROJECT_NAME} --force

  # Create the hardware definition folder.
  mkdir -p ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform

  # Import the hardware definition files and hardware platform bitstream from
  # implemented system products folder.
  cd ${HDL_PROJECTS_FOLDER}

  echo -e "\nImporting hardware definition ${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}.xsa from HDL project folder ...\n"

  cp -f ${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}.xsa \
  ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform/.

  echo -e "\nImporting hardware bitstream ${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_wrapper.bit from HDL project folder...\n"

  cp -f ${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}.runs/impl_1/${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_wrapper.bit \
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
    bash ${PETALINUX_CONFIGS_FOLDER}/project/config.board.${PETALINUX_BOARD_NAME}.sh ${PETALINUX_BOARD_PROJECT} ${PETALINUX_PROJECT_NAME}
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

  if [ "$KEEP_CACHE" = "true" ]
  then
    unconfigure_cache_path
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

  # Copy all family boot instructions to the project folder and pre-built images folder.
  if [ $PETALINUX_BOARD_FAMILY ] && [ -d ${PETALINUX_DOCS_FOLDER}/${PETALINUX_BOARD_FAMILY}/ ] && [ "$(ls -A ${PETALINUX_DOCS_FOLDER}/${PETALINUX_BOARD_FAMILY}/)" ];
  then
    cp ${PETALINUX_DOCS_FOLDER}/${PETALINUX_BOARD_FAMILY}/* .
    cp ${PETALINUX_DOCS_FOLDER}/${PETALINUX_BOARD_FAMILY}/* pre-built/linux/images/
  fi

  # Copy all family boot scripts to the project folder and pre-built images folder.
  if [ $PETALINUX_BOARD_FAMILY ] && [ -d ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_FAMILY}/ ] && [ "$(ls -A ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_FAMILY}/)" ];
  then
    cp ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_FAMILY}/* .
    cp ${PETALINUX_SCRIPTS_FOLDER}/boot/${PETALINUX_BOARD_FAMILY}/* pre-built/linux/images/
  fi

  # Copy rebuild scripts to the project folder and pre-built images folder.
  if [ $PETALINUX_BOARD_FAMILY ];
  then
    # rebuild is just make, but with verify_environment instead of setup_project and no package_bsp
    REBUILD_SCRIPT=rebuild_${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}.sh
    cp ${PETALINUX_SCRIPTS_FOLDER}/make_${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}.sh $REBUILD_SCRIPT
    sed -i 's/setup_project/verify_environment/' $REBUILD_SCRIPT
    sed -i '/package_bsp/d' $REBUILD_SCRIPT
    cp $REBUILD_SCRIPT pre-built/linux/images/.

    # build_common.sh has all the common build code with out the unnecessary project setup code
    cp ${PETALINUX_SCRIPTS_FOLDER}/build_common.sh common.sh
    sed -i 's/${PETALINUX_CONFIGS_FOLDER}\/project/./' common.sh
    cp common.sh pre-built/linux/images/.
  fi

  # Copy all boot method config scripts to the project folder and pre-built images folder.
  if [ -d ${PETALINUX_CONFIGS_FOLDER}/project/ ] && [ "$(ls -A ${PETALINUX_CONFIGS_FOLDER}/project/)" ];
  then
    cp ${PETALINUX_CONFIGS_FOLDER}/project/config.boot_method.EXT4.sh .
    cp ${PETALINUX_CONFIGS_FOLDER}/project/config.boot_method.EXT4.sh pre-built/linux/images/.
    cp ${PETALINUX_CONFIGS_FOLDER}/project/config.boot_method.INITRD.sh .
    cp ${PETALINUX_CONFIGS_FOLDER}/project/config.boot_method.INITRD.sh pre-built/linux/images/.
  fi

  # Change to PetaLinux project folder.
  cd ${PETALINUX_PROJECTS_FOLDER}

  # Package the hardware source into a BSP package output.
  petalinux-package bsp -p ${PETALINUX_PROJECT_NAME} \
  --hwsource ${HDL_PROJECTS_FOLDER}/${HDL_BOARD_NAME}_${HDL_PROJECT_NAME}_${PLNX_VER}/ \
  --output ${PETALINUX_PROJECT_NAME} --force
}

create_petalinux_project_append()
{
    : # provides optional board specific project setup, see make_u96v2_sbc_dualcam.sh
}

setup_project()
{
    verify_repositories
    verify_environment
    check_git_tag

    build_hw_platform
    create_petalinux_project
    create_petalinux_project_append
    configure_petalinux_project
}
