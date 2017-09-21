FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
dirs755 += "/media/card"

do_install_append () {
	echo "/dev/mmcblk1p1 /media/card auto defaults,sync  0  0" >> ${D}${sysconfdir}/fstab
	echo "alias ls='ls --color=auto'" >> ${D}${sysconfdir}/profile
	echo "alias ll='ls --color=auto -l'" >> ${D}${sysconfdir}/profile
	echo "alias la='ls --color=auto -la'" >> ${D}${sysconfdir}/profile
	echo "alias modetest='modetest -M xilinx_drm'" >> ${D}${sysconfdir}/profile
	echo "export PATH=/media/card:/media/card/bin:\$PATH" >> ${D}${sysconfdir}/profile
	echo "export QT_QPA_PLATFORM=eglfs" >> ${D}${sysconfdir}/profile
	echo "export QT_QPA_GENERIC_PLUGINS=libinput" >> ${D}${sysconfdir}/profile
	echo "export QT_QPA_ENABLE_TERMINAL_KEYBOARD=1" >> ${D}${sysconfdir}/profile
	echo "export QT_QPA_EGLFS_INTEGRATION=eglfs_x11" >> ${D}${sysconfdir}/profile
}
