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

if [ "$PETALINUX_BOARD_NAME" == "u96v2_sbc" ];
then
    # make the sd card the default boot dev - SD0
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk0p2\""
    
    # set rootfs formats (add ext4, ext4.gz, and wic)
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.gz wic\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "uz" ];
then
    # make the sd card the default boot dev - SD1
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk1p2\""

    # set rootfs formats (add ext4, ext4.gz, and wic)
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.gz wic\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "zub1cg" ];
then
    # make the sd card the default boot dev - SD1
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk0p2\""

    # set rootfs formats (add ext4, ext4.gz, and wic)
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.gz wic\""

    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART0_NAME -v "\"system\""
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART0_SIZE -v "0x1C00000"

    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART1_NAME -v "\"user\""
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PSU_QSPI_0_BANKLESS_PART1_SIZE -v "0x400000"
fi

if [ "$PETALINUX_BOARD_NAME" == "minized_sbc" ];
then
    # make the emmc the default boot dev - SD1
    # Minized does not have sd card
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk0p2\""

    # set rootfs formats (add ext4, ext4.gz, and wic)
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.gz wic\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "mz" ];
then
    # make the sd card the default boot dev - SD0
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk0p2\""

    # set rootfs formats (add ext4, ext4.gz, and wic)
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.gz wic\""
fi

if [ "$PETALINUX_BOARD_FAMILY" == "pz" ];
then
    # make the sd card the default boot dev - SD0
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_SDROOT_DEV -v "\"/dev/mmcblk0p2\""

    # set rootfs formats (add ext4, ext4.gz, and wic)
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4 ext4.gz wic\""
fi
