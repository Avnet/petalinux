SUMMARY = "Recipe for  build an external wilc Linux kernel module"
SECTION = "PETALINUX/modules"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-3.0;md5=c79ff39f19dfec6d293b95dea7b07891"

inherit module

SRC_URI =  "git://github.com/linux4wilc/driver;protocol=http;branch=master"

#Hash for untagged v15.0 era driver sources
#SRCREV = "1a0619d1b7489641838471417dcd9453efdca628"

#Hash for tagged v15.0 driver sources
SRCREV = "299678b9e81e3babdb1357261babce1e15f3af12"

SRC_URI += "file://0001-fix-syntax-error-change-virtual-intf.patch;patchdir=${WORKDIR}/git/wilc1000 \
	    file://0002-Makefile-support-for-modules.patch;patchdir=${WORKDIR}/git/wilc1000 \
	    file://0003-set-GPIO-numbers.patch;patchdir=${WORKDIR}/git/wilc1000 \
"

DEPENDS += "virtual/kernel"

S = "${WORKDIR}/git/wilc1000"
#S = "${WORKDIR}"

EXTRA_OEMAKE = 'CONFIG_WILC=m \
		CONFIG_WILC_SDIO=m \
		CONFIG_WILC1000_HW_OOB_INTR=n \
		KERNEL_SRC="${STAGING_KERNEL_DIR}" \
		O=${STAGING_KERNEL_BUILDDIR} \
'

