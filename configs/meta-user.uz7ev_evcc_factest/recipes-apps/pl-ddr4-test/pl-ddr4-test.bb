#
# This file is the pl-ddr4-test recipe.
#

SUMMARY = "Simple pl-ddr4-test application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://linux_plddr4_test.c \
	   file://Makefile \
		  "

S = "${WORKDIR}"

FILES_${PN} += "/home/root/*"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 pl-ddr4-test ${D}/home/root
}
