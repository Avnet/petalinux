# ----------------------------------------------------------------------------
#       _____
#      *     *
#     *____   *____
#    * *===*   *==*
#   *___*===*___**  AVNET
#        *======*
#         *====*
# ----------------------------------------------------------------------------
# 
#  This design is the property of Avnet.  Publication of this
#  design is not authorized without written consent from Avnet.
# 
#  Please direct any questions to the UltraZed community support forum:
#     http://www.ultrazed.org/forum
# 
#  Product information is available at:
#     http://www.ultrazed.org/product/ultrazed-EG
# 
#  Disclaimer:
#     Avnet, Inc. makes no warranty for the use of this code or design.
#     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
#     any errors, which may appear in this code, nor does it make a commitment
#     to update the information contained herein. Avnet, Inc specifically
#     disclaims any implied warranties of fitness for a particular purpose.
#                      Copyright(c) 2017 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------
# 
#  Create Date:         Jan 12, 2017
#  Design Name:         UltraZedFlash Programming
#  Module Name:         uz-flash-programming.sh
#  Project Name:        UltraZed Flash Programming Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed-EG + I/O Carrier
#                       UltraZed-EV + EV Carrier
# 
#  Tool versions:       Xilinx Vivado & PetaLinux 2016.2
#                       Xilinx Vivado & PetaLinux 2017.2
# 
#  Description:         Startup Script for UltraZed In System Flash 
#                       Programming
# 
#  Dependencies:        
#
#  Revision:            Jan 12, 2017: 1.0 Initial version
#                       Dec 20, 2017: 1.1 Updated for UltraZed-EV
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# Sleep interval between programming operations.
SLEEP_INTERVAL_NORMAL=1

# Names and locations of files to be programmed into QSPI Flash and eMMC
# These folders are the path of where the microSD card is mounted
CUSTOM_IMAGE_FOLDER=/mnt/sd
CUSTOM_BOOT_IMAGE=BOOT_EMMC_CUSTOM.bin
CUSTOM_LINUX_IMAGE=image_custom.ub

# Variables used to determine whether Flash programming will be carried out 
# or not.  This is determined by the presense of CUSTOM_BOOT_IMAGE and
# CUSTOM_LINUX_IMAGE files on the SD card.  If the files are present, then  
# the SOM Flash QSPI and eMMC NVM devices are programmed.
CUSTOM_BOOT_IMAGE_IDENTIFIED=-1
CUSTOM_LINUX_IMAGE_IDENTIFIED=-1
CUSTOM_BOOT_IMAGE_PROGRAM_RESULT=-1
CUSTOM_LINUX_IMAGE_PROGRAM_RESULT=-1

# Checks to see if CUSTOM boot image files are present.  If they are 
# present, then the files are programmed into the respective flash devices.
check_for_custom_boot_image_files ()
{
  if [ -e ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE} ]
  then
    if [ -f ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE} ]
    then
      echo "CUSTOM boot image file found at ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE}"
      CUSTOM_BOOT_IMAGE_IDENTIFIED=1
    else
      echo " "
      echo "The CUSTOM boot image file ${CUSTOM_BOOT_IMAGE} does not exist in the ${CUSTOM_IMAGE_FOLDER} folder"
      echo " "
    fi
  else
      echo " "
      echo "OOB boot files not found."
      echo "Unable to program QSPI and eMMC with OOB boot files."
      echo " "
  fi
}

# Checks to see if custom Linux image files are present and also determine 
# where they are located.
check_for_custom_linux_image_files ()
{
  if [ -e ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_LINUX_IMAGE} ]
  then
    echo "CUSTOM Linux image file found at ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_LINUX_IMAGE}"
    CUSTOM_LINUX_IMAGE_IDENTIFIED=1
  else
    echo " "
    echo "The CUSTOM Linux image file ${CUSTOM_LINUX_IMAGE} does not exist in the ${CUSTOM_IMAGE_FOLDER} folder"
    echo " "
  fi
}


# Programs the custom boot images to QSPI and captures the results.
program_custom_boot_image_to_qspi ()
{
  if [ -f ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE} ]
  then		
    echo " "
    echo "+++ Programming QSPI CUSTOM boot image..."
    echo " "
    flashcp -v ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE} /dev/mtd2
    CUSTOM_BOOT_IMAGE_PROGRAM_RESULT=$?
  fi
}

# Programs the custom Linux images to eMMC and captures the results.
program_custom_linux_image_to_emmc ()
{
  echo " "
  echo "+++ Erasing eMMC partition table..."
  echo " "
  dd if=/dev/zero of=/dev/mmcblk0 bs=1M count=100
  sync

  echo " "
  echo "+++ Creating eMMC partition yable..."
  echo " "
# Feed commands into fdisk to create an initial partition table.
echo "n
p
1
1
+256M
v
w" | fdisk /dev/mmcblk0 && sleep 3 && sync

# Then use fdisk to wipe the partition table out.
echo "d
w" | fdisk /dev/mmcblk0 && sleep 3 && sync

# fdisk - third time is the charm!
echo "n
p
1
1
+256M
v
w" | fdisk /dev/mmcblk0 && sleep 3 && sync

  sync

  echo " "
  echo "+++ Formatting eMMC boot partition to FAT32..."
  echo " "
  mkdosfs -F 32 /dev/mmcblk0p1
  sync

  echo " "
  echo "+++ Loading eMMC CUSTOM Linux image..."
  echo " "
			
  # Create the mount point folder and mount the destination eMMC partition.			
  mkdir -p /mnt/emmc 
  mount /dev/mmcblk0p1 /mnt/emmc
  
  # Copy the CUSTOM image to the new eMMC partition			
  cp ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_LINUX_IMAGE} /mnt/emmc/image.ub
  CUSTOM_LINUX_IMAGE_PROGRAM_RESULT=$?
  sync

  # Unmount the destination eMMC partition.			
  umount /mnt/emmc
}

# Displays the banner header for the programming results.
print_programming_results_banner_header ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"
  echo "***   UltraZed Custom Flash Image Programming                  ***"
  echo "***                                                            ***"
  echo "******************************************************************"
}

# Displays also the individual programming results.
print_programming_results ()
{
  echo "***                                                            ***"

  if [ $CUSTOM_BOOT_IMAGE_PROGRAM_RESULT == "0" ]; 		
  then
    echo "*** Custom boot image QSPI flash programming:    SUCCESS       ***"
  else
    echo "*** Custom boot image QSPI flash programming:  --- FAIL ---    ***"
  fi

  if [ $CUSTOM_LINUX_IMAGE_PROGRAM_RESULT == "0" ]; 		
  then
    echo "*** Custom Linux image eMMC flash programming:   SUCCESS       ***"
  else
    echo "*** Custom Linux image eMMC flash programming: --- FAIL ---    ***"
  fi

  echo "***                                                            ***"
}

# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# First, create the mount points and mount the microSD card
mkdir -p /mnt/sd
mount /dev/mmcblk1p1 /mnt/sd

# Discover if the CUSTOM boot image files exist.
check_for_custom_boot_image_files

# If an image was discovered for booting, also look for Linux image files 
# which need to be programmed also.
if [ $CUSTOM_BOOT_IMAGE_IDENTIFIED == "1" ];
then
  check_for_custom_linux_image_files
fi

# Program the CUSTOM boot images to QSPI.
if [ $CUSTOM_BOOT_IMAGE_IDENTIFIED == "1" ];
then
  program_custom_boot_image_to_qspi
fi

# Program the CUSTOM Linux image(s) to eMMC.
if [ $CUSTOM_LINUX_IMAGE_IDENTIFIED == "1" ];
then
  # But only if the QSPI programming was already successful.
  if [ $CUSTOM_BOOT_IMAGE_PROGRAM_RESULT == "0" ];
  then
    program_custom_linux_image_to_emmc
  fi
fi

# Display the "Results" test banner so that the test operator can see that 
# the programming.
print_programming_results_banner_header

# Show programming results for flash programming.
print_programming_results

echo "******************************************************************"
echo " "

# Don't forget to unmount the microSD card
umount /mnt/sd

# END HERE: All done with the programming.
