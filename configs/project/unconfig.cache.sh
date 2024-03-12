#!/bin/bash

KCONFIG_EDIT="${PETALINUX}/components/yocto/buildtools/sysroots/x86_64-petalinux-linux/usr/bin/kconfig-tweak"
CONFIG_FILE=project-spec/configs/config

${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_YOCTO_LOCAL_SSTATE_FEEDS_URL ''
# kconfig-tweak has issues when replacing a value that has ':'
${KCONFIG_EDIT} --file ${CONFIG_FILE} --undefine CONFIG_PRE_MIRROR_URL
${KCONFIG_EDIT} --file ${CONFIG_FILE} --set-str CONFIG_PRE_MIRROR_URL "file://$CACHE_DIR/downloads_${PETALINUX_VER}/"
