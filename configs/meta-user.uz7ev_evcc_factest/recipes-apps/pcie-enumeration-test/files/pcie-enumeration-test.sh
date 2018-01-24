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
#  Create Date:         Jan 19, 2018
#  Design Name:         PCIe Enumeration Test Script
#  Module Name:         pcie-enumeration-test.sh
#  Project Name:        UltraZed-EV PCIe Enumeration
#  Target Devices:      Xilinx Zynq UltraScale+ MPSoC
#  Hardware Boards:     UltraZed-EV + EV Carrier
# 
#  Tool versions:       Xilinx Vivado 2017.2
#                       Xilinx Vivado 2017.3
# 
#  Description:         Test Script for UltraZed-EV PCIe Enumeration
# 
#  Dependencies:        
#
#  Revision:            Jan 19, 2018: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# Sleep interval between tests.
SLEEP_INTERVAL_MICRO=4000
SLEEP_INTERVAL_NORMAL=1

# Setup variable to store the test results into.
PCIE_ENUMERATION_TEST_RESULT=-1

sleep ${SLEEP_INTERVAL_NORMAL}
sync

# Grep the dmesg output to detect of the Broadcom TIGON3 Ethernet NIC has been installed
# This verifies that the PCIe interface is configured correctly
dmesg | grep "eth1: Tigon3" > /dev/null
PCIE_ENUMERATION_TEST_RESULT=$?
  
echo " "
echo "******************************************************************"
echo "***                                                            ***"

if [ ${PCIE_ENUMERATION_TEST_RESULT} == "0" ];
then
  echo "*** PCIe Enumeration Test:  PASS                               ***"
else
  echo "*** PCIe Enumeration Test:  FAIL                               ***"
fi

echo "***                                                            ***"                
echo "******************************************************************"
echo " "

# END HERE: All done with the testing.

exit ${PCIE_ENUMERATION_TEST_RESULT}


