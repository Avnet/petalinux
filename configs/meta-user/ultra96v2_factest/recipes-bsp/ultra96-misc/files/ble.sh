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
#  Module Name:         ble.sh
#  Project Name:        Ultra96-V2 Bluetooth LE startup script
#  Target Devices:      Xilinx Zynq UltraScale+ MPSoC
#  Hardware Boards:     Ultra96-V2
# 
#  Tool versions:       Xilinx Vivado 2017.4
# 
#  Description:         Startup Script for Ultra96-V2 Bluetooth LE interface 
# 
#  Dependencies:        
#
#  Revision:            Jul 22, 2017: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/bin/sh
#


#Turn echo of commands off:
set +v

echo BT_POWER_UP > /dev/wilc_bt
echo BT_DOWNLOAD_FW > /dev/wilc_bt
echo BT_FW_CHIP_WAKEUP > /dev/wilc_bt

#Set uart0 = PS UART0 = ttyPS1 baudrate:
stty -F /dev/ttyPS1 115200
stty -F /dev/ttyPS1 crtscts
#Initialize the device:
hciattach /dev/ttyPS1 -t 10 any 115200 noflow nosleep 
sleep 2s

#Configure the right BT device:
hciconfig hci0 up

sleep 1s
hciconfig hci0 reset
hciconfig -a

sleep 1s
#Scan for BT LE devices:
hcitool lescan

#echo BT_FW_CHIP_ALLOW_SLEEP > /dev/wilc_bt

#Turn echo on
set -v

