#!/bin/bash

KCONFIG_EDIT="${PETALINUX}/components/yocto/buildtools/sysroots/x86_64-petalinux-linux/usr/bin/kconfig-tweak"
CONFIG_FILE=project-spec/configs/config
ROOTFS_CONFIG_FILE=project-spec/configs/rootfs_config

PETALINUX_BOARD_NAME=$1
PETALINUX_BOARD_FAMILY=$2
INITRAMFS_IMAGE=$3

${KCONFIG_EDIT} --file ${CONFIG_FILE} --disable CONFIG_SUBSYSTEM_ROOTFS_EXT4
${KCONFIG_EDIT} --file ${CONFIG_FILE} --enable CONFIG_SUBSYSTEM_ROOTFS_INITRD
${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_INITRAMFS_IMAGE_NAME "${INITRAMFS_IMAGE}"

# Remove the dropbear package group from the rootfs.  This conflicts with
# openssh, which is needed for greater throughput
${KCONFIG_EDIT} --file ${ROOTFS_CONFIG_FILE} --disable CONFIG_packagegroup-core-ssh-dropbear

case "$PETALINUX_BOARD_FAMILY"  in

    "mz" | "pz" | "zub1cg" )

        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART0_NAME 'boot'
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-val CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART0_NAME 0x300000

        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART1_NAME 'kernel'
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-val CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART1_SIZE 0xCC0000

        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART2_NAME 'bootenv'
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-val CONFIG_SUBSYSTEM_FLASH_PS7_QSPI_0_BANKLESS_PART0_NAME 0x40000
        ;&

    "uz" )
        # restore rootfs formats
        ${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_SUBSYSTEM_RFS_FORMATS 'cpio cpio.gz cpio.gz.u-boot tar.gz jffs2'
        ;;
esac
