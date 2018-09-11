#
# This file is the fmc-network1-factest-init recipe.
#

SUMMARY = "Simple fmc-network1-factest-init application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://fmc-network1-factest-init \
		  "

S = "${WORKDIR}"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit update-rc.d

INITSCRIPT_NAME = "fmc-network1-factest-init"
INITSCRIPT_PARAMS = "start 99 5 ."

do_install() {
    install -d ${D}${sysconfdir}/init.d
    install -m 0755 ${S}/fmc-network1-factest-init ${D}${sysconfdir}/init.d/fmc-network1-factest-init
}

FILES_${PN} += "${sysconfdir}/*"

