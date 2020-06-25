SRC_URI += "file://bsp.cfg \
            file://vitis_kconfig.cfg \
            file://user_UZ3EG_PCIEC.cfg \
            file://0001-hwmon-pmbus-associate-PMBUS_SKIP_STATUS_CHECK-with-d.patch \
            file://0002-hwmon-pmbus-Fix-page-count-auto-detection.patch \
            file://0003-hwmon-pmbus-Add-Infineon-IRPS5401-driver.patch \
            "
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
