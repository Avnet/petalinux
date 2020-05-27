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
#  Create Date:         Mar 07, 2018
#  Design Name:         MAC ID Read from IIC EEPROM
#  Module Name:         eeprom-read-mac.sh
#  Project Name:        UltraZed IIC EEPROM Read MAC ID Script
#  Target Devices:      Xilinx Zynq and Zynq UltraScale+ MPSoC
#  Hardware Boards:     UltraZed-EG + I/O Carrier
#                       UltraZed-EG + PCIe Carrier
#                       UltraZed-EV + EV Carrier
#                       PicoZed + FMC2 Carrier
# 
#  Tool versions:       Xilinx Vivado 2017.4
# 
#  Description:         Read and Config Script for IIC EEPROM MAC ID
# 
#  Dependencies:        
#
#  Revision:            Mar 07, 2018: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/bin/sh

# The way this script works is to read the six EUI-48 bytes from the 
# specified I2C EEPROM and format the data into a string that is accepted by 
# Linux network configuration.
#
# According to the manufacturer datasheet, the six EUI-48 bytes are located
# at the top of the EEPROM memory space in addresses 0xFA to 0xFF:
#
# http://avnet.me/mac-id-eeprom-datasheet  
#
# The first three bytes are the Organizationally Unique Identifier (OUI) 
# assigned to the EEPROM manufacturer by the IEEE Registration Authority. 
# The remaining three bytes are the Extension Identifier, and are generated 
# by the EEPROM manufacturer to ensure a globally unique, 48-bit value which
# can be used in an embedded computing system as MAC address for a network
# interface controller (NIC).

# Configuration parameters which may be embedded Linux implementation specific
# depending on whether BusyBox or real Bash is used for command interpreter.

# This is the hexidecimal prefix that must be removed from the results of 
# using the 'i2cget' tool.
HEX_PREFIX=0x

# This is the delimiter character that must be inserted into the formatted MAC
# ID string that gets used by the Linux networking subsystem.
MAC_ID_DELIMITER=:

# Sleep interval used to synchonize accesses to the filesystem.
SLEEP_INTERVAL_NORMAL=1

# Default Ethernet interface that will be reconfigured.
ETHERNET_INTERFACE_NAME=eth0

# Default output file name is NULL.
OUTPUT_FILE=NULL

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
    OUTPUT_FILE="$2"
    shift # past argument
    ;;
    -i|--interface)
    ETHERNET_INTERFACE_NAME="$2"
    shift # past argument
    ;;    -s|--slave)
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

echo "Reading MAC ID from target EEPROM at ${SLAVE} on bus i2c-${BUS}"

# Read each of the six individual EUI-48 byte values from EEPROM memory 
# locations 0xFA to 0xFF and store as individual octets.
EEPROM_RAW_READ_STRING=$(i2cget -y ${BUS} ${SLAVE} 0xFA)
MAC_OCTET1=${EEPROM_RAW_READ_STRING#$HEX_PREFIX}

EEPROM_RAW_READ_STRING=$(i2cget -y ${BUS} ${SLAVE} 0xFB)
MAC_OCTET2=${EEPROM_RAW_READ_STRING#$HEX_PREFIX}

EEPROM_RAW_READ_STRING=$(i2cget -y ${BUS} ${SLAVE} 0xFC)
MAC_OCTET3=${EEPROM_RAW_READ_STRING#$HEX_PREFIX}

EEPROM_RAW_READ_STRING=$(i2cget -y ${BUS} ${SLAVE} 0xFD)
MAC_OCTET4=${EEPROM_RAW_READ_STRING#$HEX_PREFIX}

EEPROM_RAW_READ_STRING=$(i2cget -y ${BUS} ${SLAVE} 0xFE)
MAC_OCTET5=${EEPROM_RAW_READ_STRING#$HEX_PREFIX}

EEPROM_RAW_READ_STRING=$(i2cget -y ${BUS} ${SLAVE} 0xFF)
MAC_OCTET6=${EEPROM_RAW_READ_STRING#$HEX_PREFIX}

# Compute and display formatted MAC ID string.
RESULT_FORMATTED_MAC_STRING="$MAC_OCTET1$MAC_ID_DELIMITER$MAC_OCTET2$MAC_ID_DELIMITER$MAC_OCTET3$MAC_ID_DELIMITER$MAC_OCTET4$MAC_ID_DELIMITER$MAC_OCTET5$MAC_ID_DELIMITER$MAC_OCTET6"
echo "Retrieved MAC ID $RESULT_FORMATTED_MAC_STRING from target EEPROM"

# Make sure the output file exits else simply reconfig networking interface 
# and exit.
if [ ! -f ${OUTPUT_FILE} ]; then
    echo "$0: network interface configuration file $OUTPUT_FILE not found."
	
	# Bring down the networking interface to prepare for configuration.
	ifconfig ${ETHERNET_INTERFACE_NAME} down
	sleep ${SLEEP_INTERVAL_NORMAL}

	# Set the specified network interface configuration to use the new 
	# fomatted MAC ID.
	ifconfig ${ETHERNET_INTERFACE_NAME} hw ether ${RESULT_FORMATTED_MAC_STRING}
	sleep ${SLEEP_INTERVAL_NORMAL}

	# Bring up the networking interface to commit updated configuration.
	ifconfig ${ETHERNET_INTERFACE_NAME} up
	sleep ${SLEEP_INTERVAL_NORMAL}

	# Capture the return code from ifconfig as the script result.
	IIC_EEPROM_MAC_RESULT=$?
else
	echo "Networking interfaces configuration file $OUTPUT_FILENAME detected"
    echo "programming EEPROM MAC ID to ${ETHERNET_INTERFACE_NAME}"

	# Append the network configuration file with the new MAC address 
	# configuration.
	sed -i "/iface $ETHERNET_INTERFACE_NAME inet dhcp/a\ \ \ \ \ \ \ \ hwaddress ether $RESULT_FORMATTED_MAC_STRING" "${OUTPUT_FILE}"
	
	# Capture the return code from set as the script result.
	IIC_EEPROM_MAC_RESULT=$?
fi

# Return captured status code from above.
exit ${IIC_EEPROM_MAC_RESULT}
