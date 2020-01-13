#!/bin/sh

#Overwrite the default wpa_supplicant setup with one edited with
#the WLAN SSID and password.
#echo "Copy the wpa_supplicant.conf file to /etc."
#echo "NOTE!  This file needs to be edited with the WLAN SSID and password!"
#cp -f /usr/local/bin/wpa_supplicant.conf /etc/.

#Overwrite the default wpa_supplicant setup with one edited with
#the WLAN SSID and password that was previously copied to the eMMC.
echo "Copy the wpa_supplicant.conf file from eMMC to /etc."
echo "NOTE!  This file must have been previously edited with"
echo "the WLAN SSID and password!"
#Mount the eMMC
echo "Mount the eMMC."
mount /dev/mmcblk1p1 /mnt
#Further, overwrite wpa_supplicant.conf if it exists on eMMC/SD card:
echo "Copy the wpa_supplicant.conf file from eMMC to /etc."
cp -f /mnt/wpa_supplicant.conf /etc/.
#Unmount drive again:
echo "Unmount the eMMC."
umount /mnt

#Run wpa_supplicant for secure networking:
echo "Bringup the WiFi interface."
wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf -B

#To add DHCP:
echo "Fetch an IP address from the DHCP server."
udhcpc -i wlan0

