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

settings_file=/mnt/emmc/ble/settings.txt
if [ ! -f $settings_file ]
then
  echo "BLE settings.txt file not found."
else
  echo "BLE settings.txt file found.  Changing directory to the BLE demo location."
  #Note that this cd will only work if you run the script as a dot command:#. ble.sh
  cd /mnt/emmc/ble/
fi

# Turn echo of commands on:
set -v
# Set uart0 = 16550 UART in PL = ttyS2 baudrate:
stty -F /dev/ttyS2 115200
stty -F /dev/ttyS2 crtscts
# Initialize the device:
hciattach /dev/ttyS2 -t 10 bcm43xx 3000000 flow nosleep $BD_ADDR
set +v
# Above turns echo off

