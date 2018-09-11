do_install_append () {
	sed -i -e '/hvc0/d' ${D}${sysconfdir}/inittab
}
