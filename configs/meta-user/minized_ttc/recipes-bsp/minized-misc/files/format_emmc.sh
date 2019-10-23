#Delete partition and add a new one of 128MB
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

#Format as a DOS partition
echo "mkdosfs -F 32 /dev/mmcblk1p1"
mkdosfs -F 32 /dev/mmcblk1p1

echo "sleep 1"
sleep 1

#Mount eMMC
echo "/dev/mmcblk1p1 /mnt"
mount /dev/mmcblk1p1 /mnt
echo "ls -l /mnt"
ls -l /mnt

#Unmount drive again:
echo "umount /mnt"
umount /mnt

 
