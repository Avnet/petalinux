#!/bin/bash

KCONFIG_EDIT="${PETALINUX}/components/yocto/buildtools/sysroots/x86_64-petalinux-linux/usr/bin/kconfig-tweak"
CONFIG_FILE=project-spec/configs/config
ROOTFS_CONFIG_FILE=project-spec/configs/rootfs_config

PETALINUX_BOARD_NAME=$1
PETALINUX_BOARD_FAMILY=$2

${KCONFIG_EDIT} --file ${CONFIG_FILE} --disable CONFIG_SUBSYSTEM_ROOTFS_INITRD
${KCONFIG_EDIT} --file ${CONFIG_FILE} --disable CONFIG_SUBSYSTEM_ROOTFS_INITRAMFS
${KCONFIG_EDIT} --file ${CONFIG_FILE} --enable CONFIG_SUBSYSTEM_ROOTFS_EXT4

# Remove the dropbear package group from the rootfs.  This conflicts with
# openssh, which is needed for greater throughput
${KCONFIG_EDIT} --file ${ROOTFS_CONFIG_FILE} --disable CONFIG_packagegroup-core-ssh-dropbear

case "$PETALINUX_BOARD_FAMILY"  in

    "u96v2" | "mz" | "pz")
        # make the sd card the default boot dev - SD0
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_SDROOT_DEV '/dev/mmcblk0p2'

        # set rootfs formats
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_RFS_FORMATS 'tar.gz ext4 ext4.gz wic'
        ;;

    "uz" | "zub1cg" )
        # make the sd card the default boot dev - SD1
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_SDROOT_DEV '/dev/mmcblk1p2'

        # set rootfs formats
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_RFS_FORMATS 'tar.gz ext4 ext4.gz wic'
        ;;

esac
