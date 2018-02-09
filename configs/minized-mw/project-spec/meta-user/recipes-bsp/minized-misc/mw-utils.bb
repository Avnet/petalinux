DESCRIPTION = "Add files that interact with linux u-boot utils"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# Ensure the our fw_env.config file is written last
RDEPENDS_${PN} += "u-boot-fw-utils"

EXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://bootvars.conf \
            file://fw_env.config \
            file://mw_setboot \
            file://_mw_setbootfile \
            file://fw_getbitstream \
            file://fw_getdevicetree \
            file://fw_getrdname \
            file://fw_setbitstream \
            file://fw_setdevicetree \
            file://fw_setrdname \
"

do_install() {
        install -d ${D}${sysconfdir}

	# Place bootvars.conf in /etc (used by MW API scripts)
        install -m 0755 ${WORKDIR}/bootvars.conf ${D}${sysconfdir}/bootvars.conf
       
	 # Replace default /etc/fw_env.config with one customized for MiniZed
	install -m 0755 ${WORKDIR}/fw_env.config ${D}${sysconfdir}/fw_env.config
        
	# Place MathWorks API scripts in /usr/sbin
	install -d ${D}/${sbindir}
        install -m 0755 ${WORKDIR}/mw_setboot ${D}${sbindir}/mw_setboot
        install -m 0755 ${WORKDIR}/_mw_setbootfile ${D}${sbindir}/_mw_setbootfile
        install -m 0755 ${WORKDIR}/fw_getbitstream ${D}${sbindir}/fw_getbitstream 
        install -m 0755 ${WORKDIR}/fw_getdevicetree ${D}${sbindir}/fw_getdevicetree 
        install -m 0755 ${WORKDIR}/fw_getrdname ${D}${sbindir}/fw_getrdname 
        install -m 0755 ${WORKDIR}/fw_setbitstream ${D}${sbindir}/fw_setbitstream 
        install -m 0755 ${WORKDIR}/fw_setdevicetree ${D}${sbindir}/fw_setdevicetree 
        install -m 0755 ${WORKDIR}/fw_setrdname ${D}${sbindir}/fw_setrdname 
}

FILES_${PN} = " \
	${sysconfdir}/bootvars.conf \
	${sysconfdir}/fw_env.config \
	${sbindir}/mw_setboot \
	${sbindir}/_mw_setbootfile \
	${sbindir}/fw_getbitstream \
	${sbindir}/fw_getdevicetree \
	${sbindir}/fw_getrdname \
	${sbindir}/fw_setbitstream \
	${sbindir}/fw_setdevicetree \
	${sbindir}/fw_setrdname \
"
