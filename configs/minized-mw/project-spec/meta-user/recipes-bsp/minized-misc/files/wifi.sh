#!/bin/sh

#Disable wired Ethernet:
ifdown eth0

#Delete the wlan0 file if it already exists
file="/var/run/wpa_supplicant/wlan0"
rm -f $file

#Connect to WiFi network specified in wpa_supplicant.conf file:
wpa_supplicant -D nl80211 -i wlan0 -c /mnt/emmc/wpa_supplicant.conf -B
sleep 3

#Request IP address via DHCP:
udhcpc -i wlan0
