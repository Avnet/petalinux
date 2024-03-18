#!/bin/bash

KCONFIG_EDIT="${PETALINUX}/components/yocto/buildtools/sysroots/x86_64-petalinux-linux/usr/bin/kconfig-tweak"
CONFIG_FILE=project-spec/configs/config

PETALINUX_BOARD_PROJECT=$1
PETALINUX_PROJECT_NAME=$2

BASE_YOCTO_MACHINE='u96v2-sbc-base'

# The system Hostname will be the petalinux project name with '-' instead of '_'
PETALINUX_PROJECT_HOSTNAME=$(echo $PETALINUX_PROJECT_NAME | sed 's/\_/-/g')

${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_HOSTNAME "$PETALINUX_PROJECT_HOSTNAME"
${KCONFIG_EDIT} --file ${CONFIG_FILE} --enable CONFIG_SUBSYSTEM_PMUFW_SERIAL_PSU_UART_1_SELECT
${KCONFIG_EDIT} --file ${CONFIG_FILE} --enable CONFIG_SUBSYSTEM_FSBL_SERIAL_PSU_UART_1_SELECT
${KCONFIG_EDIT} --file ${CONFIG_FILE} --enable CONFIG_SUBSYSTEM_ATF_SERIAL_PSU_UART_1_SELECT
${KCONFIG_EDIT} --file ${CONFIG_FILE} --enable CONFIG_SUBSYSTEM_SERIAL_PSU_UART_1_SELECT
${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_USER_LAYER_0 '${PROOT}/project-spec/meta-avnet'

case "$PETALINUX_BOARD_PROJECT" in
    "dualcam")
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_MACHINE_NAME 'u96v2-sbc-dualcam'
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_INCLUDE_MACHINE_NAME 'u96v2-sbc-dualcam';
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_USER_LAYER_1 '${PROOT}/project-spec/meta-on-semiconductor'
        ;;

    "base")
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_MACHINE_NAME "$BASE_YOCTO_MACHINE";
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_INCLUDE_MACHINE_NAME "$BASE_YOCTO_MACHINE";
        ;;

    *)
        echo "***WARNING: Unknown board_project name ('$PETALINUX_BOARD_PROJECT'): setting YOCTO_MACHINE_NAME to generic '$BASE_YOCTO_MACHINE'***"
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_MACHINE_NAME "$BASE_YOCTO_MACHINE";
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_INCLUDE_MACHINE_NAME "$BASE_YOCTO_MACHINE";
        ;;
esac
