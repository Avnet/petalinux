SUMMARY = "wilc-firmware: Firmware binaries for Microchip WILC1000/WILC3000"
SECTION = "PETALINUX/modules"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-3.0;md5=c79ff39f19dfec6d293b95dea7b07891"

SRC_URI = "git://github.com/linux4wilc/firmware;protocol=http;branch=master"

SRCREV = "6f385eabfd8886c8c5faf0cd7929fcbb3e4c2e0f"

S = "${WORKDIR}/git"

do_install() {
	install -d ${D}/lib/firmware

	install -m 644 ${S}/wilc1000_wifi_firmware.bin ${D}/lib/firmware/wilc1000_wifi_firmware.bin
	install -m 644 ${S}/wilc3000_wifi_firmware.bin ${D}/lib/firmware/wilc3000_wifi_firmware.bin
	install -m 644 ${S}/wilc3000_bt_firmware.bin ${D}/lib/firmware/wilc3000_bt_firmware.bin
	install -m 644 ${S}/wilc3000_bt_firmware_no_rtc.bin ${D}/lib/firmware/wilc3000_bt_firmware_no_rtc.bin
}

PACKAGES =+ "${PN}-wilc1000 ${PN}-wilc3000 ${PN}-bt-firmware-no-rtc"

FILES_${PN}-wilc1000 = " \
	/lib/firmware/wilc1000_wifi_firmware.bin \
"

FILES_${PN}-wilc3000 = " \
	/lib/firmware/wilc3000_wifi_firmware.bin \
	/lib/firmware/wilc3000_bt_firmware.bin \
"

FILES_${PN}-bt-firmware-no-rtc = " \
	/lib/firmware/wilc3000_bt_firmware_no_rtc.bin \
"

