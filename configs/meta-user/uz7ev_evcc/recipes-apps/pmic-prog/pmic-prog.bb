#
# This is the pmic-prog application recipe
#
#

SUMMARY = "pmic-prog application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "i2c-tools"

SRC_URI = "file://i2c-comm.c \
           file://i2c-comm.h \
           file://ir3806x-reg.h \
           file://ir3806x.c \
           file://ir3806x.h \
           file://irps5401-reg.h \
           file://irps5401.c \
           file://irps5401.h \
           file://main.c \
           file://utils.c \
           file://utils.h \
           file://CMakeLists.txt \
          "

S = "${WORKDIR}"

inherit pkgconfig cmake

do_install() {
        install -d ${D}${bindir}
        install -m 0755 ${B}/pmic_prog ${D}${bindir}
}

