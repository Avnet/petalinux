#Copy from USB = /dev/sda1 (remember to apply auxiliary power!)

#Mount USB
mkdir /tempusb
mount /dev/sda1 /tempusb
ls -l /tempusb

#Mount eMMC/SD card on Pmod
mount /dev/mmcblk1p1 /mnt

#Copy latest files from USB to eMMC/SD card:
echo "cp /tempusb/image.ub /mnt/"
cp /tempusb/image.ub /mnt/
echo "cp /tempusb/wpa_supplicant.conf /mnt/"
cp /tempusb/wpa_supplicant.conf /mnt/
echo "ls -l /mnt"
ls -l /mnt

#To prevent file corruption:
echo "sync"
sync

#Unmount drives again:
echo "umount /tempusb"
umount /tempusb
echo "umount /mnt"
umount /mnt



