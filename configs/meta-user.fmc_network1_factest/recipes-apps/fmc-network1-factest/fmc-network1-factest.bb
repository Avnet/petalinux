#
# This file is the fmc-network1-factest recipe.
#

SUMMARY = "Simple fmc-network1-factest application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://fmc-network1-factory-test.sh \
		  "

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 fmc-network1-factory-test.sh ${D}/home/root
}

FILES_${PN} += "/home/root/fmc-network1-factory-test.sh \
               "
