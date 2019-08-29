FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI_append_ultra96-zynqmp = " \
    file://0001-EMBEDDEDSW-PATCH-sw_apps-zynqmp_pmufw-Leave-MIO34-tr.patch \
    file://0001-zynqmp_pmufw-Add-support-for-Ultra96-power-button_v2board.patch \ 
    "

#Add debug for PMUFW
#XSCTH_BUILD_DEBUG = "1"

YAML_COMPILER_FLAGS_append_ultra96-zynqmp = " -DENABLE_MOD_ULTRA96 -DENABLE_SCHEDULER"
