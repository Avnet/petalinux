FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI_append_ultra96-zynqmp = " \
    file://0001-ultra96-v2-ap-config.patch \
    file://0002-ultra96-v2-udhcpd-config.patch \
    "

