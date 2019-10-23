# Steps for BT speaker Demo
# Note: this uses Bluetooth Classic, not BLE

#Look up the GPIO offsets from /sys/class/gpio/gpiochipN/label fields
get-gpio-offsets
offsets_file=/mnt/emmc/gpio_offsets.txt
MIO0_OFFSET=$(grep MIO0_OFFSET $offsets_file | cut -d = -f2)
echo "Using MIO0_OFFSET from $offsets_file : $MIO0_OFFSET"
EMIO0=$((MIO0_OFFSET + 54))
echo "EMIO0 : $EMIO0"

#To turn on BT_REG_ON, which is on EMIO GPIO #0
echo $EMIO0 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio$EMIO0/direction
echo 1 > /sys/class/gpio/gpio$EMIO0/value
# Get Wi-Fi MAC address and re-use it for bluetooth BDADDR
BD_ADDR=`cat /sys/class/net/wlan0/address`

#Turn echo of commands on:
set -v
# Set uart0 = 16550 UART in PL = ttyS2 baudrate:
stty -F /dev/ttyS2 115200
stty -F /dev/ttyS2 crtscts
# Initialize the device (you should really pick an unique MAC):
hciattach /dev/ttyS2 -t 10 bcm43xx 3000000 flow nosleep $BD_ADDR

#Configure the right BT device:
hciconfig hci0 up
pulseaudio --system -D

bluetoothctl
##scan on
##... copy BTMAC, e.g.  04:52:C7:12:87:36 Bose Mini II SoundLink
##scan off
##pair 04:52:C7:12:87:36
##connect 04:52:C7:12:87:36
##quit

#Now copy your .mp3 file to the MiniZed emmc at /mnt/emmc/audio
#Play your file with:
##mpg123 /mnt/emmc/audio/mysong.mp3
set +v
#Above turns echo off
