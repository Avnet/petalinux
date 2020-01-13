#!/bin/sh -e

### BEGIN INIT INFO
# Provides: WPA cleanup on Ultra96
# Required-Start:
# Required-Stop:
# Default-Start:S
# Default-Stop:0 6
# Short-Description: Deletes wpa files in /var/run/wpa_supplicant on reboot
# Description:
### END INIT INFO

SYS_GPIO_FOLDER=/sys/class/gpio
WIFI_LED=506
BT_LED=505
LED_ON=1
LED_OFF=0


DESC="ultra96-radio-leds.sh will turn the WiFi and Bluetooth LEDs on and off on Ultra96"

start ()
{
   echo -n "Turning Ultra96 WiFi & Bluetooth LEDs ON..."
   cd $SYS_GPIO_FOLDER
   
   if [ ! -d ./gpio$WIFI_LED ] ; 
   then
      # Export the sysfs GPIO if necessary
      echo $WIFI_LED > export
   fi

   if [ ! -d ./gpio$BT_LED ]; 
   then
      # Export the sysfs GPIO if necessary
      echo $BT_LED > export
   fi

   # Set their direction to output
   echo out > gpio$WIFI_LED/direction
   echo out > gpio$BT_LED/direction

   # Turn each of the LEDs on
   echo $LED_ON > gpio$WIFI_LED/value
   echo $LED_ON > gpio$BT_LED/value

   # Release the sysfs GPIOs
   echo $WIFI_LED > unexport
   echo $BT_LED > unexport

	echo "done."
	echo " "
}

stop ()
{
   echo -n "Turning Ultra96 WiFi & Bluetooth LEDs OFF..."
   cd $SYS_GPIO_FOLDER

   if [ ! -d ./gpio$WIFI_LED ] ; 
   then
      # Export the sysfs GPIO if necessary
      echo $WIFI_LED > export
   fi

   if [ ! -d ./gpio$BT_LED ]; 
   then
      # Export the sysfs GPIO if necessary
      echo $BT_LED > export
   fi

   # Set their direction to output
   echo out > gpio$WIFI_LED/direction
   echo out > gpio$BT_LED/direction

   # Turn each of the LEDs off
   echo $LED_OFF > gpio$WIFI_LED/value
   echo $LED_OFF > gpio$BT_LED/value

   # Release the sysfs GPIOs
   echo $WIFI_LED > unexport
   echo $BT_LED > unexport

	echo "done."
	echo " "
}

case "$1" in
	start)
		start;
		;;
	stop)
		stop;
		;;
	*)
		echo "Usage: /etc/init.d/ultra96-radio-leds.sh {start|stop}"
		exit 1
esac

exit 0

