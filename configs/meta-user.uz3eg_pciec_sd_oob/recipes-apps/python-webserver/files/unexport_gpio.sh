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
#  Design Name:         GPIO Sysfs Uninit Script
#  Module Name:         setup_gpio.sh
#  Project Name:        GPIO Sysfs Uninit Script
#  Target Devices:      Xilinx Zynq and Zynq UltraScale+ MPSoC
#  Hardware Boards:     UltraZed-EG + I/O Carrier
#                       UltraZed-EV + EV Carrier
# 
#  Tool versions:       Xilinx Vivado 2017.3
# 
#  Description:         GPIO Sysfs Uninit Script
# 
#  Dependencies:        
#
#  Revision:            Feb 22, 2018: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/bin/sh

LED_GPIO_BASE=496
LED0_BASE=$((LED_GPIO_BASE+0))
LED1_BASE=$((LED_GPIO_BASE+1))
LED2_BASE=$((LED_GPIO_BASE+2))
LED3_BASE=$((LED_GPIO_BASE+3))
LED4_BASE=$((LED_GPIO_BASE+4))
LED5_BASE=$((LED_GPIO_BASE+5))
LED6_BASE=$((LED_GPIO_BASE+6))
LED7_BASE=$((LED_GPIO_BASE+7))

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
echo ${LED0_BASE} > /sys/class/gpio/unexport

# PL LED1: 
echo ${LED1_BASE} > /sys/class/gpio/unexport

# PL LED2: 
echo ${LED2_BASE} > /sys/class/gpio/unexport

# PL LED3: 
echo ${LED3_BASE} > /sys/class/gpio/unexport

# PL LED4: 
echo ${LED4_BASE} > /sys/class/gpio/unexport

# PL LED5: 
echo ${LED5_BASE} > /sys/class/gpio/unexport

# PL LED6: 
echo ${LED6_BASE} > /sys/class/gpio/unexport

# PL LED7: 
echo ${LED7_BASE} > /sys/class/gpio/unexport

## PL SW0:
#echo ${SW0_BASE} > /sys/class/gpio/unexport

## PL SW1:
#echo ${SW1_BASE} > /sys/class/gpio/unexport

## PL SW2:
#echo ${SW2_BASE} > /sys/class/gpio/unexport

## PL SW3:
#echo ${SW3_BASE} > /sys/class/gpio/unexport

## PL SW4:
#echo ${SW4_BASE} > /sys/class/gpio/unexport

## PL SW5:
#echo ${SW5_BASE} > /sys/class/gpio/unexport

## PL SW6:
#echo ${SW6_BASE} > /sys/class/gpio/unexport

## PL SW7:
#echo ${SW7_BASE} > /sys/class/gpio/unexport

