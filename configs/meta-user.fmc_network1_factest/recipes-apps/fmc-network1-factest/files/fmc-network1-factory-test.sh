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
#                      Copyright(c) 2018 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------
# 
#  Create Date:         Jun 25, 2018
#  Design Name:         UltraZed-EG + PCIe Carrier + FMC-NETWORK1
#  Module Name:         fmc-network1-factory-test.sh
#  Project Name:        FMC-NETWORK1 HW Factory Acceptance Test Script
#  Target Devices:      Xilinx Zynq UltraScale+ MPSoC
#  Hardware Boards:     UltraZed-EG + PCIe Carrier + FMC-NETWORK1
# 
#  Tool versions:       Xilinx Vivado 2017.4
# 
#  Description:         Startup Script for FMC-NETWORK1 HW Factory Acceptance 
#                       Test design
# 
#  Dependencies:        
#
#  Revision:            Jun 25, 2018: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/bin/sh

# FAT version information.
FAT_VERSION_NUMBER=1.0
FMC_PART_NUMBER=FMC-NETWORK1

# Sleep interval between tests.
SLEEP_INTERVAL_NORMAL=1

# Information used to program and erase the I2C clock config EEPROM
TEST_APP_FOLDER=/home/root
EEPROM_PROGRAM_FILE=AT24C02D_28FEB18_EEPROM.TXT
EEPROM_ERASE_FILE=ERASE_EEPROM.TXT

# Information about where test applications are located and what they are named:
EEPROM_TEST_APP=eeprom-test.sh

# Setup variables to store individual test results into.
# Common tests
ETHERNET1_PING_TEST_RESULT=-1
ETHERNET2_PING_TEST_RESULT=-1
IIC_MAC_EEPROM1_TEST_RESULT=-1
IIC_MAC_EEPROM1_TEST_RESULT=-1
IIC_IPMI_EEPROM_TEST_RESULT=-1
TEST_TEMP_EMPTY=eeprom_empty.txt
TEST_TEMP_PROGRAMMED=eeprom_programmed.txt
TEST_TEMP_TARGET=eeprom_contents.txt

# Information used to execute comparison test between expected results
# and actual read results from target I2C device.
TEST_TEMP_FOLDER=/tmp

# Displays the Factory Acceptance Test start banner.
print_fat_start_banner ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"                
  echo "*** STARTING FACTORY TEST OF ALL ${FMC_PART_NUMBER} PERIPHERALS      ***"
  echo "***                                                            ***"
  echo "******************************************************************"
  echo " "
}

# Performs all of the tests for the target FMC board.
launch_all_tests ()
{
  # Move the carrier Ethernet port to something out of the test subnets.
  ifconfig eth0 192.168.0.10
  sleep ${SLEEP_INTERVAL_NORMAL}

  # Launch the Ethernet1 ping test.
  echo " "
  echo "+++ Running Ethernet1 Ping Test..."
  echo " "
  
  ifconfig eth1 192.168.1.10
  sleep ${SLEEP_INTERVAL_NORMAL}
  sleep ${SLEEP_INTERVAL_NORMAL}
  sleep ${SLEEP_INTERVAL_NORMAL}
  sleep ${SLEEP_INTERVAL_NORMAL}
  sleep ${SLEEP_INTERVAL_NORMAL}
  
  ping -c 10 192.168.1.100
  ETHERNET1_PING_TEST_RESULT=$?

  if [ $ETHERNET1_PING_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** ETHERNET1 PING TEST:  PASS                                 ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** ETHERNET1 PING TEST:  FAIL                                 ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  # Launch the Ethernet2 ping test.
  echo " "
  echo "+++ Running Ethernet2 Ping Test..."
  echo " "
  
  ifconfig eth2 192.168.2.10
  sleep ${SLEEP_INTERVAL_NORMAL}
  sleep ${SLEEP_INTERVAL_NORMAL}
  sleep ${SLEEP_INTERVAL_NORMAL}
  sleep ${SLEEP_INTERVAL_NORMAL}
  sleep ${SLEEP_INTERVAL_NORMAL}
  
  ping -c 10 192.168.2.100
  ETHERNET2_PING_TEST_RESULT=$?

  if [ $ETHERNET2_PING_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** ETHERNET2 PING TEST:  PASS                                 ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** ETHERNET2 PING TEST:  FAIL                                 ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  # Launch the Carrier IIC Ethernet MAC EEPROM1 test.
  if [ -e ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
    then
      echo " "
      echo "+++ Running Carrier IIC Ethernet MAC EEPROM1 Test..."
      echo " "
      ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 3 --slave 0x51
      IIC_MAC_EEPROM1_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${EEPROM_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi


  # Launch the Carrier IIC Ethernet MAC EEPROM2 test.
  if [ -e ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
    then
      echo " "
      echo "+++ Running Carrier IIC Ethernet MAC EEPROM2 Test..."
      echo " "
      ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 5 --slave 0x51
      IIC_MAC_EEPROM2_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The file ${EEPROM_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

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
 0090|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 00a0|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 00b0|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 00c0|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 00d0|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 00e0|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
 00f0|  ff ff ff ff ff ff ff ff   ff ff ff ff ff ff ff ff 
" > ${TEST_TEMP_FOLDER}/${TEST_TEMP_EMPTY}

  # Define the expected output of eeprog here for a programmed EEPROM
  echo "
 0000|  01 00 00 12 00 01 00 ec   02 02 0d ef 00 00 b4 00 
 0010|  ab 00 bd 00 00 00 0a 00   e8 03 02 02 0d 2a c5 01 
 0020|  4a 01 39 01 5a 01 00 00   0a 00 e8 03 02 02 0d e2 
 0030|  0d 02 b0 04 74 04 ec 04   00 00 00 00 00 00 01 02 
 0040|  0d fd f3 03 00 00 00 00   00 00 00 00 00 00 00 00 
 0050|  01 02 0d e0 10 04 b4 00   b0 00 b7 00 00 00 00 00 
 0060|  01 00 01 02 0d fb f5 05   00 00 00 00 00 00 00 00 
 0070|  00 00 00 00 fa 02 08 d5   27 0c 15 00 00 00 00 0a 
 0080|  00 82 00 00 7e 00 00 00   00 00 00 00 00 00 00 00 
 0090|  01 06 00 00 00 00 c5 41   76 6e 65 74 cb 46 4d 43 
 00a0|  2d 4e 45 54 57 4f 52 4b   c0 cd 41 45 53 2d 46 4d 
 00b0|  43 2d 4e 57 31 2d 47 c0   c1 00 00 00 00 00 00 dd 
 00c0|  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
 00d0|  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
 00e0|  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
 00f0|  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
" > ${TEST_TEMP_FOLDER}/${TEST_TEMP_PROGRAMMED}

  echo " "
  echo "+++ Running IPMI EEPROM Test..."
  echo " "

  # Capture fresh test results to a target file.
  eeprog -xf /dev/i2c-1 0x50 -8 -r 0x00:0x100 > ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}

  sleep ${SLEEP_INTERVAL_NORMAL}
  sync

  # Compare the target test results with the 'golden' results.  If the files 
  # match identically, the return code from diff is 0 and the test passes.
  diff -q ${TEST_TEMP_FOLDER}/${TEST_TEMP_EMPTY} ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET} >/dev/null

  # Capture the return code from diff as the test result.
  IIC_IPMI_EEPROM_EMPTY_TEST_RESULT=$?

  echo " "
  echo "Testing for empty IPMI EEPROM"
  echo " "

  # If the EEPROM is empty...
  if [ ${IIC_IPMI_EEPROM_EMPTY_TEST_RESULT} == "0" ];
  then
    # IPMI EEPROM is empty and needs to be programmed
    echo " "
    echo "+++ IPMI EEPROM is empty and needs to be programmed"
    echo " "
    sleep ${SLEEP_INTERVAL_NORMAL}  
    if [ -e ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
    then
      if [ -f ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
      then
        ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 1 --slave 0x50 --file ${TEST_APP_FOLDER}/${EEPROM_PROGRAM_FILE}
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
  IIC_IPMI_EEPROM_TEST_RESULT=$?

  echo " "
  echo "Testing for correct contents in IPMI EEPROM"
  echo " "
    
  if [ ${IIC_IPMI_EEPROM_TEST_RESULT} == "0" ];
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** IIC IPMI CONFIG EEPROM Test:  PASS                         ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    # EEPROM is programmed, but not with the correct contents
    echo " "
    echo "+++ Reprogramming the IIC IPMI EEPROM with the correct contents..."
    echo " "
    sleep ${SLEEP_INTERVAL_NORMAL}  
    if [ -e ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
    then
      if [ -f ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} ]
      then
        echo " "
        echo "+++ First erase the incorrect contents..."
        echo " "
        ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 1 --slave 0x50 --file ${TEST_APP_FOLDER}/${EEPROM_ERASE_FILE}
        sleep ${SLEEP_INTERVAL_NORMAL}
        sleep ${SLEEP_INTERVAL_NORMAL}
        sleep ${SLEEP_INTERVAL_NORMAL}
        echo " "
        echo "+++ Program the correct contents..."
        echo " "
        ${TEST_APP_FOLDER}/${EEPROM_TEST_APP} --bus 1 --slave 0x50 --file ${TEST_APP_FOLDER}/${EEPROM_PROGRAM_FILE}
	# Capture the return code which indicates whether the specified file 
	# was programmed correctly or not.
	IIC_IPMI_EEPROM_TEST_RESULT=$?
      fi
    else
      echo " "
      echo "The file ${EEPROM_TEST_APP} does not exist in the ${TEST_APP_FOLDER} folder"
      echo " "
    fi
  fi
  
} # launch_all_tests ()

# Displays the banner header for the final results.
print_fat_results_banner_header ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"
  echo "***   ${FMC_PART_NUMBER} Factory Test Build V${FAT_VERSION_NUMBER} Complete            ***"
  echo "***                                                            ***"
  echo "******************************************************************"
}

# Displays also the test results for all common tests.
print_fat_results_all_tests ()
{
  echo "***                                                            ***"
  if [ $ETHERNET1_PING_TEST_RESULT == "0" ];
  then
    echo "*** Ethernet1 Ping:                    PASS                    ***"
  else
    echo "*** Ethernet1 Ping:                --- FAIL ---                ***"
  fi

  if [ $ETHERNET2_PING_TEST_RESULT == "0" ];
  then
    echo "*** Ethernet2 Ping:                    PASS                    ***"
  else
    echo "*** Ethernet2 Ping:                --- FAIL ---                ***"
  fi

  if [ $IIC_MAC_EEPROM1_TEST_RESULT == "0" ]; 
  then
    echo "*** IIC MAC EEPROM1 Test:              PASS                    ***"
  else
    echo "*** IIC MAC EEPROM1 Test:          --- FAIL ---                ***"
  fi

  if [ $IIC_MAC_EEPROM2_TEST_RESULT == "0" ]; 
  then
    echo "*** IIC MAC EEPROM2 Test:              PASS                    ***"
  else
    echo "*** IIC MAC EEPROM2 Test:          --- FAIL ---                ***"
  fi

  if [ $IIC_IPMI_EEPROM_TEST_RESULT == "0" ]; 
  then
    echo "*** IIC IPMI EEPROM Test:              PASS                    ***"
  else
    echo "*** IIC IPMI EEPROM Test:          --- FAIL ---                ***"
  fi

  echo "***                                                            ***"
}

# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# Display the "Start" test banner so that the test operator can see that the 
# FAT is starting.
print_fat_start_banner

# Run all of the tests for FMC-NETWORK1 board here.
launch_all_tests

# Display the "Results" test banner so that the test operator can see that 
# the FAT is complete.
print_fat_results_banner_header

# Show test results for all tests for the FAT procedures.
print_fat_results_all_tests

echo "******************************************************************"
echo " "

# END HERE: All done with the testing.
