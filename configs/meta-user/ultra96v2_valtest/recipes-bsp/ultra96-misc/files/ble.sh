# The max # of GPIOs this release of Linux is configured to support is 1024.  The ZYNQ PS GPIO block has 118 IOs (54 on MIO, 64 on EMIO).  
# 1024-118 = 906, hence "gpiochip906".  In our design, we have BT_REG_ON tied to EMIO[0], which is the first GPIO after all of the MIO, or 906 + 54 = 960.

#settings_file=/mnt/emmc/ble/settings.txt
#if [ ! -f $settings_file ]
#then
#  echo "BLE settings.txt file not found."
#  BLUETOOTH_MAC=`cat /sys/class/net/wlan0/address`
#  echo "Using default Bluetooth MAC: $BLUETOOTH_MAC"
#else
#  BLUETOOTH_MAC=$(grep BLUETOOTH_MAC $settings_file | cut -d = -f2)
#  echo "Using Bluetooth MAC from $settings_file : $BLUETOOTH_MAC"
#  #Note that this cd will only work if you run the script as a dot command:#. ble.sh
#  cd /mnt/emmc/ble/
#fi


# Turn echo of commands on:
set -v
echo BT_POWER_UP > /dev/wilc_bt
echo BT_DOWNLOAD_FW > /dev/wilc_bt
echo BT_FW_CHIP_WAKEUP > /dev/wilc_bt

# Set uart0 = 16550 UART in PL = ttyS2 baudrate:
stty -F /dev/ttyS1 115200
stty -F /dev/ttyS1 crtscts
# Initialize the device:
hciattach /dev/ttyS1 -t 10 any 115200 noflow nosleep 
hciconfig hci0 up
set +v
# Above turns echo off

echo BT_FW_CHIP_ALLOW_SLEEP > /dev/wilc_bt

