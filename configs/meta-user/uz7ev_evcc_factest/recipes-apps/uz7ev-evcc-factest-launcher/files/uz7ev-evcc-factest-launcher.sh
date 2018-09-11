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
#  Design Name:         Factory Test Launcher Script
#  Module Name:         uz7ev-evcc-factest-launcher.sh
#  Project Name:        Factory Test Launcher Script
#  Target Devices:      Xilinx Zynq and Zynq UltraScale+ MPSoC
#  Hardware Boards:     UltraZed-EG + I/O Carrier
#                       UltraZed-EV + EV Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
#                       Xilinx Vivado 2017.3
# 
#  Description:         Factory Test Launcher Script
# 
#  Dependencies:        
#
#  Revision:            Nov 28, 2016: 1.0 Initial version
#                       Jan 29, 2018: 1.1 Added option to infinitely loop
#                                         the SOM-only tests
# 
# ----------------------------------------------------------------------------
#!/bin/sh

FACTORY_TEST_SCRIPT=/home/root/uz7ev-evcc-factory-test.sh
FLASH_PROGRAMMING_SCRIPT=/home/root/uz-flash-programming.sh

# Show the factory test banner.
echo " "
echo "******************************************************************"
echo "***                                                            ***"
echo "***    Avnet SOM + Carrier Factory Test Launcher V1.5          ***"
echo "***                                                            ***"
echo "******************************************************************"
echo "***                                                            ***"
echo "***    Please Press Key to Perform Desired Function Below:     ***"
echo "***                                                            ***"
echo "***    'f' - Factory Test                                      ***"
echo "***    'i' - Run Factory Test in Infinite Loop                 ***"
echo "***    's' - Install Software to Flash Memory                  ***"
echo "***    'x' - Linux Command Prompt                              ***"
echo "***                                                            ***"
echo "***    NOTE: Waiting will AUTO-INITIATE Factory Test           ***"
echo "***                                                            ***"
echo "******************************************************************"
echo " "

while :
do
	answer="c"
	read -t 5 answer
	if [ "$answer" = "x" ]
	then
		echo " "
		echo "******************************************************************"
		echo "*** Exiting to Linux Command Prompt                            ***"
		echo "******************************************************************"
		echo " "
		break
	elif [ "$answer" = "f" ]
	then
		echo " "
		echo "******************************************************************"
		echo "*** Initiating Factory Test Suite                              ***"
		echo "******************************************************************"
		echo " "
		source ${FACTORY_TEST_SCRIPT}
		break
	elif [ "$answer" = "i" ]
	then
		echo " "
		echo "******************************************************************"
		echo "*** Initiating Looped Factory Test Suite                       ***"
		echo "******************************************************************"
		echo " "
		while true;
        do
          source ${FACTORY_TEST_SCRIPT}
        done
		break
	elif [ "$answer" = "s" ]
	then
		echo " "
		echo "******************************************************************"
		echo "*** Initiating Software Install to Flash Memory                ***"
		echo "******************************************************************"
		echo " "
		source ${FLASH_PROGRAMMING_SCRIPT}
		break

	else
		echo " "
		echo "******************************************************************"
		echo "*** Auto-Initiating Factory Test Suite                         ***"
		echo "******************************************************************"
		echo " "
		source ${FACTORY_TEST_SCRIPT}
		break
	fi
done
