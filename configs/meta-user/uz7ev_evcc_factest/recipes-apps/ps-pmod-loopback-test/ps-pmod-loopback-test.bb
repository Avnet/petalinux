#
# This file is the ps-pmod-loopback-test recipe.
#

SUMMARY = "Simple ps-pmod-loopback-test application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://linux_ps_pmod_loopback_test.c \
	   file://Makefile \
		  "

S = "${WORKDIR}"

FILES_${PN} += "/home/root/*"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 ps-pmod-loopback-test ${D}/home/root
}
