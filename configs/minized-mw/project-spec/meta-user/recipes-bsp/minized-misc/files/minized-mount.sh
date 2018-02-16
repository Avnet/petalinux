#!/bin/sh

# Add any custom code to be run at startup here

echo Running Minized Startup Script ...
echo
echo Mounting /dev/mmcblk1p1 on /mnt/emmc

mkdir /mnt/emmc
mount /dev/mmcblk1p1 /mnt/emmc

echo
echo Mounting /dev/sda1 on /mnt/usb

mkdir /mnt/usb
mount /dev/sda1 /mnt/usb

echo MiniZed Boot Script DONE!

echo 
echo "Running user script @ /mnt/emmc/init.sh"
/mnt/emmc/init.sh

echo
echo MiniZed Startup Script DONE!
echo
