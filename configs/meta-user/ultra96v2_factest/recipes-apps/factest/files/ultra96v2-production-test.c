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
//  Module Name:         ultra96v2-production-test.c
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
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>


#include "test_zcu100.h"
#include "typedef.h"

//following is defined to keep compiler happy, so that we can have common code between CE test and production test
sig_atomic_t volatile keep_threads_running = TRUE;

void main(int argc, char *argv[])
{
	int 			iteration_count = 0;	
	unsigned int 	test_loop_count = 0;	//TO DO = this will overflow in the infinite loop. it does not make sense to do testing for infinite...
	TEST_RESULT 	test_result = PASS;
	int				test_failed_count = 0;
	int				test_passed_count = 0;
	TEST_RESULT 	result = PASS;
	
	//disable system messages being redirected to console when test is going on...
//tc	system("dmesg -D"); //TO DO - Uncomment this in final code - Jignesh
   //https://askubuntu.com/questions/97256/how-do-i-disable-messages-or-logging-from-printing-on-the-console-virtual-termin
   system("dmesg -n 1"); //for busybox use of dmesg in the PetaLinux rootfs
   
	/*
	 * Print banner.
	 */
	USR_MSG("\n\r\n\t\t\t\tUltra96-v2 PRODUCTION TEST");
	USR_MSG("\n\r\t\t\t\tPerformed on %s\n\r\n", __DATE__);
	print_double_line();
	USR_MSG("USAGE: ./factest <1-255> (test loop count default = 1)\n");
	print_double_line();

	
	USR_MSG("\nPlease make sure that all the devices connected as required for Production Testing...\n");

	iteration_count = 1;
	if(argc > 1)	
	{
		iteration_count = atoi(argv[1]);
		if((1 >  iteration_count) || (iteration_count > 255) )
		{
			ERR_MSG("Invalid argument - %s, executing test for 1 time...\n",argv[1]);
			iteration_count = 1;
		}
	}
	else
	{
		USR_MSG("No arguments provided to command line, executing test for 1 time\n");
	}
	USR_MSG("\n");
	//NOTE: detect the USB devices before starting of the test. 
	//this wont allow the tester to add/remove usb device once the testing starts.
	//if wanted this can be added inside the loop.
	get_usb_devices_details();
	while(iteration_count > test_loop_count)
	{
		test_result = PASS;
		
		result = test_zcu100();
		test_result = (FAIL == result)? FAIL : test_result;
		
		result = test_zcu100_loopback();
		test_result = (FAIL == result)? FAIL : test_result;
		
		test_loop_count++;
		
		if(PASS == test_result)
			test_passed_count++;
		else	
			test_failed_count++;
		USR_MSG("\n\nTest Count - %d, Result = %s\n",test_loop_count,(PASS == test_result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");		
	}
	USR_MSG("\n\n");
	print_double_line();
	USR_MSG("\t\tTest Executed %d times, \tPassed %d times, \tFailed %d times\n",test_loop_count,test_passed_count, test_failed_count);
	print_double_line();
	
	//enable system messages being redirected to console when test is going on...
	//system("dmesg -E");
   system("dmesg -n 7"); //for busybox use of dmesg in the PetaLinux rootfs
	
	//to discard characters from the STDIO buffer...
	{	
		char c;
		while((c = getchar()) != '\n' && c != EOF)
			/* discard */ ;
	}
	USR_MSG("\n\n");
	print_double_line();	
//tc	USR_MSG("Production Test Completed, Shutting down board\n");
	USR_MSG("Production Test Completed.\n");
//tc	USR_MSG("Press any key to continue shutdown...\n");
	USR_MSG("Press any key to exit...\n");
	print_double_line();	

	getchar();	
//tc	system("shutdown -h now");	//TO DO - Uncomment this in final code - Jignesh
}
TEST_RESULT test_zcu100(void)
{
	TEST_RESULT test_result = PASS;
	TEST_RESULT result = PASS;

	USR_MSG	("\n");
	result = test_wlen_led();
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("WLAN LED Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	USR_MSG	("\n");
	result = test_bt_led();
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("BT LED Test Complete! \t\t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	USR_MSG	("\n");
	result = test_gpio_led();
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("GPIO LED Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

#if 0
	USR_MSG	("\n");
	//tricky to test as this switch is under the loopback board. so not feasible to press/releas switch
	result = test_gpio_sw();
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("GPIO Switch Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();
#endif

	USR_MSG	("\n");
	//need another board setup for this testing to do, another board will be configured as AP
	result = test_wifi();
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("Wi-Fi Test Complete! \t\t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	USR_MSG	("\n");
	result = test_bt();//able to scan device, not able to do ftp required for ce
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("Bluetooth Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	USR_MSG	("\n");
	result = test_sd();//Testing done, working fine.tested for production and working
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("SD Card Test Complete! \t\t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

#if 0
   //Since USB MSG will be used in loopback with USB3 Port 0, disable following code	
	//USB Mass Storage Gadget will be connected as loopback with this port, so need not to test seperately. 
	//if enabled, test will fail with USB MSG connected
	USR_MSG	("\n");
	result = test_usb(USB3_PORT_0);//Testing Done, working fine. 
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("USB3 Port 1 Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();
#endif

	USR_MSG	("\n");
	result = test_usb(USB3_PORT_1);//Testing Done, working fine.
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("USB3 Port 2 Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	USR_MSG	("\n");
	result = test_usb_upstream();//testing done (not through code though) using combination of software loopback and manual testing. 
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("USB Upstream Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	USR_MSG	("\n");
	result = test_dp();//verify stdout is displayed on the DisplayPort monitor
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("DisplayPort Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	return test_result;
}
TEST_RESULT test_zcu100_loopback(void)
{
	TEST_RESULT test_result = PASS;
	TEST_RESULT result = PASS;

	USR_MSG	("\n");
	result = test_gpio_loopback();
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("GPIO Loopback Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();
	
	USR_MSG	("\n");
	result = test_usb(USB2_PORT);
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("USB 2.0 Test Complete! \t\t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();
		
	USR_MSG	("\n");
	result = test_eeprom(LS_I2C_EEPROM_0);
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("I2C LS EEPROM 1 Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();
	
	USR_MSG	("\n");
	result = test_eeprom(LS_I2C_EEPROM_1);
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("I2C LS EEPROM 2 Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();
	
	USR_MSG	("\n");
	result = test_eeprom(HS_I2C_EEPROM_0);
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("I2C HS EEPROM 1 Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();
	
	USR_MSG	("\n");
	result = test_eeprom(HS_I2C_EEPROM_1);
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("I2C HS EEPROM 2 Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	USR_MSG	("\n");
	result = test_flash();
	test_result = (FAIL == result)? FAIL : test_result;
	print_single_line();
	USR_MSG("SPI Flash Test Complete! \t\tResult: \t%s\n",(PASS == result)?"\033[32mPASS\033[0m":"\033[31mFAIL\033[0m");
	print_single_line();

	return test_result;
}
