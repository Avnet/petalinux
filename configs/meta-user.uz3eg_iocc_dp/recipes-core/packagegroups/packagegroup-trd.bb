DESCRIPTION = "TRD related Packages"
LICENSE = "NONE"

inherit packagegroup distro_features_check
REQUIRED_DISTRO_FEATURES = "x11"
ANY_OF_DISTRO_FEATURES = "x11 fbdev opengl"

X11_PACKAGES = " \
	packagegroup-core-x11-xserver \
	packagegroup-core-x11-utils \
	dbus \
	mini-x-session \
	liberation-fonts \
	xauth \
	xhost \
	xset \
	xtscal \
	xcursor-transparent-theme \
	xinit \
	xinput \
	xinput-calibrator \
	xkbcomp \
	xkeyboard-config \
	xkeyboard-config-locale-en-gb \
	xmodmap \
	xrandr \
	"

QT_PACKAGES = " \
	qtbase-dev \
	qtbase-plugins \
	qtsystems-dev \
	qtconnectivity-dev \
	qtdeclarative-dev \
	qtdeclarative-plugins \
	qtimageformats-dev \
	qtimageformats-plugins \
	qtscript-dev \
	qtsensors-dev \
	qtsensors-plugins \
	qtsvg-dev \
	qtsvg-plugins \
	qttools-dev \
	qttools-tools \
	qtxmlpatterns-dev \
	qtbase-examples \
	qtquick1-dev \
	qtquick1-plugins \
	qtquickcontrols-qmlplugins \
	qttools-plugins \
	qtcharts \
	"

APP_PACKAGES = " \
	glib-2.0 \
	i2c-tools \
	libdrm \
	libdrm-kms \
	libdrm-tests \
	libv4l \
	media-ctl \
	trd-files \
	usbutils \
	v4l-utils \
	yavta \
	"

APP_PACKAGES_DEV = " \
	file \
	gdbserver \
	ldd \
	strace \
	trd-files-dev \
	valgrind \
	"

RDEPENDS_${PN}_append += " \
	packagegroup-petalinux-gstreamer \
	packagegroup-petalinux-opencv \
	packagegroup-petalinux-openamp \
	${X11_PACKAGES} \
	${QT_PACKAGES} \
	${APP_PACKAGES} \
	"

RDEPENDS_${PN}-dev_append += " \
	${APP_PACKAGES_DEV} \
	"
