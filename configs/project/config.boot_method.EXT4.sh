#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config
ROOTFS_CONFIG_FILE=project-spec/configs/rootfs_config

PETALINUX_BOARD_NAME=$1
PETALINUX_BOARD_FAMILY=$2

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_INITRD -u
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_INITRAMFS -u
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_EXT4

# Remove the dropbear package group from the rootfs.  This conflicts with 
# openssh, which is needed for greater throughput
${KCONFIG_EDIT} -c ${ROOTFS_CONFIG_FILE} -o CONFIG_packagegroup-core-ssh-dropbear -u

if [ "$PETALINUX_BOARD_FAMILY" == "ultra96v2" ];
then
    # add wic image to program sd card
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""

    # make the sd card the default boot dev - SD0
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk0p2\""
    
    # set rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.tar.gz\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "uz" ];
then
    # add wic image to program EMMC
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""

    # make the sd card the default boot dev - SD1
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk1p2\""

    # set rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.tar.gz\""
fi

if [ "$PETALINUX_BOARD_NAME" == "minized" ];
then
    # add wic image to program EMMC
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""

    # make the emmc the default boot dev - SD1
    # Minized does not have sd card
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk1p2\""

    # set rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.tar.gz\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "mz" ];
then
    # add wic image to program EMMC
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""

    # make the sd card the default boot dev - SD0
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk0p2\""

    # set rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.tar.gz\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "pz" ];
then
    # add wic image to program EMMC
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"tar.gz wic\""

    # make the sd card the default boot dev - SD0
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk0p2\""

    # set rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.tar.gz\""
fi
