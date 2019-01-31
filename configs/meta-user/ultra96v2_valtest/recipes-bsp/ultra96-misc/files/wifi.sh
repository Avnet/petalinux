#Copy the supplied wpa_supplicant.conf file to /etc
#!!!NEED TO EDIT WITH SSID PASSWORD FIRST!!!
cp -f /home/root/wpa_supplicant.conf /etc

# Load ATWILC3000 driver
modprobe wilc
modprobe wilc-sdio

# bring up interface
ifconfig wlan0 up

#Run wpa_supplicant for secure networking:
wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf -B

#To add DHCP:
udhcpc -i wlan0

#To run iperf3:
#server mode:
#comment this to simplify production testing:
#iperf3 -s


