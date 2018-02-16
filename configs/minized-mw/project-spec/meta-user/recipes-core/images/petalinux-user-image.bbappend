IMAGE_INSTALL_append = " peekpoke"
IMAGE_INSTALL_append = " gpio-demo"

# Minized specific customizations
IMAGE_INSTALL_append = " minized-misc"

# WiFi support 
IMAGE_INSTALL_append = " minized-firmware"
IMAGE_INSTALL_append = " minized-firmware-wl"
IMAGE_INSTALL_append = " iw"

# API scripts for MathWorks support
IMAGE_INSTALL_append = " mw-utils"

# Boost C++ librares for MathWorks support
IMAGE_INSTALL_append = " boost"

# U-Boot Linux utils for MathWorks support
IMAGE_INSTALL_append = " u-boot-fw-utils"

# Standard C/C++ Libraries
IMAGE_INSTALL_append = " glibc"
