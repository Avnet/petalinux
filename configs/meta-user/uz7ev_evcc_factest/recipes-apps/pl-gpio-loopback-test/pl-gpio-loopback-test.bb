#
# This file is the pl-gpio-loopback-test recipe.
#

SUMMARY = "Simple pl-gpio-loopback-test application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://gpio.c \
	   file://gpio.h \
	   file://linux_pl_gpio_loopback.c \
	   file://platform.h \
	   file://pl_gpio_udriver.c \
	   file://pl_gpio_udriver.h \
	   file://types.h \
       file://Makefile \
		  "

S = "${WORKDIR}"

FILES_${PN} += "/home/root/*"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}/home/root
	     install -m 0755 pl-gpio-loopback-test ${D}/home/root
}
