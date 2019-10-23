#Delete partition
echo -e "d
q
" | fdisk /dev/mmcblk1

#Mount eMMC
mount /dev/mmcblk1p1 /mnt
ls -l /mnt
