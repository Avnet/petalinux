#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config

ARCH=$1
CACHE_DIR=$2

${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_YOCTO_LOCAL_SSTATE_FEEDS_URL -v "\"$CACHE_DIR/sstate_${PETALINUX_VER}/$1/\""
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_PRE_MIRROR_URL -v "\"file://$CACHE_DIR/downloads_${PETALINUX_VER}/\""
