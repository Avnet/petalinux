#
# This file is the tricube recipe.
#

SUMMARY = "A simple EGL/OpenGLES application that runs on x11 and renders a rotating triangle."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://tricube.cpp \
	file://tricube.h \
    "

S = "${WORKDIR}"

DEPENDS = " \
	libmali-xlnx \
	libx11 \
	libxext \
	libxfixes \
	libxdamage \
	libdrm \
	"

do_compile () {
   ${CXX} ${CFLAGS} ${LDFLAGS} tricube.cpp -lm -lpthread -ldl -lX11 -lxcb -lXau -lXdmcp -ldrm \ 
   -lXfixes -lXext -lXdamage -lexpat -lglapi -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lEGL \ 
   -lGLESv2 -lGL -o tricube -I${libdir} -L${includedir}
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 tricube ${D}${bindir}
}
