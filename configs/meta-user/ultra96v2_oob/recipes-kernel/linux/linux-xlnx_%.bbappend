SRC_URI += "file://bsp.cfg \
            file://vitis_kconfig.cfg \
            file://user_ULTRA96V2.cfg \
            file://fix_u96v2_pwrseq_simple.patch \
            file://0001-hwmon-pmbus-associate-PMBUS_SKIP_STATUS_CHECK-with-d.patch \
            file://0002-hwmon-pmbus-Fix-page-count-auto-detection.patch \
            file://0003-hwmon-pmbus-Add-Infineon-IRPS5401-driver.patch \
            file://0004-hwmon-pmbus-ir38060-Add-driver-for-Infineon-IR38060-.patch \
            "

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
