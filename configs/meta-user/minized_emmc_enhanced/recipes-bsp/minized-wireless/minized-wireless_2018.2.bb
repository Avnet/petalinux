SUMMARY = "minized-wireless:  Wi-Fi/BT drivers and firmware for the Murata 1DX module"
#SECTION = "PETALINUX/modules"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	git://github.com/murata-wireless/cyw-fmac-fw;protocol=http;branch=battra;destsuffix=cyw-fmac-fw;name=cyw-fmac-fw \
	git://github.com/murata-wireless/cyw-fmac-nvram;protocol=http;branch=battra;destsuffix=cyw-fmac-nvram;name=cyw-fmac-nvram \
	git://github.com/murata-wireless/cyw-bt-patch;protocol=http;branch=morty-battra;destsuffix=cyw-bt-patch;name=cyw-bt-patch \
	git://github.com/murata-wireless/cyw-fmac-utils-imx32;protocol=http;branch=battra;destsuffix=cyw-fmac-utils-imx32;name=cyw-fmac-utils-imx32 \
"

SRCREV_cyw-fmac-fw = "8b818105f91a247b04c87aad40232b76b47b99b0"
SRCREV_cyw-fmac-nvram = "f2ec90e2bbd971477ae6868fe517262643c7aba5"
SRCREV_cyw-bt-patch = "01bb661269a9d4555da18ee42aa8a9f03ddf8ee6"
SRCREV_cyw-fmac-utils-imx32 = "060688dfe76df98751207c8146268ce7fd80b6ab"

DEPENDS = "libnl virtual/kernel"

S = "${WORKDIR}"

KERNEL_VERSION = "${@base_read_file('${STAGING_KERNEL_BUILDDIR}/kernel-abiversion')}"


do_install() {
	install -d ${D}/lib/firmware/brcm
	install -d ${D}/etc/firmware
	install -d ${D}/usr/bin

	install -m 644 ${S}/cyw-fmac-fw/brcmfmac43430-sdio.bin ${D}/lib/firmware/brcm/brcmfmac43430-sdio.bin
	install -m 644 ${S}/cyw-fmac-fw/brcmfmac43430-sdio.1DX.clm_blob ${D}/lib/firmware/brcm/brcmfmac43430-sdio.clm_blob
	install -m 644 ${S}/cyw-fmac-nvram/brcmfmac43430-sdio.1DX.txt ${D}/lib/firmware/brcm/brcmfmac43430-sdio.txt
	install -m 644 ${S}/cyw-bt-patch/CYW43430A1.1DX.hcd ${D}/etc/firmware/BCM43430A1.hcd
	install -m 755 ${S}/cyw-fmac-utils-imx32/wl ${D}/usr/bin/wl
}

PACKAGES =+ "${PN}-mfgtest"

FILES_${PN} = " \
	/lib/firmware/brcm/brcmfmac43430-sdio.bin \
	/lib/firmware/brcm/brcmfmac43430-sdio.clm_blob \
	/lib/firmware/brcm/brcmfmac43430-sdio.txt \
	/etc/firmware/BCM43430A1.hcd \
"

FILES_${PN}-mfgtest = " \
	/usr/bin/wl \
"

