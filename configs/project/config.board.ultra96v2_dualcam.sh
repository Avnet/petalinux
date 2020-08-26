#!/bin/bash

KCONFIG_EDIT=${PETALINUX}/tools/common/petalinux/utils/petalinux-kconfig-edit
CONFIG_FILE=project-spec/configs/config
BOARD_CONFIGS_PATH=$(realpath $0 | xargs dirname)

bash ${BOARD_CONFIGS_PATH}/config.board.ultra96v2.sh
${KCONFIG_EDIT} -c ${CONFIG_FILE} -o CONFIG_USER_LAYER_1 -v "\"\${PROOT}/project-spec/meta-on-semiconductor\""
