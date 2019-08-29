SUMMARY = "wilc-firmware: Firmware binaries for Microchip WILC1000/WILC3000"
SECTION = "PETALINUX/modules"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-3.0;md5=c79ff39f19dfec6d293b95dea7b07891"

SRC_URI = "git://github.com/linux4wilc/firmware;protocol=http;branch=master"

SRCREV = "37b910bf91333192b2f7a17530d28155e277173b"

S = "${WORKDIR}/git"

do_install() {
	install -d ${D}/lib/firmware/mchp

	install -m 644 ${S}/wilc1000_wifi_firmware.bin ${D}/lib/firmware/mchp
	install -m 644 ${S}/wilc3000_wifi_firmware.bin ${D}/lib/firmware/mchp
	install -m 644 ${S}/wilc3000_ble_firmware.bin ${D}/lib/firmware/mchp
}

PACKAGES =+ "${PN}-wilc1000 ${PN}-wilc3000"

FILES_${PN}-wilc1000 = " \
	/lib/firmware/mchp/wilc1000_wifi_firmware.bin \
"

FILES_${PN}-wilc3000 = " \
	/lib/firmware/mchp/wilc3000_wifi_firmware.bin \
	/lib/firmware/mchp/wilc3000_ble_firmware.bin \
"


