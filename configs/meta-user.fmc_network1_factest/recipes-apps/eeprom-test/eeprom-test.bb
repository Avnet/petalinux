#
# This file is the eeprom-test recipe.
#

SUMMARY = "Simple eeprom-test application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://eeprom-test.sh \
           file://ERASE_EEPROM.TXT \
           file://AT24C02D_28FEB18_EEPROM.TXT \
		  "

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/home/root/
	     install -m 0755 ${S}/eeprom-test.sh ${D}/home/root/
	     install -m 0755 ${S}/ERASE_EEPROM.TXT ${D}/home/root/
	     install -m 0755 ${S}/AT24C02D_28FEB18_EEPROM.TXT ${D}/home/root/
}

FILES_${PN} += "/home/root/eeprom-test.sh \
           /home/root/ERASE_EEPROM.TXT \
           /home/root/AT24C02D_28FEB18_EEPROM.TXT \
               "
