//        ** **        **          **  ****      **  **********  **********
//       **   **        **        **   ** **     **  **              **
//      **     **        **      **    **  **    **  **              **
//     **       **        **    **     **   **   **  *********       **
//    **         **        **  **      **    **  **  **              **
//   **           **        ****       **     ** **  **              **
//  **  .........  **        **        **      ****  **********      **
//     ...........
//                                     Reach Further
//
// ----------------------------------------------------------------------------
// 
//  This design is the property of Avnet.  Publication of this
//  design is not authorized without written consent from Avnet.
// 
//  Please direct any questions to the Ultra96 community support forum:
//     http://avnet.me/zed-forums	
// 
//  Product information is available at:
//     http://avnet.me/ultra96v2	
// 
//  Disclaimer:
//     Avnet, Inc. makes no warranty for the use of this code or design.
//     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//     any errors, which may appear in this code, nor does it make a commitment
//     to update the information contained herein. Avnet, Inc specifically
//     disclaims any implied warranties of fitness for a particular purpose.
//                      Copyright(c) 2016 Avnet, Inc.
//                              All rights reserved.
// 
// ----------------------------------------------------------------------------
// 
//  Create Date:         Jul 22, 2017
//  Design Name:         Ultra96-V2
//  Module Name:         test_zcu100.h
//  Project Name:        Ultra96-V2 production test application
//  Target Devices:      Xilinx Zynq UltraScale+ MPSoC
//  Hardware Boards:     Ultra96-V2
// 
//  Tool versions:       Xilinx Vivado 2017.4
// 
//  Description:         Main Ultra96-V2 production test application 
// 
//  Dependencies:        
//
//  Revision:            Jul 22, 2017: 1.0 Initial version
//                       created by Xilinx for Ultra96-V1 board
//                       Mar 28, 2019: 1.1 Updated for Ultra96-V2 board
// 
// ----------------------------------------------------------------------------
#ifndef _test_zcu100_h_
#define _test_zcu100_h_

#include "typedef.h"

TEST_RESULT test_zcu100(void);

TEST_RESULT test_zcu100_loopback(void);

TEST_RESULT check_user_response(void);

void print_single_line(void);

void print_double_line(void);


void * thread_wlen_led_function(void*);

void * thread_bt_led_function(void*);

void * thread_gpio_led_function(void*);

void * thread_gpio_sw_function(void*);

void * thread_wifi_function(void*);

void * thread_bt_function(void*);

void * thread_sd_function(void*);

void * thread_usb_function(void*);

void * thread_usb_upstream_function(void*);

void * thread_eeprom_function(void*);

void * thread_flash_function(void*);

void * thread_gpio_loopback_function(void*);

void * thread_dp_function(void*);

void * thread_ina226_function(void*);

void * thread_pmic_function(void*);


TEST_RESULT test_wlen_led(void);

TEST_RESULT test_bt_led(void);

TEST_RESULT test_gpio_led(void);

TEST_RESULT test_gpio_sw(void);

TEST_RESULT test_wifi(void);

TEST_RESULT test_bt(void);

TEST_RESULT test_sd(void);

TEST_RESULT test_usb(USB_PORTS);

TEST_RESULT test_usb_upstream(void);

TEST_RESULT test_eeprom(I2C_EEPROM);

TEST_RESULT test_gpio_loopback(void);

TEST_RESULT test_dp(void);

TEST_RESULT test_ina226(void);

TEST_RESULT test_pmic(void);

TEST_RESULT test_flash(void);

void get_usb_devices_details(void);

#endif//_test_zcu100_h_
