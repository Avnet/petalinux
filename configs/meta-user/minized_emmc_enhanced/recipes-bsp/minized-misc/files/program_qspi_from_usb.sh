#Copy from USB = /dev/sda1 (remember to apply auxiliary power!)

#Mount USB
mkdir /tempusb
mount /dev/sda1 /tempusb
ls -l /tempusb

#Mount eMMC/SD card on Pmod
mount /dev/mmcblk1p1 /mnt

#Copy latest .bin file from USB to eMMC/SD card:
echo "cp /tempusb/smallboot.bin /mnt/"
cp /tempusb/smallboot.bin /mnt/
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



