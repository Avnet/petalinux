DESCRIPTION = "Custom / Misc files for MiniZed"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://minized-mount.sh \
	    file://boost-symlinks.sh \
	    file://mwapi-symlinks.sh \
	    file://100-mw-drivers.rules \
	    file://prmpt-color.sh\
	    file://wifi.sh \
" 


do_install() {
	# udev rule creates symlinks for mwipcore driver 
	install -d ${D}/etc/udev/rules.d
	install -m 0755 ${WORKDIR}/100-mw-drivers.rules ${D}/etc/udev/rules.d/100-mw-drivers.rules
	install -d ${D}/etc/rcS.d
	install -m 0755 ${WORKDIR}/mwapi-symlinks.sh ${D}/etc/rcS.d/S97mwapi-symlinks.sh
	install -m 0755 ${WORKDIR}/boost-symlinks.sh ${D}/etc/rcS.d/S98boost-symlinks.sh
	install -m 0755 ${WORKDIR}/minized-mount.sh ${D}/etc/rcS.d/S99minized-mount.sh
	install -d ${D}/etc/profile.d/
	install -m 0755 ${WORKDIR}/prmpt-color.sh ${D}/etc/profile.d
	install -d ${D}/home/root/
	install -m 0755 ${WORKDIR}/wifi.sh ${D}/home/root/
}

FILES_${PN} = " \
    /etc/rcS.d/S97mwapi-symlinks.sh \
    /etc/rcS.d/S98boost-symlinks.sh \
    /etc/rcS.d/S99minized-mount.sh \
    /etc/udev/rules.d/100-mw-drivers.rules \
    /etc/profile.d/prmpt-color.sh \
    /home/root/wifi.sh \
"
