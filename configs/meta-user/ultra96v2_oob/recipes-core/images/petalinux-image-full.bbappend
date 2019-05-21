#Note: Mention Each package in individual line
#      cascaded representation with line breaks are not valid in this file.
IMAGE_INSTALL_append = " peekpoke"
IMAGE_INSTALL_append = " gpio-demo"
IMAGE_INSTALL_append = " packagegroup-base-extended"
IMAGE_INSTALL_append = " cmake"
IMAGE_INSTALL_append = " lmsensors-sensorsdetect"
IMAGE_INSTALL_append = " python-pyserial"
IMAGE_INSTALL_append = " libftdi"
IMAGE_INSTALL_append = " python3-pip"
IMAGE_INSTALL_append = " iperf3"
IMAGE_INSTALL_append = " packagegroup-petalinux-ultra96-webapp"
IMAGE_INSTALL_append = " packagegroup-petalinux-v4lutils"
IMAGE_INSTALL_append = " packagegroup-petalinux-96boards-sensors"
IMAGE_INSTALL_append = " packagegroup-petalinux-x11"
IMAGE_INSTALL_append = " packagegroup-petalinux-matchbox"
IMAGE_INSTALL_append = " ultra96-ap-setup"


IMAGE_INSTALL_append = " bonnie++"

# hostapd - For Wi-Fi hotspot / Soft AP support
IMAGE_INSTALL_append = " hostapd"
IMAGE_INSTALL_append = " hostap-conf"
IMAGE_INSTALL_append = " hostap-utils"

# Standard C/C++ Libraries
IMAGE_INSTALL_append = " glibc"
#IMAGE_INSTALL_append = " gflags"


# Add this for wireless support:
IMAGE_INSTALL_append = " wpa-supplicant"

# wilc3000 Wi-Fi/BT driver firmware and utilities
IMAGE_INSTALL_append = " wilc"
IMAGE_INSTALL_append = " wilc-firmware-wilc3000"
IMAGE_INSTALL_append = " iw"

IMAGE_INSTALL_append = " ultra96-misc"

# Bluetooth stack
IMAGE_INSTALL_append = " bluez5"
IMAGE_INSTALL_append = " bluez5-obex"
IMAGE_INSTALL_append = " bluez5-doc"