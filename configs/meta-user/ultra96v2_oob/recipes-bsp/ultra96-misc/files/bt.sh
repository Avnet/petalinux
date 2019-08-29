#The max # of GPIOs this release of Linux is configured to support is 1024.  The ZYNQ PS GPIO block has 118 IOs (54 on MIO, 64 on EMIO).  
#1024-118 = 906, hence “gpiochip906”.  In our design, we have BT_REG_ON tied to EMIO[0], which is the first GPIO after all of the MIO, or 906 + 54 = 960.

#Turn echo of commands on:
set -v

echo BT_POWER_UP > /dev/wilc_bt
echo BT_DOWNLOAD_FW > /dev/wilc_bt
echo BT_FW_CHIP_WAKEUP > /dev/wilc_bt

stty -F /dev/ttyS1 115200
stty -F /dev/ttyS1 crtscts
# Initialize the device:
hciattach /dev/ttyS1 -t 10 any 115200 noflow nosleep
sleep 2s

#Configure the right BT device:
hciconfig hci0 up

#begin new
sleep 1s
hciconfig hci0 reset
hciconfig hci0 class 0x200404
#for no password:
hciconfig hci0 sspmode 1
hciconfig hci0 piscan
hciconfig hci0 leadv
hciconfig -a
#end new

sleep 1s
#Scan for BT devices:
hcitool scan
set +v
#Above turns echo off

#Scan for BLE devices:
#hcitool lescan

#hciconfig -a
#hcitool dev


