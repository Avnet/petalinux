#
# This file is the iic-gpio-expander-test recipe.
#

SUMMARY = "Simple iic-gpio-expander-test application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://iic-gpio-expander-test.sh \
		  "

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 iic-gpio-expander-test.sh ${D}/home/root
}

FILES_${PN} += "/home/root/iic-gpio-expander-test.sh \
               "
