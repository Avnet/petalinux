SUMMARY = "minized-firmware:  Firmware binaries for adding Wi-Fi/BT to MiniZed"
SECTION = "PETALINUX/modules"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"

#FILESEXTRAPATHS_prepend := "${THISDIR}/minized-firmware:"

SRC_URI = "file://minized-firmware_${PV}.tar.gz"

# This is needed for the wl_fmac_imx utility
DEPENDS = "libnl"

S = "${WORKDIR}"

do_install() {
	install -d ${D}/lib/firmware/brcm
	install -d ${D}/etc/firmware
	install -d ${D}/usr/bin
	install -m 644 ${WORKDIR}/brcmfmac43430-sdio.bin ${D}/lib/firmware/brcm/brcmfmac43430-sdio.bin
	install -m 644 ${WORKDIR}/bcmdhd.1DX.SDIO.cal ${D}/lib/firmware/brcm/brcmfmac43430-sdio.txt
	install -m 644 ${WORKDIR}/BCM43430A1.1DX.hcd ${D}/etc/firmware     
	install -m 755 ${WORKDIR}/wl_fmac_imx ${D}/usr/bin/wl
}

PACKAGES =+ "${PN}-wl"

FILES_${PN} = " \
	/lib/firmware/brcm/brcmfmac43430-sdio.bin \
	/lib/firmware/brcm/brcmfmac43430-sdio.txt \
	/etc/firmware/BCM43430A1.1DX.hcd \	
"

FILES_${PN}-wl = " \
	/usr/bin/wl \
"

