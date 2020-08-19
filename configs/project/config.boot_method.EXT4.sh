#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config

PETALINUX_BOARD_FAMILY=$1

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_EXT4

if [ "$PETALINUX_BOARD_FAMILY" == "uz" ];
then
    # add wic image to program emmc
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""
fi