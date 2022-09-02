#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config

PETALINUX_BOARD_PROJECT=$1
PETALINUX_PROJECT_NAME=$2

BASE_YOCTO_MACHINE='uz7ev-evcc'

# The system Hostname will be the petalinux project name with '-' instead of '_'
PETALINUX_PROJECT_HOSTNAME=$(echo $PETALINUX_PROJECT_NAME | sed 's/\_/-/g')

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_HOSTNAME -v "\"$PETALINUX_PROJECT_HOSTNAME\""
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_USER_LAYER_0 -v "\"\${PROOT}/project-spec/meta-avnet\""

if [ "$PETALINUX_BOARD_PROJECT" == "base" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"$BASE_YOCTO_MACHINE\""
elif [ "$PETALINUX_BOARD_PROJECT" == "hdmi" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-hdmi\""
elif [ "$PETALINUX_BOARD_PROJECT" == "hdmi_v" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-hdmi-v\""
elif [ "$PETALINUX_BOARD_PROJECT" == "nvme" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-nvme\""
else
    echo "***WARNING: Unknown board_project name ('$PETALINUX_BOARD_PROJECT'): setting YOCTO_MACHINE_NAME to generic '$BASE_YOCTO_MACHINE'***"
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"$BASE_YOCTO_MACHINE\""
fi
