#Delete eMMC partition (if it exists) and add a new one of 128MB
echo "fdisk /dev/mmcblk1 (1 partition of 128MB)"
echo -e "d
o
n
p
1
1
+128M
w
" | fdisk /dev/mmcblk1

echo "sleep 1"
sleep 1

#Format eMMC block as a DOS partition
echo "mkdosfs -F 32 /dev/mmcblk1p1"
mkdosfs -F 32 /dev/mmcblk1p1

echo "sleep 1"
sleep 1

#Mount eMMC
echo "/dev/mmcblk1p1 /mnt"
mount /dev/mmcblk1p1 /mnt
echo "ls -l /mnt"
ls -l /mnt

#Copy files from USB = /dev/sda1 (remember to apply auxiliary power!)
#Mount USB
mkdir /tempusb
mount /dev/sda1 /tempusb
ls -l /tempusb

#Copy latest files from USB to eMMC:
echo "cp /tempusb/image.ub /mnt/"
cp /tempusb/image.ub /mnt/
echo "cp /tempusb/wpa_supplicant.conf /mnt/"
cp /tempusb/wpa_supplicant.conf /mnt/
echo "cp /tempusb/smallboot.bin /mnt/"
cp /tempusb/smallboot.bin /mnt/
#Overwrite wpa_supplicant.conf if it exists on eMMC:
cp -rf /mnt/wpa_supplicant.conf /etc
echo "ls -l /mnt"
ls -l /mnt

#To prevent file corruption:
echo "sync"
sync

#program qspi:
echo "cat /proc/mtd"
cat /proc/mtd
echo "flashcp smallboot.bin /dev/mtd0"
flashcp /mnt/smallboot.bin /dev/mtd0

#Unmount drives again:
echo "umount /tempusb"
umount /tempusb
echo "umount /mnt"
umount /mnt

#Add the wi-fi driver module to the kernel:
modprobe bcmdhd
#Run wpa_supplicant for secure networking:
wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf -B
#To disable wired Ethernet:
ifconfig eth0 down
#To add DHCP:
udhcpc -i wlan0

echo "sleep 1"
sleep 1

#Run user I2C and LED test application:
echo "/usr/bin/i2csensor"
/usr/bin/i2csensor

#shut down system:
echo "shutdown -h now"
shutdown -h now

