#
# This file is the ultra96-radio-leds recipe.
#

SUMMARY = "Simple ultra96-radio-leds application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://ultra96-radio-leds.sh \
	"

S = "${WORKDIR}"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit update-rc.d

INITSCRIPT_NAME = "ultra96-radio-leds.sh"
INITSCRIPT_PARAMS = "start 99 S . stop 90 6 . stop 90 0 ."

do_install() {
    install -d ${D}${sysconfdir}/init.d
    install -m 0755 ${S}/ultra96-radio-leds.sh ${D}${sysconfdir}/init.d/ultra96-radio-leds.sh
}

FILES_${PN} += "${sysconfdir}/*"