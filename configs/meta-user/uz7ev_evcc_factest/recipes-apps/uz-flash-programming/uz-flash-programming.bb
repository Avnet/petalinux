#
# This file is the uz-flash-programming recipe.
#

SUMMARY = "Simple uz-flash-programming application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://uz-flash-programming.sh \
		  "

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 uz-flash-programming.sh ${D}/home/root
}

FILES_${PN} += "/home/root/uz-flash-programming.sh \
               "
