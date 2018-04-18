#
# This file is the mac-eeprom-config recipe.
#

SUMMARY = "Simple mac-eeprom-config application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://mac_eeprom_config.sh \
		  "

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 mac_eeprom_config.sh ${D}/home/root
}

FILES_${PN} += "/home/root/mac_eeprom_config.sh \
               "
