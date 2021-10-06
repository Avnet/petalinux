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

# VITIS-AI FIX: meta not included in petalinux in 2021.1 version:
# https://forums.xilinx.com/t5/Embedded-Linux/Petalinux-2021-1-packagegroup-petalinux-vitisai-problem/td-p/1257091
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_USER_LAYER_1 -v "\"\${PROOT}/project-spec/meta-vitis-ai\""

if [ "$PETALINUX_BOARD_PROJECT" == "base" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"$BASE_YOCTO_MACHINE\""
elif [ "$PETALINUX_BOARD_PROJECT" == "hdmi" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-hdmi\""
elif [ "$PETALINUX_BOARD_PROJECT" == "hdmi_v" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-hdmi-v\""
elif [ "$PETALINUX_BOARD_PROJECT" == "hdmi_v_n" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-hdmi-v-n\""
elif [ "$PETALINUX_BOARD_PROJECT" == "nvme" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-nvme\""
elif [ "$PETALINUX_BOARD_PROJECT" == "quadcam_h" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-quadcam-h\""
elif [ "$PETALINUX_BOARD_PROJECT" == "quadcam_h_v" ];
then
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"uz7ev-evcc-quadcam-h-v\""
else
    echo "***WARNING: Unknown board_project name ('$PETALINUX_BOARD_PROJECT'): setting YOCTO_MACHINE_NAME to generic '$BASE_YOCTO_MACHINE'***"
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_MACHINE_NAME -v "\"$BASE_YOCTO_MACHINE\""
fi
