#!/bin/bash

APP_PETALINUX_INSTALL_PATH=/tools/petalinux-v2020.1-final
KCONFIG_EDIT=${APP_PETALINUX_INSTALL_PATH}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_SUBSYSTEM_ROOTFS_EXT4
