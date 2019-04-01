#
# This file is the factest recipe.
#

SUMMARY = "Simple factest application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	   file://Makefile \
      file://common_util.c \
	   file://test_bt.c \
	   file://test_dp.c \
	   file://test_eeprom.c \
	   file://test_flash.c \
	   file://test_gpio.c \
	   file://test_sd.c \
	   file://test_usb.c \
	   file://test_wifi.c \
	   file://test_zcu100.h \
	   file://typedef.h \
	   file://ultra96v2-production-test.c \
      "

S = "${WORKDIR}"

FILES_${PN} += "/home/root/*"

DEPENDS = "bluez5"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 factest ${D}/home/root
}
