DESCRIPTION = "Custom / Misc files for MiniZed"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

#FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://pulseaudio-bluetooth.conf \
            file://minized-mount.sh \
	    file://wpa_supplicant.conf \
	    file://wifi.sh \
	    file://bt.sh \
	    file://ble.sh \
	    file://speaker.sh \
	    file://format_emmc.sh \
	    file://delete_emmc.sh \
	    file://update_from_usb.sh \
	    file://mount_drives.sh \
	    file://program_qspi_from_usb.sh \
	    file://onetest.sh \
" 


do_install() {
	install -d ${D}/etc/dbus-1/system.d
	install -m 0644 ${WORKDIR}/pulseaudio-bluetooth.conf ${D}/etc/dbus-1/system.d/pulseaudio-bluetooth.conf
	install -d ${D}/etc/rcS.d
	install -m 0755 ${WORKDIR}/minized-mount.sh ${D}/etc/rcS.d/S99minized-mount.sh
	install -d ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/wpa_supplicant.conf ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/wifi.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/bt.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/ble.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/speaker.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/format_emmc.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/delete_emmc.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/update_from_usb.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/mount_drives.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/program_qspi_from_usb.sh ${D}/usr/local/bin/
	install -m 755 ${WORKDIR}/onetest.sh ${D}/usr/local/bin/
}

FILES_${PN} = " \
    		/etc/dbus-1/system.d/pulseaudio-bluetooth.conf \
    		/etc/rcS.d/S99minized-mount.sh \
		/usr/local/bin/wpa_supplicant.conf \
		/usr/local/bin/wifi.sh \
		/usr/local/bin/bt.sh \
		/usr/local/bin/ble.sh \
		/usr/local/bin/speaker.sh \
		/usr/local/bin/format_emmc.sh \
		/usr/local/bin/delete_emmc.sh \
		/usr/local/bin/update_from_usb.sh \
		/usr/local/bin/mount_drives.sh \
		/usr/local/bin/program_qspi_from_usb.sh \
		/usr/local/bin/onetest.sh \
"
