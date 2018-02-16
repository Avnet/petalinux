SRC_URI_append ="\
    file://system-user.dtsi \
    file://axilite.dtsi \
    file://base.dtsi \
    file://zynq-mw-common.dtsi \
    file://zynq-mw-axilite-common.dtsi \
"
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
