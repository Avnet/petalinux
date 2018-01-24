#
# This file is the uz7ev-evcc-factest recipe.
#

SUMMARY = "Simple uz7ev-evcc-factest application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://uz7ev-evcc-factory-test.sh \
		  "

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 uz7ev-evcc-factory-test.sh ${D}/home/root
}

FILES_${PN} += "/home/root/uz7ev-evcc-factory-test.sh \
               "
