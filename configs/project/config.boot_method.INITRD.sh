#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config

PETALINUX_BOARD_NAME=$1
PETALINUX_BOARD_FAMILY=$2

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_INITRD

if [ "$PETALINUX_BOARD_FAMILY" == "uz" ];
then
    # restore rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2\""
fi

if [ "$PETALINUX_BOARD_NAME" == "minized" ];
then
    # restore rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2\""
fi
