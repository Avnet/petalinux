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
#Set uart0 = serial1 = ttyPS1 baudrate:
#(this was for UART0 from PS) stty -F /dev/ttyPS1 115200
#(this was for UART0 from PL) stty -F /dev/ttyS0 115200
stty -F /dev/ttyS2 115200
stty -F /dev/ttyS2 crtscts
# Initialize the device:
hciattach /dev/ttyS2 -t 10 bcm43xx 3000000 flow nosleep $BD_ADDR
sleep 2s

#Configure the right BT device:
hciconfig hci0 up

#The settings below are optional
sleep 1s
hciconfig hci0 reset
hciconfig hci0 class 0x200404
#for no password:
hciconfig hci0 sspmode 1
hciconfig hci0 piscan
hciconfig hci0 leadv
hciconfig -a
#end no password

sleep 1s
#Scan for BT devices:
hcitool scan
set +v
#Above turns echo off

#Scan for BLE devices:
#hcitool lescan

#hciconfig -a
#hcitool dev

