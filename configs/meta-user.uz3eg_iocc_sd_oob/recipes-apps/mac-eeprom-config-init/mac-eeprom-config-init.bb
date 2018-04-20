#
# This file is the mac-eeprom-config-init recipe.
#

SUMMARY = "Simple mac-eeprom-config-init application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://mac-eeprom-config-init \
		  "

S = "${WORKDIR}"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit update-rc.d

INITSCRIPT_NAME = "mac-eeprom-config-init"
# Run the script at the very beginning of run level 5.  
# Right before the networking is initialized
INITSCRIPT_PARAMS = "start 00 5 ."


do_install() {
    install -d ${D}${sysconfdir}/init.d
    install -m 0755 ${S}/mac-eeprom-config-init ${D}${sysconfdir}/init.d/mac-eeprom-config-init
}

FILES_${PN} += "${sysconfdir}/*"
