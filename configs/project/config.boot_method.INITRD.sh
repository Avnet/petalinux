#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config
ROOTFS_CONFIG_FILE=project-spec/configs/rootfs_config

PETALINUX_BOARD_NAME=$1
PETALINUX_BOARD_FAMILY=$2
INITRAMFS_IMAGE=$3

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_EXT4 -u
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_INITRD
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_INITRAMFS_IMAGE_NAME -v "\"${INITRAMFS_IMAGE}\""

# Remove the dropbear package group from the rootfs.  This conflicts with 
# openssh, which is needed for greater throughput
${KCONFIG_EDIT} -c ${ROOTFS_CONFIG_FILE} -o CONFIG_packagegroup-core-ssh-dropbear -u

if [ "$PETALINUX_BOARD_FAMILY" == "uz" ];
then
    # restore rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2\""
fi

if [ "$PETALINUX_BOARD_NAME" == "minized_sbc" ] || [ "$PETALINUX_BOARD_FAMILY" == "mz" ] || [ "$PETALINUX_BOARD_FAMILY" == "pz" ] || [ "$PETALINUX_BOARD_FAMILY" == "zub1cg" ]
then
    # restore rootfs formats
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_RFS_FORMATS -v "\"cpio cpio.gz cpio.gz.u-boot tar.gz jffs2\""

    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART0_NAME -v "\"boot\""
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART0_SIZE -v "0x300000"

    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART1_NAME -v "\"kernel\""
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART1_SIZE -v "0xCC0000"

    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART2_NAME -v "\"bootenv\""
    ${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART2_SIZE -v "0x40000"
fi

