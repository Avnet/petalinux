#!/bin/sh

# Source environment for init script
source /etc/profile

# Set console loglevel to KERN_INFO
echo "Setting console loglevel to 0 ..."
echo "0" > /proc/sys/kernel/printk

# add /media/card to library search path
echo '/media/card' >> /etc/ld.so.conf
/sbin/ldconfig

# Auto-start tricube application if present
tri=$(which tricube)
if [ $? -eq 0 ]; then
	runtricube.sh &
fi
