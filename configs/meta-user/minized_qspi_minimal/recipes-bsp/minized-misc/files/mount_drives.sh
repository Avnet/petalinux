#Mount USB
mkdir /tempusb
mount /dev/sda1 /tempusb
ls -l /tempusb

#Mount eMMC/SD card on Pmod
mount /dev/mmcblk1p1 /mnt
ls -l /mnt

echo "*********************************************"
echo "USB drive is on /tempusb and eMMC is on /mnt"
echo "*********************************************"



