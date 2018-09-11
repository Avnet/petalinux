#
# This file is the eltt2 recipe.
#

SUMMARY = "Simple eltt2 application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://eltt2.c \
       file://eltt2.h \
	   file://Makefile \
		  "

S = "${WORKDIR}"

FILES_${PN} += "/home/root/*"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}/home/root/
	     install -m 0755 eltt2 ${D}/home/root/
}
