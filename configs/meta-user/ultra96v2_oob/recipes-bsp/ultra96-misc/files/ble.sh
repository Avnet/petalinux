# Turn echo of commands on:
set -v
echo BT_POWER_UP > /dev/wilc_bt
echo BT_DOWNLOAD_FW > /dev/wilc_bt
echo BT_FW_CHIP_WAKEUP > /dev/wilc_bt

# Set uart0 = PS UART0 = ttyPS1 baudrate:
stty -F /dev/ttyPS1 115200
stty -F /dev/ttyPS1 crtscts
# Initialize the device:
hciattach /dev/ttyPS1 -t 10 any 115200 noflow nosleep 
sleep 2s

hciconfig hci0 up

sleep 1s
hciconfig -a
# Turn echo off
set +v

#echo BT_FW_CHIP_ALLOW_SLEEP > /dev/wilc_bt

