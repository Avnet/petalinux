#!/bin/bash

# This script will generate a u-boot boot script
# This $BOOT_SCR_NAME.scr will get an ip address from dhcp,
#    set the tftp server ip (given by argument to this script), and start booting the board with tftp

# Stop the script whenever we had an error (non-zero returning function)
set -e

BOOT_SCR_NAME="avnet_jtag_tftp_dhcp"

if ! mkimage -V > /dev/null 2>&1
then 
    echo -e "  mkimage not installed \n  try: sudo apt install u-boot-tools\n"
    exit 1
fi

if [ "$#" -ne 1 ];
then
    echo -e "  \"create_dhcp_boot_scr.sh\" requires exactly 1 argument.\n  Usage:  sh create_dhcp_boot_scr.sh TFTP_SERVER_IP"
    exit 1
fi

SERVER_IP=$1
echo -e "  Will use $SERVER_IP as the TFTP server IP"

cat <<EOT > $BOOT_SCR_NAME.scr.txt
################
# This is a boot script for U-Boot
#
################
setenv autostart yes
setenv autoload no
dhcp
setenv serverip $SERVER_IP
tftpboot 0x04000000 image.ub
EOT

mkimage -A arm -T script -C none -n "Ubuntu boot script" -d $BOOT_SCR_NAME.scr.txt $BOOT_SCR_NAME.scr

rm $BOOT_SCR_NAME.scr.txt
