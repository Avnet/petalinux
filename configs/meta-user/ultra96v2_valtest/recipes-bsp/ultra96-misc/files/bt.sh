#Turn echo of commands on:
set -v

echo BT_POWER_UP > /dev/wilc_bt
echo BT_DOWNLOAD_FW > /dev/wilc_bt
echo BT_FW_CHIP_WAKEUP > /dev/wilc_bt

stty -F /dev/ttyPS1 115200
stty -F /dev/ttyPS1 crtscts
# Initialize the device:
hciattach /dev/ttyPS1 -t 10 any 115200 noflow nosleep
sleep 2s

#Configure the right BT device:
hciconfig hci0 up

#begin new
sleep 1s
hciconfig hci0 reset
#Bluetooth COD (class) is TBD.  This is a 6 digit hex number
#EXAMPLE 0x200404
#hciconfig hci0 class 0x<TBD>

#for no password:
hciconfig hci0 sspmode 1
hciconfig hci0 piscan
hciconfig hci0 leadv
hciconfig -a
#end new

sleep 1s
#Scan for BT devices:
hcitool scan
#Turn echo off
set +v

#Scan for BLE devices:
#hcitool lescan

#hciconfig -a
#hcitool dev


