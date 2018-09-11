# ----------------------------------------------------------------------------
#
#        ** **        **          **  ****      **  **********  **********
#       **   **        **        **   ** **     **  **              **
#      **     **        **      **    **  **    **  **              **
#     **       **        **    **     **   **   **  *********       **
#    **         **        **  **      **    **  **  **              **
#   **           **        ****       **     ** **  **              **
#  **  .........  **        **        **      ****  **********      **
#     ...........
#                                     Reach Further
#
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
#                      Copyright(c) 2016 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------
# 
#  Create Date:         Dec 20, 2017
#  Design Name:         UltraZed-EV + EV Carrier
#  Module Name:         uz7ev-evcc-factory-test.sh
#  Project Name:        UltraZed-EV EV Carrier HW Factory Acceptance Test Script
#  Target Devices:      Xilinx Zynq UltraScale+ EV MPSoC
#  Hardware Boards:     UltraZed-EV + EV Carrier
# 
#  Tool versions:       Xilinx Vivado 2017.3
# 
#  Description:         Startup Script for UltraZed-EV EVCC HW Factory Acceptance 
#                       Test design
# 
#  Dependencies:        
#
#  Revision:            Dec 20, 2017: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/bin/sh

# FAT version information.
FAT_VERSION_NUMBER=1.1

# Sleep interval between tests.
SLEEP_INTERVAL_NORMAL=1

# Names and locations of files to be programmed into QSPI Flash and eMMC
# These folders are the path of where the microSD card is mounted
OOB_IMAGE_FOLDER=/mnt/sd
OOB_BOOT_IMAGE=BOOT_EMMC_CUSTOM.bin
OOB_LINUX_IMAGE=image_custom.ub


# Information used to program and erase the I2C clock config EEPROM
TEST_APP_FOLDER=/home/root
EEPROM_PROGRAM_FILE=IDT8T49N241_10Aug17_EEPROM.TXT
EEPROM_ERASE_FILE=ERASE_EEPROM.TXT

# Variables used to determine whether a SOM is being tested or if a Carrier is
# being tested.  This is determined by the presense of OOB_BOOT_FIRMWARE and
# OOB_LINUX_IMAGE files on the SD card.  If the files are present, then the 
# SOM tests are run along with programming steps for QSPI and eMMC NVM 
# devices.
CARRIER_PART_NUMBER=AES-US3CAR-1
SOM_PART_NUMBER=AES-US2SOM-1
SOM_ONLY_TESTS=-1


# DEBUG uncomment next line to force all tests to run
# SOM_ONLY_TESTS=1

# Information about where test applications are located and what they are named:
EEPROM_TEST_APP=eeprom-test.sh
IIC_GPIO_EXPANDER_TEST_APP=iic-gpio-expander-test.sh
PCIE_TEST_APP=pcie-enumeration-test.sh
PL_DDR4_TEST_APP=pl-ddr4-test
PL_GPIO_LOOPBACK_TEST_APP=pl-gpio-loopback-test
PS_PMOD_LOOPBACK_TEST_APP=ps-pmod-loopback-test
USB3_LINK_TEST_APP=usb3-link-test.sh
USER_SWITCH_TEST_APP=user-switch-test
USER_LED_TEST_APP=user-led-test


# Setup variables to store individual test results into.
# Common tests
IIC_MAC_EEPROM_TEST_RESULT=-1
USB_DEVICE_READ_TEST_RESULT=-1
USB3_LINK_TEST_RESULT=-1
PL_GPIO_LOOPBACK_TEST_RESULT=-1

# Carrier-only tests
IIC_CLOCK_EEPROM_EMPTY_TEST_RESULT=-1
IIC_CLOCK_EEPROM_CONTENTS_TEST_RESULT=-1
PCIE_TEST_RESULT=-1

# SOM-only tests
ETHERNET_PING_TEST_RESULT=-1
IIC_GPIO_EXPANDER_TEST_RESULT=-1
PS_PMOD_LOOPBACK_TEST_RESULT=-1
USER_SWITCH_TEST_RESULT=-1
USER_LED_TEST_RESULT=-1
IIC_SOM_EEPROM_TEST_RESULT=-1
PL_DDR4_TEST_RESULT=-1
OOB_IMAGE_PROGRAM_RESULT=-1

# Information used to execute comparison test between expected results
# and actual read results from target I2C device.
TEST_TEMP_FOLDER=/tmp
TEST_TEMP_EMPTY=eeprom_empty.txt
TEST_TEMP_PROGRAMMED=eeprom_programmed.txt
TEST_TEMP_TARGET=eeprom_contents.txt
TEST_TEMP_PL_DDR4_GOLDEN=pl_ddr4_golden.txt
TEST_TEMP_PL_DDR4_DEVMEM=pl_ddr4_devmem.txt

# Checks to see if OOB boot firmware files are present.  If they are present,
# then the factory test also includes the SOM only portion of tests by setting
# SOM_ONLY_TESTS to 1.
check_for_som_oob_boot_firmware_files ()
{
  if [ -e ${OOB_IMAGE_FOLDER}/${OOB_BOOT_IMAGE} ]
  then
    if [ -f ${OOB_IMAGE_FOLDER}/${OOB_BOOT_IMAGE} ]
    then
      echo "OOB Boot Firmware file found at ${OOB_IMAGE_FOLDER}/${OOB_BOOT_IMAGE}"
      SOM_ONLY_TESTS=1
    else
      echo " "
      echo "The OOB Boot Firmware file ${OOB_BOOT_IMAGE} does not exist in the ${OOB_IMAGE_FOLDER} folder"
      echo "WARNING:  Carrier only tests will be performed!"
      echo " "
    fi
  else
      echo " "
      echo "OOB boot files not found!"
      echo "Unable to program QSPI with OOB Boot Firmware"
      echo "WARNING:  Carrier only tests will be performed!"
      echo " "
  fi
} # check_for_som_oob_boot_firmware_files ()

# Checks to see if OOB Linux image files are present and also determine 
# where they are located.
check_for_som_oob_linux_image_files ()
{
  if [ -e ${OOB_IMAGE_FOLDER}/${OOB_LINUX_IMAGE} ]
  then
    echo "OOB Linux Firmware file found at ${OOB_IMAGE_FOLDER}/${OOB_LINUX_IMAGE}"
  else
    echo " "
    echo "The OOB Linux Firmware file ${OOB_LINUX_IMAGE} does not exist in the ${OOB_IMAGE_FOLDER} folder"
    echo " "
  fi
} # check_for_som_oob_linux_image_files ()


# Displays the Factory Acceptance Test start banner.
print_fat_start_banner ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"                

  # If only SOM tests are being run, display the SOM part number 
  if [ $SOM_ONLY_TESTS == "1" ];
  then
    echo "*** STARTING FACTORY TEST OF ALL ${SOM_PART_NUMBER} PERIPHERALS      ***"
  else
    echo "*** STARTING FACTORY TEST OF ALL ${CARRIER_PART_NUMBER} PERIPHERALS      ***"
  fi

  echo "***                                                            ***"
  echo "******************************************************************"
  echo " "
}

# Performs all of the tests that are common to both SOM and Carrier boards.
launch_all_common_tests ()
{
  # Launch the USB device read test
  
  # The USB test involves mounting the test USB drive and checking that a 
  # file can be read from the removable drive.
  echo " "
  echo "+++ Running USB Device Read Test..."
  echo " "

  # Check to see if the USB mass storage block device is enumerated.
  if [ -b /dev/sda1 ]
  then
    mkdir /mnt/sda
    mount /dev/sda1 /mnt/sda

    echo "Avnet is one of the world's largest electronics components, technology, distribution and services companies, offering leading design chain services combined with world-class supply chain services in support of the electronics industry. It is an operating group of Avnet, Inc. 

Avnet is focused on exceeding customers'​ expectations. We serve electronic original equipment manufacturers (EOEMs) and electronic manufacturing services (EMS) providers in more than 80 countries, distributing electronic components and embedded solutions from leading manufacturers. From design to delivery, we are a company that is dedicated to support across the board. 

Avnet Americas specifically serves customers in the United States, Canada, Mexico and Brazil. Providing leading products, training and support to the electronics community.

Bring the power of the world's largest distributor to your desktop. Get access to over 5 million products, including pricing and availability, datasheets, cross reference and parametric data and more on the Avnet e-commerce site: http://avnetexpress.avnet.com/

Follow Avnet on Twitter: @AvnetDesignWire

Our Markets and Technologies include:
Automotive
Defense & Aerospace
Embedded Vision
Internet of Things
IP & E
Programmable Logic
& More!

Specialties
Electronics Components Design-Chain, Electronics Components Supply-Chain, Electronics Components Distribution, Embedded Solutions, Interconnect, Passive, Electromechanical Components

Website
http://www.products.avnet.com

Industry
Semiconductors

Type
Public Company

Headquarters
2211 S 47th St Phoenix, AZ 85034 United States

Company Size
10,001+ employees

Founded
1921" > /tmp/usb_device_read_golden.txt

    sync
    cp -f /tmp/usb_device_read_golden.txt /mnt/sda/usb_device_read_test.txt
    sync

    if [ -f /mnt/sda/usb_device_read_test.txt ]
    then
      diff -q /tmp/usb_device_read_golden.txt /mnt/sda/usb_device_read_test.txt
      USB_DEVICE_READ_TEST_RESULT=$?
    else
      echo "******************************************************************"
      echo " "
      echo "   Test file not found on USB Thumb Drive, make sure that you are"
      echo "   using the USB test thumb drive and re-run factoy tests"
      echo " "
      echo "******************************************************************"
      echo " "
    fi
	
    umount /mnt/sda
    rm -rf /mnt/sda

  else
    echo "******************************************************************"
    echo " "
    echo "   No USB Mass Storage Block Device Enumerated!"
    echo "   Connect the test USB Thumb drive to Carrier board"
    echo "   USB Type-A connector and re-run factory tests"
    echo " "
    echo "******************************************************************"
    echo " "
  fi

  if [ $USB_DEVICE_READ_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USB READ TEST:  PASS                                       ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USB READ TEST:  FAIL                                       ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the Carrier USB3 LINK test.
  if [ -e ${TEST_APP_FOLDER}/${USB3_LINK_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${USB3_LINK_TEST_APP} ]
    then
      echo " "
      echo "+++ Running USB3 LINK Test..."
      echo " "
      ${TEST_APP_FOLDER}/${USB3_LINK_TEST_APP}
      USB3_LINK_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${USB3_LINK_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the PL GPIO loopback test.
  if [ -e ${TEST_APP_FOLDER}/${PL_GPIO_LOOPBACK_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${PL_GPIO_LOOPBACK_TEST_APP} ]
    then
      echo " "
      echo "+++ Running PL GPIO Loopback Test..."
      echo " "
      ${TEST_APP_FOLDER}/${PL_GPIO_LOOPBACK_TEST_APP}
      PL_GPIO_LOOPBACK_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${PL_GPIO_LOOPBACK_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  if [ $PL_GPIO_LOOPBACK_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PL GPIO/PMOD LOOPBACK:  PASS                               ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PL GPIO/PMOD LOOPBACK:  FAIL                               ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the Carrier IIC Ethernet MAC EEPROM test.
  if [ -e ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
    then
      echo " "
      echo "+++ Running Carrier IIC Ethernet MAC EEPROM Test..."
      echo " "
      ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 2 --slave 0x51
      IIC_MAC_EEPROM_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${EEPROM_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

} # launch_all_common_tests ()

# Performs all of the tests that are specific to the carrier board.
launch_all_carrier_specific_tests ()
{
  # Launch carrier IIC clock2 config EEPROM test IF the EEPROM is empty
  # Define the expected output of eeprog here for an empty EEPROM
  echo "
 0000|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 0010|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 0020|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 0030|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 0040|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 0050|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 0060|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 0070|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 0080|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
" > ${TEST_TEMP_FOLDER}/${TEST_TEMP_EMPTY}

  # Define the expected output of eeprog here for a programmed EEPROM
  echo "
 0000|  ff ff ff ff ff fe ef 00   03 00 31 00 00 01 00 00 
 0010|  01 07 00 00 07 00 00 77   6d 00 00 00 00 00 00 ff 
 0020|  ff ff ff 01 3f 00 29 00   04 df 57 00 01 00 00 d0 
 0030|  00 00 00 00 00 00 00 00   00 0f 00 00 00 44 48 02 
 0040|  00 04 00 00 0a 00 00 06   00 00 0a 00 00 00 00 00 
 0050|  00 00 00 00 00 00 00 0c   63 fc 8d 06 66 66 66 0c 
 0060|  63 fc 8d 00 00 00 00 00   89 0a 2b 20 00 00 00 00 
 0070|  00 00 00 00 00 00 00 00   00 00 27 0c 00 00 00 00 
 0080|  00 00 00 00 49 ff ff ff   ff ff ff ff ff ff ff ff 
" > ${TEST_TEMP_FOLDER}/${TEST_TEMP_PROGRAMMED}

  echo " "
  echo "+++ Running Carrier IIC CLOCK CONFIG EEPROM Test..."
  echo " "

  # Capture fresh test results to a target file.
  eeprog -xf /dev/i2c-2 0x52 -8 -r 0x00:0x90 > ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}

  sleep ${SLEEP_INTERVAL_NORMAL}
  sync

  # Compare the target test results with the 'golden' results.  If the files 
  # match identically, the return code from diff is 0 and the test passes.
  diff -q ${TEST_TEMP_FOLDER}/${TEST_TEMP_EMPTY} ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET} >/dev/null

  # Capture the return code from diff as the test result.
  IIC_CLOCK_EEPROM_EMPTY_TEST_RESULT=$?

  echo " "
  echo "Testing for empty clock config EEPROM"
  echo " "

  # If the EEPROM is empty...
  if [ ${IIC_CLOCK_EEPROM_EMPTY_TEST_RESULT} == "0" ];
  then
    # Clock config EEPROM is empty and needs to be programmed
    echo " "
    echo "+++ Clock config EEPROM is empty and needs to be programmed"
    echo " "
    sleep ${SLEEP_INTERVAL_NORMAL}  
    if [ -e ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
    then
      if [ -f ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
      then
        ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 2 --slave 0x52 --file ${TEST_APP_FOLDER}/${EEPROM_PROGRAM_FILE}
        echo " "
        echo "*** !!!TOGGLE POWER TO REBOOT BEFORE CONTINUING!!! ***"
        echo " "
        read -p "*** TURN THE BOARD OFF NOW ***"
        echo " "
      fi
    else
      echo " "
      echo "The file ${EEPROM_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
      echo " "
    fi
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}
  sync

  # Compare the target test results with the 'golden' results.  If the files 
  # match identically, the return code from diff is 0 and the test passes.
  diff -q ${TEST_TEMP_FOLDER}/${TEST_TEMP_PROGRAMMED} ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET} >/dev/null
  # Capture the return code from diff as the test result.
  IIC_CLOCK_EEPROM_CONTENTS_TEST_RESULT=$?

  echo " "
  echo "Testing for correct contents in config EEPROM"
  echo " "
    
  if [ ${IIC_CLOCK_EEPROM_CONTENTS_TEST_RESULT} == "0" ];
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** IIC CLOCK CONFIG EEPROM Test:  PASS                        ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    # EEPROM is programmed, but not with the correct contents
    echo " "
    echo "+++ Reprogramming the IIC Clock EEPROM with the correct contents..."
    echo " "
    sleep ${SLEEP_INTERVAL_NORMAL}  
    if [ -e ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
    then
      if [ -f ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
      then
        echo " "
        echo "+++ First erase the incorrect contents..."
        echo " "
        ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 2 --slave 0x52 --file ${TEST_APP_FOLDER}/${EEPROM_ERASE_FILE}
        sleep ${SLEEP_INTERVAL_NORMAL}
        sleep ${SLEEP_INTERVAL_NORMAL}
        sleep ${SLEEP_INTERVAL_NORMAL}
        echo " "
        echo "+++ Program the correct contents..."
        echo " "
        ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 2 --slave 0x52 --file ${TEST_APP_FOLDER}/${EEPROM_PROGRAM_FILE}
        echo " "
        echo "*** !!!TOGGLE POWER TO REBOOT BEFORE CONTINUING!!! ***"
        echo " "
        read -p "*** TURN THE BOARD OFF NOW ***"
        echo " "
      fi
    else
      echo " "
      echo "The file ${EEPROM_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
      echo " "
    fi
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the Carrier PCIe enumeration test.
  if [ -e ${TEST_APP_FOLDER}/${PCIE_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${PCIE_TEST_APP} ]
    then
      echo " "
      echo "+++ Running PCIe Enumeration Test..."
      echo " "
      ${TEST_APP_FOLDER}/${PCIE_TEST_APP}
      PCIE_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${PCIE_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

} # launch_all_carrier_specific_tests ()


# Performs all of the tests that are related to the SOM only.
launch_all_som_specific_tests ()
{
  # Launch the Ethernet ping test.
  echo " "
  echo "+++ Running Ethernet Ping Test..."
  echo " "
  
  ifconfig eth0 192.168.1.100
  sleep ${SLEEP_INTERVAL_NORMAL}
  
  ping -c 10 192.168.1.10
  ETHERNET_PING_TEST_RESULT=$?

  if [ $ETHERNET_PING_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** ETHERNET PING TEST:  PASS                                  ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** ETHERNET PING TEST:  FAIL                                  ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the IIC GPIO expander test.
  if [ -e ${TEST_APP_FOLDER}/${IIC_GPIO_EXPANDER_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${IIC_GPIO_EXPANDER_TEST_APP} ]
    then
      echo " "
      echo "+++ Running IIC GPIO Expander Test..."
      echo " "
      ${TEST_APP_FOLDER}/${IIC_GPIO_EXPANDER_TEST_APP}
      IIC_GPIO_EXPANDER_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${IIC_GPIO_EXPANDER_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the PS Pmod loopback test.
  if [ -e ${TEST_APP_FOLDER}/${PS_PMOD_LOOPBACK_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${PS_PMOD_LOOPBACK_TEST_APP} ]
    then
      echo " "
      echo "+++ Running PS Pmod Loopback Test..."
      echo " "
      ${TEST_APP_FOLDER}/${PS_PMOD_LOOPBACK_TEST_APP}
      PS_PMOD_LOOPBACK_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${PS_PMOD_LOOPBACK_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  if [ $PS_PMOD_LOOPBACK_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PS PMOD LOOPBACK:  PASS                                    ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PS PMOD LOOPBACK:  FAIL                                    ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the User Switch test.
  if [ -e ${TEST_APP_FOLDER}/${USER_SWITCH_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${USER_SWITCH_TEST_APP} ]
    then
      echo " "
      echo "+++ Running User Switch Test..."
      echo " "
      ${TEST_APP_FOLDER}/${USER_SWITCH_TEST_APP}
      USER_SWITCH_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${USER_SWITCH_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  if [ $USER_SWITCH_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USER SWITCH:  PASS                                         ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USER SWITCH:  FAIL                                         ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the User LED and PB Switch test.
  if [ -e ${TEST_APP_FOLDER}/${USER_LED_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${USER_LED_TEST_APP} ]
    then
      echo " "
      echo "+++ Running User LED and PB Switch Test..."
      echo " "
      ${TEST_APP_FOLDER}/${USER_LED_TEST_APP}
      USER_LED_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${USER_LED_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  if [ $USER_LED_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USER LED and PB SWITCH TEST:  PASS                         ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USER LED and PB SWITCH TEST:  FAIL                         ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the SOM IIC EEPROM test.
  if [ -e ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
    then
      echo " "
      echo "+++ Running SOM IIC EEPROM Test..."
      echo " "
      ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 0 --slave 0x50
      IIC_SOM_EEPROM_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${EEPROM_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

##########

  # Launch the PL DDR4 test.

  echo "0xDEADBEEFFACEB00C" > ${TEST_TEMP_FOLDER}/${TEST_TEMP_PL_DDR4_GOLDEN}

  if [ -e ${TEST_APP_FOLDER}/${PL_DDR4_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${PL_DDR4_TEST_APP} ]
    then
      echo " "
      echo "+++ Running the PL DDR4 Test..."
      echo " "
      # The PL DDR4 test app writes a specific value to memory
      ${TEST_APP_FOLDER}/${PL_DDR4_TEST_APP}
      # Use devmem to read back the value that was written to PL DDR4
      devmem 0x400000000 64 > ${TEST_TEMP_FOLDER}/${TEST_TEMP_PL_DDR4_DEVMEM}
    fi
  else
    echo " "
    echo "The file ${EEPROM_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi
  
  sleep ${SLEEP_INTERVAL_NORMAL}
  sync
  
  # Compare the target test results with the 'golden' results.  If the files 
  # match identically, the return code from diff is 0 and the test passes.
  diff -q ${TEST_TEMP_FOLDER}/${TEST_TEMP_PL_DDR4_GOLDEN} ${TEST_TEMP_FOLDER}/${TEST_TEMP_PL_DDR4_DEVMEM} >/dev/null
  PL_DDR4_TEST_RESULT=$?
  
  sleep ${SLEEP_INTERVAL_NORMAL}

  if [ $PL_DDR4_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PL DDR4 Test:  PASS                                        ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PL DDR4 Test:  FAIL                                        ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi
  
} # launch_all_som_specific_tests ()

# Programs the OOB boot images to QSPI.  Used on SOM FAT runs.
#program_oob_boot_firmware_to_qspi ()
#{
  #if [ -f ${OOB_IMAGE_FOLDER}/${OOB_BOOT_IMAGE} ]
  #then		
    #echo " "
    #echo "+++ Programming QSPI OOB Image..."
    #echo " "
    #flashcp -v ${OOB_IMAGE_FOLDER}/${OOB_BOOT_IMAGE} /dev/mtd2
    #OOB_IMAGE_PROGRAM_RESULT=$?
  #fi
#} # program_oob_boot_firmware_to_qspi ()

# Programs the OOB Linux images to eMMC.  Used on SOM FAT runs.
program_oob_linux_image_to_emmc ()
{
  echo " "
  echo "+++ Erasing eMMC Partition Table..."
  echo " "
  dd if=/dev/zero of=/dev/mmcblk0 bs=1M count=100
  sync

  echo " "
  echo "+++ Creating eMMC Partition Table..."
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
  echo "+++ Formatting eMMC Boot Partition to FAT32..."
  echo " "
  mkdosfs -F 32 /dev/mmcblk0p1
  sync

  echo " "
  echo "+++ Loading eMMC OOB Linux Image..."
  echo " "
			
  # Create the mount point folder and mount the destination eMMC partition.	
  mkdir -p /mnt/emmc
  mount /dev/mmcblk0p1 /mnt/emmc
  
  # Copy the OOB image to the new eMMC partition
  cp ${OOB_IMAGE_FOLDER}/${OOB_LINUX_IMAGE} /mnt/emmc/image.ub
  OOB_IMAGE_PROGRAM_RESULT=$?
  sync

  # Unmount the destination eMMC partition.			
  umount /mnt/emmc
  rm -rf /mnt/emmc

  # Show some more meaningful results to the test operator.
  sleep ${SLEEP_INTERVAL_NORMAL}
  if [ $OOB_IMAGE_PROGRAM_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** OOB IMAGE PROGRAMMING:  PASS                               ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** OOB IMAGE PROGRAMMING:  FAIL                               ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi
} # program_oob_linux_image_to_emmc ()

# Displays the banner header for the final results.
print_fat_results_banner_header ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"

  if [ $SOM_ONLY_TESTS == "1" ];
  then
    echo "***   ${SOM_PART_NUMBER} Factory Test Build V${FAT_VERSION_NUMBER} Complete            ***"
  else
    echo "***   ${CARRIER_PART_NUMBER} Factory Build Test V${FAT_VERSION_NUMBER} Complete            ***"
  fi

  echo "***                                                            ***"
  echo "******************************************************************"
}

# Displays also the test results for all common tests.
print_fat_results_common_tests ()
{
  echo "***                                                            ***"
  if [ $IIC_MAC_EEPROM_TEST_RESULT == "0" ]; 
  then
    echo "*** IIC MAC EEPROM Test:               PASS                    ***"
  else
    echo "*** IIC MAC EEPROM Test:           --- FAIL ---                ***"
  fi

  if [ $USB_DEVICE_READ_TEST_RESULT == "0" ]; 
  then
    echo "*** USB Read Link Test:                PASS                    ***"
  else
    echo "*** USB Read Link Test:            --- FAIL ---                ***"
  fi

  if [ $USB3_LINK_TEST_RESULT == "0" ]; 
  then
    echo "*** USB3 Link Test:                    PASS                    ***"
  else
    echo "*** USB3 Link Test:                --- FAIL ---                ***"
  fi

  if [ $PL_GPIO_LOOPBACK_TEST_RESULT == "0" ]; 
  then
    echo "*** PL GPIO Loopback Test:             PASS                    ***"
  else
    echo "*** PL GPIO Loopback Test:         --- FAIL ---                ***"
  fi
  
} # print_fat_results_common_tests ()


# Display the test results for all carrier tests.
print_fat_results_carrier_specific_tests ()
{
  echo "***                                                            ***"
  if [ $IIC_CLOCK_EEPROM_CONTENTS_TEST_RESULT == 0 ];
  then
    echo "*** IIC Clock Config EEPROM Test:      PASS                    ***"
  else
    echo "*** IIC Clock Config EEPROM Test:  --- FAIL ---                ***"
  fi

  if [ $PCIE_TEST_RESULT == "0" ]; 
  then
    echo "*** PCIe Enumeration Test:             PASS                    ***"
  else
    echo "*** PCIe Enumeration Test:         --- FAIL ---                ***"
  fi

  echo "***                                                            ***"
} # print_fat_results_carrier_specific_tests ()

# Displays also the test results for the SOM.
print_fat_results_som_specific_tests ()
{
  echo "***                                                            ***"

  if [ $ETHERNET_PING_TEST_RESULT == 0 ];
  then
    echo "*** Ethernet Ping:                     PASS                    ***"
  else
    echo "*** Ethernet Ping:                 --- FAIL ---                ***"
  fi

  if [ $IIC_GPIO_EXPANDER_TEST_RESULT == "0" ]; 
  then
    echo "*** IIC GPIO Expander Test:            PASS                    ***"
  else
    echo "*** IIC GPIO Expander Test:        --- FAIL ---                ***"
  fi

  if [ $PS_PMOD_LOOPBACK_TEST_RESULT == "0" ]; 
  then
    echo "*** PS Pmod Loopback Test:             PASS                    ***"
  else
    echo "*** PS Pmod Loopback Test:         --- FAIL ---                ***"
  fi

  if [ $USER_SWITCH_TEST_RESULT == "0" ]; 
  then
    echo "*** User Switch Test:                  PASS                    ***"
  else
    echo "*** User Switch Test:              --- FAIL ---                ***"
  fi

  if [ $USER_LED_TEST_RESULT == "0" ]; 
  then
    echo "*** User LED and PB Switch Test:       PASS                    ***"
  else
    echo "*** User LED and PB Switch Test:   --- FAIL ---                ***"
  fi

  if [ $IIC_SOM_EEPROM_TEST_RESULT == "0" ]; 
  then
    echo "*** SOM IIC EEPROM Test:               PASS                    ***"
  else
    echo "*** SOM IIC EEPROM Test:           --- FAIL ---                ***"
  fi

  if [ $PL_DDR4_TEST_RESULT == "0" ]; 
  then
    echo "*** PL DDR4 Test:                      PASS                    ***"
  else
    echo "*** PL DDR4 Test:                  --- FAIL ---                ***"
  fi

  if [ $OOB_IMAGE_PROGRAM_RESULT == "0" ]; 		
  then
    echo "*** OOB Image Programming:             PASS                    ***"
  else
    echo "*** OOB Image Programming:         --- FAIL ---                ***"
  fi

  echo "***                                                            ***"
}

# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# First, create the mount points and mount the microSD card
mkdir -p /mnt/sd
mount /dev/mmcblk1p1 /mnt/sd

# Discover if the OOB boot firmware files exist.  If those files do not exist,
# then this is a FAT run for carrier card only because SOM_ONLY_TESTS 
# is expected to be set to 1.
check_for_som_oob_boot_firmware_files

# If firmware was discovered for booting, also look for Linux image files 
# which need to be programmed also.
if [ $SOM_ONLY_TESTS == "1" ];
then
  check_for_som_oob_linux_image_files
fi

# Display the "Start" test banner so that the test operator can see that the 
# FAT is starting.
print_fat_start_banner

# Run all of the tests that are common to both SOM and Carrier boards here.
launch_all_common_tests

# If this is a SOM FAT run, then launch all of the SOM specific tests here.
if [ $SOM_ONLY_TESTS == "1" ];
then
  launch_all_som_specific_tests
else
  launch_all_carrier_specific_tests
fi 

# If this is a SOM FAT run, program the OOB firmware images to QSPI.
#if [ $SOM_ONLY_TESTS == "1" ];
#then
  #program_oob_boot_firmware_to_qspi
#fi

# If this is a SOM FAT run, program the OOB Linux image(s) to eMMC.
if [ $SOM_ONLY_TESTS == "1" ];
then
  # But only if the QSPI programming was successful.
  #if [ $OOB_IMAGE_PROGRAM_RESULT == "0" ];
  #then
    program_oob_linux_image_to_emmc
  #fi
fi

# Display the "Results" test banner so that the test operator can see that 
# the FAT is complete.
print_fat_results_banner_header

# Show test results for tests that are common among the SOM and Carrier
# FAT procedures.
print_fat_results_common_tests

# If this is a SOM FAT run, show test results for tests that are only for
# the SOM FAT procedure.  If not a SOM FAT run it must be a carrier FAT run, so
# print the carrier results instead.
if [ $SOM_ONLY_TESTS == "1" ];
then
  print_fat_results_som_specific_tests
else
  print_fat_results_carrier_specific_tests
fi

echo "******************************************************************"
echo " "

# Don't forget to unmount the microSD card
umount /mnt/sd
rm -rf mnt/sd

# END HERE: All done with the testing.
