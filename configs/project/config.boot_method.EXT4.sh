#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config

PETALINUX_BOARD_NAME=$1
PETALINUX_BOARD_FAMILY=$2

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_INITRD -u
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_INITRAMFS -u
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_EXT4
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk1p2\""

if [ "$PETALINUX_BOARD_FAMILY" == "uz" ];
then
    # add wic image to program EMMC
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""
fi

if [ "$PETALINUX_BOARD_NAME" == "minized" ];
then
    # add wic image to program EMMC
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "mz" ];
then
    # add wic image to program EMMC
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "pz" ];
then
    # add wic image to program EMMC
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""

    # make the emmc the default boot dev
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk1p2\""
fi
