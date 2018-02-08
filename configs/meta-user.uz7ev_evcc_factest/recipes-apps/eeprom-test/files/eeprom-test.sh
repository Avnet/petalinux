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
#  Create Date:         Nov 28, 2016
#  Design Name:         IIC EEPROM Test Script
#  Module Name:         eeprom-test.sh
#  Project Name:        UltraZed IIC EEPROM Test Script
#  Target Devices:      Xilinx Zynq and Zynq UltraScale+ MPSoC
#  Hardware Boards:     UltraZed-EG + I/O Carrier
#                       UltraZed-EV + EV Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
#                       Xilinx Vivado 2017.3
# 
#  Description:         Test Script for UltraZed IIC EEPROM
# 
#  Dependencies:        
#
#  Revision:            Nov 28, 2016: 1.0 Initial version
#                       Jan 17, 2018: 1.1 Added parameters which allow for
#                                         EEPROM contents to be programmed
#                                         from and ASCII encoded TXT file
#                                         and updated Avnet logo in header
# 
# ----------------------------------------------------------------------------
#!/bin/sh

# Sleep interval between tests.
SLEEP_INTERVAL_MICRO=4000
SLEEP_INTERVAL_NORMAL=1

# Information used to execute comparison test between expected results
# and actual read results from target I2C device.
TEST_TEMP_FOLDER=/tmp
TEST_TEMP_GOLDEN=eeprom_golden.txt
TEST_TEMP_TARGET=eeprom_test.txt

# Setup variable to store the test results into.
IIC_EEPROM_TEST_RESULT=-1

# Always specify an invalid file here so that it can be overridden by an 
# argument later.
INPUT=/tmp/programming_file.txt

# Parse incoming arguments here.

while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -b|--bus)
    BUS="$2"
    shift # past argument
    ;;
    -f|--file)
    INPUT="$2"
    shift # past argument
    ;;
    -s|--slave)
    SLAVE="$2"
    shift # past argument
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done

# Make sure file exits else simply run the factory test.
if [ ! -f $INPUT ]; then
    echo "$0: programming file $INPUT not found."
	echo "Testing target EEPROM at ${SLAVE} on bus i2c-${BUS}"

	# START HERE: Non-boilerplate code above should be contained within 
	# functions so that at this point simple high level calls can be made to 
	# the bigger blocks above.

	# Program the test pattern to the target EEPROM device.
	i2cset -y ${BUS} ${SLAVE} 0x00 0x00
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x01 0x01
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x02 0x02
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x03 0x03
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x04 0x04
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x05 0x05
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x06 0x06
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x07 0x07
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x08 0x08
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x09 0x09
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0A 0x0A
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0B 0x0B
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0C 0x0C
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0D 0x0D
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0E 0x0E
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0F 0x0F
	usleep ${SLEEP_INTERVAL_MICRO}

	# Define the expected output of the i2cdump here:
	echo "" > ${TEST_TEMP_FOLDER}/${TEST_TEMP_GOLDEN}
	echo " 0000|  00 01 02 03 04 05 06 07   08 09 0a 0b 0c 0d 0e 0f " >> ${TEST_TEMP_FOLDER}/${TEST_TEMP_GOLDEN}
	echo "" >> ${TEST_TEMP_FOLDER}/${TEST_TEMP_GOLDEN}

	# Capture fresh test results to a target file.
	eeprog -xf /dev/i2c-${BUS} ${SLAVE} -8 -r 0x00:0x10 > ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}

	sleep ${SLEEP_INTERVAL_NORMAL}
	sync

    echo " "
	echo "******************************************************************"
    echo "***                                                            ***"                

	# Compare the target test results with the 'golden' results.  If the files 
	# match identically, the return code from diff is 0 and the test passes.
	diff -q ${TEST_TEMP_FOLDER}/${TEST_TEMP_GOLDEN} ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET} > /dev/null

	# Capture the return code from diff as the test result.
	IIC_EEPROM_TEST_RESULT=$?

	# Clear the test pattern from the EEPROM to leave no trace of this test.
	i2cset -y ${BUS} ${SLAVE} 0x00 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x01 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x02 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x03 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x04 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x05 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x06 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x07 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x08 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x09 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0A 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0B 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0C 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0D 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0E 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}
	i2cset -y ${BUS} ${SLAVE} 0x0F 0xFF
	usleep ${SLEEP_INTERVAL_MICRO}

	if [ ${IIC_EEPROM_TEST_RESULT} == "0" ];
	then
	  echo "*** I2C EEPROM (slave ${SLAVE}) Test:  PASS                        ***"
	else
	  echo "*** I2C EEPROM (slave ${SLAVE}) Test:  FAIL                        ***"
	fi

    echo "***                                                            ***"                
	echo "******************************************************************"
	echo " "

	# END HERE: All done with the testing.

	exit ${IIC_EEPROM_TEST_RESULT}

else
	echo "Valid programming file detected, programming EEPROM with data from ${INPUT}"

	# Iterate through each of the values in the programming source file and 
	# program each one into the EEPROM.
	MEMORY_ADDRESS=0
	MEMORY_LENGTH=0
	NEXT_VALUE=0
	
	# The while loop, read one group of 3 characters at a time.
	while IFS= read -r -n3 NEXT_VALUE
	do
		VALUE_STRING_SIZE=${#NEXT_VALUE}
		if [[ $VALUE_STRING_SIZE -gt 1 ]]; then
			i2cset -y ${BUS} ${SLAVE} ${MEMORY_ADDRESS} 0x${NEXT_VALUE}
			usleep ${SLEEP_INTERVAL_MICRO}
			echo "Writing ${NEXT_VALUE} to ${MEMORY_ADDRESS}"
			MEMORY_ADDRESS=$((MEMORY_ADDRESS+1))
		fi
	done < "$INPUT"

	MEMORY_LENGTH=${MEMORY_ADDRESS}
    echo " "
	echo "Programmed data from file into ${MEMORY_LENGTH} locations."	
    echo " "
	
	# Read all the programmed values back from the EEPROM and create a file
	# which can be used to diff with the sorce file to verify that programming
	# was successful.
	MEMORY_ADDRESS=0
	NEXT_VALUE=0
	
	# If there is a previous test result, it must have come from us so delete
	# it from the local file system.
	if [ -f ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET} ]; then
	    rm ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}
	fi
	
	touch ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}
	while [  $MEMORY_ADDRESS -lt $MEMORY_LENGTH ]; do
	    NEXT_VALUE="$(i2cget -y ${BUS} ${SLAVE} ${MEMORY_ADDRESS} | awk '{print toupper(substr($0,3))}')"
	    echo -n "${NEXT_VALUE} " >> ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}
	    echo "Read back ${NEXT_VALUE} from ${MEMORY_ADDRESS}"
	    MEMORY_ADDRESS=$((MEMORY_ADDRESS+1))
	done

	# Add the newline character to the end of the file to complete it.	
	echo "" >> ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}
	sleep ${SLEEP_INTERVAL_NORMAL}
	sync

	echo " "
	echo "******************************************************************"
    echo "***                                                            ***"                

	# Compare the target test results with the 'golden' results.  If the files 
	# match identically, the return code from diff is 0 and the test passes.
	diff -q ${INPUT} ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET} > /dev/null

	# Capture the return code from diff as the test result.
	IIC_EEPROM_TEST_RESULT=$?
	
	if [ ${IIC_EEPROM_TEST_RESULT} == "0" ];
	then
	  echo "*** I2C EEPROM (slave ${SLAVE}) Programming:  PASS                 ***"
	else
	  echo "*** I2C EEPROM (slave ${SLAVE}) Programming:  FAIL                 ***"
	fi

    echo "***                                                            ***"                
	echo "******************************************************************"
	echo " "

	# END HERE: All done with the programming.

	exit ${IIC_EEPROM_TEST_RESULT}
fi
