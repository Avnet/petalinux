#!/bin/bash

KCONFIG_EDIT="${PETALINUX}/components/yocto/buildtools/sysroots/x86_64-petalinux-linux/usr/bin/kconfig-tweak"
CONFIG_FILE=project-spec/configs/config

PETALINUX_BOARD_PROJECT=$1
PETALINUX_PROJECT_NAME=$2

BASE_YOCTO_MACHINE='zub1cg-sbc-base'

# The system Hostname will be the petalinux project name with '-' instead of '_'
PETALINUX_PROJECT_HOSTNAME=$(echo $PETALINUX_PROJECT_NAME | sed 's/\_/-/g')

${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_HOSTNAME "$PETALINUX_PROJECT_HOSTNAME"
${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_USER_LAYER_0 '${PROOT}/project-spec/meta-avnet'

${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART0_NAME 'system'
${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-val CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART0_SIZE 0x1C00000

${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART1_NAME 'user'
${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-val CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART1_SIZE 0x1C00000

${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART1_NAME ''

case "$PETALINUX_BOARD_PROJECT" in
    "dualcam")
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_MACHINE_NAME 'zub1cg-sbc-dualcam';
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_INCLUDE_MACHINE_NAME "zub1cg-sbc-dualcam";
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
