APP_PETALINUX_INSTALL_PATH=/tools/petalinux-v2020.1-final
APP_VIVADO_INSTALL_PATH=/tools/Xilinx/Vivado/2020.1
PLNX_VER=$(echo $PETALINUX_VER | sed 's/\./_/g')

FSBL_PROJECT_NAME=zynqmp_fsbl
HDL_HARDWARE_NAME=ultra96v2_oob_hw
HDL_PROJECT_NAME=ultra96v2_oob

START_FOLDER=$(realpath $0 | xargs dirname)
HDL_PROJECTS_FOLDER=${START_FOLDER}/../../hdl/Projects
HDL_SCRIPTS_FOLDER=${START_FOLDER}/../../hdl/Scripts
PETALINUX_APPS_FOLDER=${START_FOLDER}/../../petalinux/apps
PETALINUX_CONFIGS_FOLDER=${START_FOLDER}/../../petalinux/configs
PETALINUX_PROJECTS_FOLDER=${START_FOLDER}/../../petalinux/projects
PETALINUX_SCRIPTS_FOLDER=${START_FOLDER}/../../petalinux/scripts

KEEP_CACHE="true"
ARCH="aarch64"

source_tools_settings ()
{
  # Source the tools settings scripts so that both Vivado and PetaLinux can
  # be used throughout this build script.
  source ${APP_VIVADO_INSTALL_PATH}/settings64.sh
  source ${APP_PETALINUX_INSTALL_PATH}/settings.sh
}

build_hw_platform ()
{
  # Change to HDL projects folder.
  cd ${HDL_PROJECTS_FOLDER}

  # Check to see if the Vivado hardware project has not been built.
  # If it hasn't then build it now.
  # If it has then fall through and build the PetaLinux BSP
  if [ ! -e ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}.xsa ]
  then
    ls -al ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}/
    echo " "
    echo "No built Vivado HW project ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER} found."
    echo "Will build the hardware platform now."
    echo " "

    # DEBUG
    if [ "$DEBUG" == "yes" ];
    then
      echo " "
      read -t 5 -p "Pause here for 5 seconds"
      echo " "
    fi

    # Change to HDL scripts folder.
    cd ${HDL_SCRIPTS_FOLDER}
    # Launch vivado in batch mode to build hardware platforms for the selected target boards.
    # NOTE that at this time, the argv make assist script is responsible for adding board= and project=
    # this script is responsible for putting them in the correct order (board, then project)
    vivado -mode batch -notrace -source make_${HDL_PROJECT_NAME}.tcl -tclargs ${HDL_BOARD_NAME} ${HDL_PROJECT_NAME}
    #vivado -mode batch -source make_${HDL_PROJECT_NAME}.tcl
  else
    echo " "
    echo "Found Vivado HW project ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}."
    echo "Will build the PetaLinux BSP now."
    echo " "

    # DEBUG
    if [ "$DEBUG" == "yes" ];
    then
      echo " "
      read -t 5 -p "Pause here for 5 seconds"
      echo " "
    fi
  fi
}

configure_cache_path ()
{
    CONF_FILE=${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/project-spec/meta-avnet/conf/petalinuxbsp.conf
    CACHE_DIR=${PETALINUX_PROJECTS_FOLDER}/cache/
    SSTATE_CACHE=sstate_${PETALINUX_VER}/$ARCH/
    DOWNLOAD_CACHE=downloads_${PETALINUX_VER}/

    # Add the following paths to the end of ${CONF_FILE}
    # If the sstate cache has been downloaded and extracted into the PetaLinux
    # install folder this will significantly accelerate the build time
    # For more information see Xilinx AR #71240
    # https://www.xilinx.com/support/answers/71240.html
    echo " "
    echo "Setting local sstate cache paths in project config file..."
    echo " "

    mkdir -p ${CACHE_DIR}/${SSTATE_CACHE}
    mkdir -p ${CACHE_DIR}/${DOWNLOAD_CACHE}

    echo " " >> ${CONF_FILE}
    echo "PREMIRRORS_prepend = \"git://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ " >> ${CONF_FILE}
    echo "ftp://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ " >> ${CONF_FILE}
    echo "http://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \\ " >> ${CONF_FILE}
    echo "https://.*/.* file://${CACHE_DIR}/${DOWNLOAD_CACHE} \"" >> ${CONF_FILE}

    echo "DL_DIR = \"${CACHE_DIR}/${DOWNLOAD_CACHE}\"" >> ${CONF_FILE}
    echo "SSTATE_DIR = \"${CACHE_DIR}/${SSTATE_CACHE}\"" >> ${CONF_FILE}

    bash ${PETALINUX_CONFIGS_FOLDER}/project/config.cache.sh $ARCH
}

create_petalinux_bsp ()
{
    # This function is responsible for creating a PetaLinux BSP around the
    # hardware platform specificed in HDL_PROJECT_NAME variable and build
    # the PetaLinux project within the folder specified by the
    # PETALINUX_PROJECT_NAME variable.
    #
    # When complete, the BSP should boot from SD card by default.

    # Check to see if the PetaLinux projects folder even exists because when
    # you clone the source tree from Avnet Github, the projects folder is not
    # part of that tree.
    if [ ! -d ${PETALINUX_PROJECTS_FOLDER} ]; then
    # Create the PetaLinux projects folder.
    mkdir ${PETALINUX_PROJECTS_FOLDER}
    fi

    # Change to PetaLinux projects folder.
    cd ${PETALINUX_PROJECTS_FOLDER}

    # Create the PetaLinux project.
    petalinux-create --type project --template zynqMP --name ${PETALINUX_PROJECT_NAME} --force

    # Create the hardware definition folder.
    mkdir -p ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform

    # Import the hardware definition files and hardware platform bitstream from
    # implemented system products folder.
    cd ${HDL_PROJECTS_FOLDER}

    echo " "
    echo "Importing hardware definition ${HDL_BOARD_NAME}.xsa from HDL project folder ..."
    echo " "

    cp -f ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}.xsa \
    ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform/.

    echo " "
    echo "Importing hardware bitstream ${HDL_BOARD_NAME}_wrapper.bit from HDL project folder..."
    echo " "

    cp -f ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}_${PLNX_VER}/${HDL_BOARD_NAME}.runs/impl_1/${HDL_BOARD_NAME}_wrapper.bit \
    ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform/system_wrapper.bit

    # Change directories to the hardware definition folder for the PetaLinux
    # project, at this point the .hdf file must be located in this folder
    # for the petalinux-config step to be successful.
    cd ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

    # Import the hardware description into the PetaLinux project.
    petalinux-config --silentconfig --get-hw-description=./hw_platform/ -p ${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

    if [ -f ${PETALINUX_CONFIGS_FOLDER}/project/config.${PETALINUX_ROOTFS_NAME}.sh ]
    then
    echo " "
    echo "Patching PetaLinux project config ..."
    echo " "
    bash ${PETALINUX_CONFIGS_FOLDER}/project/config.${PETALINUX_ROOTFS_NAME}.sh
    else
    echo " "
    echo "WARNING: No board specific PetaLinux project configuration files found, "
    echo "PetaLinux project config is not touched for this build ..."
    echo " "
    fi

    git clone -b tnizan/dev git@github.com:Avnet/meta-avnet.git project-spec/meta-avnet

    if [ "$KEEP_CACHE" = "true" ]
    then
        configure_cache_path
    fi

    # Change PetaLinux project config to boot the rootfs from ext4 partition on the SD card.
    echo " "
    echo "Patching project config for SD EXT4 boot support..."
    echo " "

    bash ${PETALINUX_CONFIGS_FOLDER}/project/config.sd_ext4_boot.sh

    petalinux-config --silentconfig

    petalinux-build -c avnet-image-minimal

    # Create boot image which DOES contain the bistream image.
    petalinux-package --boot --fsbl ./images/linux/${FSBL_PROJECT_NAME}.elf --fpga ./images/linux/system.bit --uboot --force

    # Change to PetaLinux scripts folder.
    cd ${PETALINUX_SCRIPTS_FOLDER}
}

main_make_function ()
{
  #
  # Create the hardware platform (if necessary)
  # and build the PetaLinux BSP for the ULTRA96 target.
  #
  HDL_BOARD_NAME=ULTRA96V2
  PETALINUX_PROJECT_NAME=ultra96v2_oob_${PLNX_VER}
  PETALINUX_ROOTFS_NAME=ultra96v2_oob
  build_hw_platform
  create_petalinux_bsp
}

# First source any tools scripts to setup the environment needed to call both
# PetaLinux and Vivado from this make script.
source_tools_settings

# Call the main_make_function declared above to start building everything.
main_make_function
