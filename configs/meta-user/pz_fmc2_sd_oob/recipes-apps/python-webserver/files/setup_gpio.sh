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
#  Create Date:         Feb 22, 2018
#  Design Name:         GPIO Sysfs Init Script
#  Module Name:         setup_gpio.sh
#  Project Name:        GPIO Sysfs Init Script
#  Target Devices:      Xilinx Zynq and Zynq UltraScale+ MPSoC
#  Hardware Boards:     UltraZed-EG + I/O Carrier
#                       UltraZed-EG + PCIe Carrier
#                       UltraZed-EV + EV Carrier
#                       PicoZed + FMC2 Carrier
# 
#  Tool versions:       Xilinx Vivado 2017.3
# 
#  Description:         GPIO Sysfs Init Script
# 
#  Dependencies:        
#
#  Revision:            Feb 22, 2018: 1.0 Initial version
#                       Mar 22, 2018: 1.1 Updated for PicoZed and
#                                         PetaLinux 2017.4
# 
# ----------------------------------------------------------------------------
#!/bin/sh

###
# For the PicoZed 7010, because there is only on PL GPIO LED for this SOM,
# the PL LED is mapped to GPIO 1023. This will light up LED D6 on the FMC2 Carrier
# when this SOM is installed.
#
# For the PicoZed 7015-7030 there are 4 PL GPIO LEDs with the first LED mapped to GPIO 1020.
# This means that GPIO 1023 will light up the last LED (D9) on the FMC2 Carrier when any of
# these SOMs are instsalled.
###

LED_GPIO_BASE=1020
#LED0_BASE=$((LED_GPIO_BASE+0))
#LED1_BASE=$((LED_GPIO_BASE+1))
#LED2_BASE=$((LED_GPIO_BASE+2))
LED3_BASE=$((LED_GPIO_BASE+3))
#LED4_BASE=$((LED_GPIO_BASE+4))
#LED5_BASE=$((LED_GPIO_BASE+5))
#LED6_BASE=$((LED_GPIO_BASE+6))
#LED7_BASE=$((LED_GPIO_BASE+7))

#SW_GPIO_BASE=504
#SW0_BASE=$((SW_GPIO_BASE+0))
#SW1_BASE=$((SW_GPIO_BASE+1))
#SW2_BASE=$((SW_GPIO_BASE+2))
#SW3_BASE=$((SW_GPIO_BASE+3))
#SW4_BASE=$((SW_GPIO_BASE+4))
#SW5_BASE=$((SW_GPIO_BASE+5))
#SW6_BASE=$((SW_GPIO_BASE+6))
#SW7_BASE=$((SW_GPIO_BASE+7))

# PL LED0: 
#echo ${LED0_BASE} > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio${LED0_BASE}/direction
#echo 1 > /sys/class/gpio/gpio${LED0_BASE}/value

# PL LED1: 
#echo ${LED1_BASE} > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio${LED1_BASE}/direction
#echo 1 > /sys/class/gpio/gpio${LED1_BASE}/value

# PL LED2: 
#echo ${LED2_BASE} > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio${LED2_BASE}/direction
#echo 1 > /sys/class/gpio/gpio${LED2_BASE}/value

# PL LED3: 
echo ${LED3_BASE} > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio${LED3_BASE}/direction
echo 1 > /sys/class/gpio/gpio${LED3_BASE}/value

## PL LED4: 
#echo ${LED4_BASE} > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio${LED4_BASE}/direction
#echo 1 > /sys/class/gpio/gpio${LED4_BASE}/value

## PL LED5: 
#echo ${LED5_BASE} > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio${LED5_BASE}/direction
#echo 1 > /sys/class/gpio/gpio${LED5_BASE}/value

## PL LED6: 
#echo ${LED6_BASE} > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio${LED6_BASE}/direction
#echo 1 > /sys/class/gpio/gpio${LED6_BASE}/value

## PL LED7: 
#echo ${LED7_BASE} > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio${LED7_BASE}/direction
#echo 1 > /sys/class/gpio/gpio${LED7_BASE}/value

## PL SW0:
#echo ${SW0_BASE} > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio${SW0_BASE}/direction

## PL SW1:
#echo ${SW1_BASE} > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio${SW1_BASE}/direction

## PL SW2:
#echo ${SW2_BASE} > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio${SW2_BASE}/direction

## PL SW3:
#echo ${SW3_BASE} > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio${SW3_BASE}/direction

## PL SW4:
#echo ${SW4_BASE} > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio${SW4_BASE}/direction

## PL SW5:
#echo ${SW5_BASE} > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio${SW5_BASE}/direction

## PL SW6:
#echo ${SW6_BASE} > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio${SW6_BASE}/direction

## PL SW7:
#echo ${SW7_BASE} > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio${SW7_BASE}/direction

