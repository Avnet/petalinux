#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_LOCAL_SSTATE_FEEDS_URL -v "\"\""
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_PRE_MIRROR_URL -v "\"http://petalinux.xilinx.com/sswreleases/rel-v${PETALINUX_VER%%.*}/downloads/\""
