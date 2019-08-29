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

DESC="ultra96-wpa.sh will deletes wpa files in /var/run/wpa_supplicant on Ultra96"

test -x "/bin/ls" || exit 0
test -x "/bin/rm" || exit 0

start ()
{
   echo "Nothing to start."
	ls -al /var/run/wpa_supplicant
}

stop ()
{
   echo -n "Deleting Ultra96 WPA files..."
	rm -f /var/run/wpa_supplicant/*
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
		echo "Usage: /etc/init.d/ultra96-wpa.sh {start|stop}"
		exit 1
esac

exit 0

