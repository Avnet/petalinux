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
#  Design Name:         UltraZed + I/O Carrier
#  Module Name:         linux_iic_gpio_expander_test.sh
#  Project Name:        UltraZed IIC GPIO Expander Test Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed + I/O Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         Test Script for UltraZed IIC GPIO Expander
# 
#  Dependencies:        
#
#  Revision:            Nov 28, 2016: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/bin/sh

# Sleep interval between tests.
SLEEP_INTERVAL_NORMAL=1

# Information used to execute comparison test between expected results
# and actual read results from target I2C device.
TEST_TEMP_FOLDER=/tmp
TEST_TEMP_GOLDEN=gpio_expander_golden.txt
TEST_TEMP_TARGET=gpio_expander_test.txt

# Setup variable to store the test results into.
IIC_GPIO_EXPANDER_TEST_RESULT=-1

# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# Define the expected output of the i2cdump here:
echo "0xdf" > ${TEST_TEMP_FOLDER}/${TEST_TEMP_GOLDEN}

# Capture fresh test results to a target file.
i2cget -y 0 0x20 0x00 b > ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}

sleep ${SLEEP_INTERVAL_NORMAL}
sync

echo "******************************************************************"
echo "***                                                            ***"

# Compare the target test results with the 'golden' results.  If the files 
# match identically, the return code from diff is 0 and the test passes.
diff -q ${TEST_TEMP_FOLDER}/${TEST_TEMP_GOLDEN} ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET} > /dev/null

# Capture the return code from diff as the test result.
IIC_GPIO_EXPANDER_TEST_RESULT=$?

if [ ${IIC_GPIO_EXPANDER_TEST_RESULT} == "0" ];
then
  echo "*** I2C GPIO expander Test:  PASS                              ***"
else
  echo "*** I2C GPIO expander Test:  FAIL                              ***"
fi

echo "***                                                            ***"
echo "******************************************************************"
echo " "

# END HERE: All done with the testing.

exit ${IIC_GPIO_EXPANDER_TEST_RESULT}
