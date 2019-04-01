# ----------------------------------------------------------------------------
#
#        ** **        **          **  ****      **  **********  **********
#       **   **        **        **   ** **     **  **              **
#      **     **        **      **    **  **    **  **              **
#     **       **        **    **     **   **   **  *********       **
#    **         **        **  **      **    **  **  **              **
#   **           **        ****       **     ** **  **              **
#  **  .........  **        **        **      ****  **********      **
#     ...........
#                                     Reach Further
#
# ----------------------------------------------------------------------------
# 
#  This design is the property of Avnet.  Publication of this
#  design is not authorized without written consent from Avnet.
# 
#  Please direct any questions to the Ultra96 community support forum:
#     http://avnet.me/zed-forums	
# 
#  Product information is available at:
#     http://avnet.me/ultra96v2	
# 
#  Disclaimer:
#     Avnet, Inc. makes no warranty for the use of this code or design.
#     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
#     any errors, which may appear in this code, nor does it make a commitment
#     to update the information contained herein. Avnet, Inc specifically
#     disclaims any implied warranties of fitness for a particular purpose.
#                      Copyright(c) 2016 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------
# 
#  Create Date:         Jul 22, 2017
#  Design Name:         Ultra96-V2
#  Module Name:         wifi.sh
#  Project Name:        Ultra96-V2 Wi-Fi startup script
#  Target Devices:      Xilinx Zynq UltraScale+ MPSoC
#  Hardware Boards:     Ultra96-V2
# 
#  Tool versions:       Xilinx Vivado 2017.4
# 
#  Description:         Startup Script for Ultra96-V2 Wi-Fi interface 
# 
#  Dependencies:        
#
#  Revision:            Jul 22, 2017: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/bin/sh
#
#Add the wi-fi driver module to the kernel:

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
#iperf3 -s -i 2


