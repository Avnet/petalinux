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
//  Module Name:         test_gpio.c
//  Project Name:        Ultra96-V2 GPIO loopback test application
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
// Figuring out the exact GPIO was not totally obvious when there
// were multiple GPIOs in the system. One way to do is to go into
// the gpiochips in /sys/class/gpio and view the label as it should
// reflect the address of the GPIO in the system. The name of the
// the chip appears to be the 1st GPIO of the controller.
//
// The export causes the gpioX dir to appear in /sys/class/gpio.
// Then the direction and value can be changed by writing to them.
 
// The performance of this is pretty good, using a nfs mount,
// running on open source linux, on the ML507 reference system,
// the GPIO can be toggled about every 4 usec.
 
// The following commands from the console setup the GPIO to be
// exported, set the direction of it to an output and write a 1
// to the GPIO.
//
// bash> echo 240 > /sys/class/gpio/export
// bash> echo out > /sys/class/gpio/gpio240/direction
// bash> echo 1 > /sys/class/gpio/gpio240/value
 
// if sysfs is not mounted on your system, the you need to mount it
// bash> mount -t sysfs sysfs /sys
 
// the following bash script to toggle the gpio is also handy for
// testing
//
// while [ 1 ]; do
//  echo 1 > /sys/class/gpio/gpio240/value
//  echo 0 > /sys/class/gpio/gpio240/value
// done
 
// to compile this, use the following command
// gcc gpio.c -o gpio
 
// The kernel needs the following configuration to make this work.
//
// CONFIG_GPIO_SYSFS=y
// CONFIG_SYSFS=y
// CONFIG_EXPERIMENTAL=y
// CONFIG_GPIO_XILINX=y
 

#include <stdio.h>
#include <unistd.h>

#include "test_zcu100.h"
#include "typedef.h"

typedef enum _GPIO_DIRECTION_IN_OUT
{
	GPIO_DIRECTION_IN,
	GPIO_DIRECTION_OUT
}GPIO_DIRECTION_IN_OUT;
const char *GPIO_DIRECTION_IN_OUT_STRING[] = {"in","out"};

typedef enum _GPIO_ACTION_LED_OFF_0_ON_1
{
	GPIO_ACTION_LED_OFF_0	= 0,
	GPIO_ACTION_LED_ON_1 	= 1
}GPIO_ACTION_LED_OFF_0_ON_1;

//GPIO Pins #17,18,19,20 are LEDs, Reference ZCU100-PinAssignments_20170202snapshot.xlsx
unsigned int led_gpio_number[NUMBER_OF_GPIO_LEDS] = {17,18,19,20};//338+17,338+18,338+19,338+20

//GPIO Pin #23 is PB (Push Button), Reference ZCU100-PinAssignments_20170202snapshot.xlsx
unsigned int gpio_switch_pin_number = 23; //338+23

GPIO_DIRECTION_IN_OUT gpio_loopback_pins_direction[] ={GPIO_DIRECTION_IN,GPIO_DIRECTION_OUT};

//change the number of pins for loopback testing. include in and out both the pins.
#define NUMBER_OF_GPIO_PINS_FOR_LOOPBACK 	(sizeof(gpio_number_loopback)/sizeof(unsigned char))

//change pin number that would be part of GPIO loopback. put in/out in sequence which would be configured as loopback
unsigned char gpio_number_loopback[] = {
	/******** 	MIO Pins 	   ********/
	36, 	/*IN 	- 374, MIO36_PS_GPIO_0 */
	39, 	/*OUT 	- 377, MIO39_PS_GPIO1_2 */
	
	37, 	/*IN 	- 375, MIO37_PS_GPIO1_1 */
	40, 	/*OUT 	- 378, MIO40_PS_GPIO1_3 */

#if 0
	/*
		For the Rev B Following pins fail, so commented those
	*/
	38, 	/*IN 	- 376, MIO38_SPI0_SLCK */
	42, 	/*OUT 	- 380, MIO42_SPI_MISO */
	
	41, 	/*IN 	- 379, MIO41_SPI0_CS */
	43, 	/*OUT 	- 381, MIO43_SPI0_MOSI */
#endif
	/******** 	EMIO Pins 	   ********/
	
	101, 	/*IN 	- 439, HD_GPIO_0 */
	78, 	/*OUT 	- 416, HD_GPIO_1 */

	102, 	/*IN 	- 440, HD_GPIO_2 */
	79, 	/*OUT 	- 417, HD_GPIO_3 */

	103, 	/*IN 	- 441, HD_GPIO_4 */
	80, 	/*OUT 	- 418, HD_GPIO_5 */
	
	104, 	/*IN 	- 442, HD_GPIO_9 */
	81, 	/*OUT 	- 419, HD_GPIO_10 */

	105, 	/*IN 	- 443, HD_GPIO_11 */
	82, 	/*OUT 	- 420, HD_GPIO_12 */
	
	106, 	/*IN 	- 444, HD_GPIO_14 */
	83, 	/*OUT 	- 421, HD_GPIO_15 */
		
	/******** 	High Speed 	   ********/
	107, 	/*IN 	- 445, CSI0_MCLK */
	84, 	/*OUT 	- 422, DSI_CLK_N */
	
	108, 	/*IN 	- 446, CSI1_MCLK */
	85, 	/*OUT 	- 423, DSI_CLK_P */
	
	109, 	/*IN 	- 447, DSI_D0_P */
	86, 	/*OUT 	- 424, DSI_D1_P */
	
	110, 	/*IN 	- 448, DSI_D0_N */
	87, 	/*OUT 	- 425, DSI_D1_N */
	
	111, 	/*IN 	- 449, DSI_D2_P */
	88, 	/*OUT 	- 426, DSI_D3_P */
	
	112, 	/*IN 	- 450, DSI_D2_N */
	89, 	/*OUT 	- 427, DSI_D3_N */
	
	113, 	/*IN 	- 451, CSI0_C_P */
	90, 	/*OUT 	- 428, CSI1_C_P */
	
	114, 	/*IN 	- 452, CSI0_C_N */
	91, 	/*OUT 	- 429, CSI1_C_N */
	
	115, 	/*IN 	- 453, CSI0_D0_P */
	92, 	/*OUT 	- 430, CSI1_D0_P */
	
	116, 	/*IN 	- 454, CSI0_D0_N */
	93, 	/*OUT 	- 431, CSI1_D0_N */

	117, 	/*IN 	- 455, CSI0_D1_P */	
	94, 	/*OUT 	- 432, CSI1_D1_P */
	
	118, 	/*IN 	- 456, CSI0_D1_N */
	95, 	/*OUT 	- 433, CSI1_D1_N */
	
	119, 	/*IN 	- 457, CSI0_D2_P */
	96, 	/*OUT 	- 434, CSI1_D3_P */
	
	120, 	/*IN 	- 458, CSI0_D2_N */
	97, 	/*OUT 	- 435, CSI1_D3_N */
	
	121, 	/*IN 	- 459, HSIC_STR */
	98, 	/*OUT 	- 436, HSIC_DATA */
	
	/******** 	MIO + EMIO Pins ********/
	44, 	/*IN 	- 382, MIO44_PS_GPIO1_4 */
	99, 	/*OUT 	- 437, HD_GPIO_6 */

	45, 	/*IN 	- 383, MIO45_PS_GPIO1_5 */
	100, 	/*OUT 	- 438, HD_GPIO_13 */
};

TEST_RESULT write_to_gpio_pin(int,GPIO_ACTION_LED_OFF_0_ON_1);

TEST_RESULT read_from_gpio(int gpio_pin_number,char *sw_value)
{
	FILE *valuefp = NULL;
	char file_name[COMMAND_LENGTH];
	
	
    // Get the GPIO value ready to be toggled
	sprintf(file_name,"/sys/class/gpio/gpio%d/value",gpio_pin_number);
    valuefp = fopen(file_name, "r");
    if (!valuefp)
    {
        ERR_MSG("Cannot open GPIO value for reading\n");
        return FAIL;
    }
	
	//read the switch value
	fscanf(valuefp,"%s",sw_value);
	
	//DBG_MSG("GPIO value opened, value read - %s...\n",sw_value);
	DBG_MSG("GPIO Pin # %d : Read - %s\n",gpio_pin_number,sw_value);
	
	fclose(valuefp);
	
	return PASS;
}	

/*
 An important note you have to keep in mind if you plan to set or, more important, get the value of a GPIO through 
 this way in continous mode. If you open the "value" file for get the current GPIO status (1 or 0) remember that, 
 after the fist read operation, the file pointer will move to the next position in the file. Since this interface was 
 made to be read from cat command the returned string will be terminated by the new line character (\n). This mean 
 after the first "valid" read all the next read operation will return always the last character in the file, in this 
 case only the new line '\n'. For obtain a correct status value for each read operation you simply have to set the 
 file pointer at the beginning of the file before read by using the command below:
				lseek(fp, 0, SEEK_SET);
				
You will not have this problem if you open and close GPIO value file every time you need to read but, as you can know, 
for continuous read introduce a short delay. Since these short lines of codes are only an example if you want to use them 
in your code remember add the control for error in open GPIO file.
 */
TEST_RESULT write_to_gpio_pin(int gpio_pin_number,GPIO_ACTION_LED_OFF_0_ON_1 gpio_pin_action)
{
	FILE*	valuefp;
	char file_name[COMMAND_LENGTH];
	
	// Get the GPIO value ready to be toggled
	sprintf(file_name,"/sys/class/gpio/gpio%d/value",gpio_pin_number);
	valuefp = fopen(file_name, "w");
	if (!valuefp)
	{
		ERR_MSG("Cannot open GPIO value\n");
		return FAIL;
	}

	//DBG_MSG("GPIO value opened, now turning %s...\n",ON_OFF_ACTION_DISPLAY_STRING[gpio_pin_action]);
	DBG_MSG("GPIO Pin # %d : Writing - %s\n",gpio_pin_number,GPIO_ACTION_LED_OFF_0_ON_1_STRING[gpio_pin_action]);

	//perform the action on the GPIO LED - on or off
	fprintf(valuefp,"%s",GPIO_ACTION_LED_OFF_0_ON_1_STRING[gpio_pin_action]);
	 
	fclose (valuefp);
	
	return PASS;
}

TEST_RESULT test_gpio_sw(void)
{//switch value 0 means pressed and 1 means released
    FILE* 		exportfp;
	FILE* 		directionfp;
	FILE* 		unexportfp;
	char 		sw_value[10];
	TEST_RESULT test_result;
	char 		gpio_number_string[10];
	char file_name[COMMAND_LENGTH];
	
	//print message for the user to tell to press/ release the gpio switch
	USR_MSG("Testing GPIO Switch - %d\n",gpio_switch_pin_number);
	
	
	//check for gpio SW state
	sprintf(gpio_number_string,"%d",ZCU100_GPIO_BASE + gpio_switch_pin_number);

	// The GPIO has to be exported to be able to see it in sysfs
	exportfp = fopen("/sys/class/gpio/export", "w");
	if (!exportfp)
	{
		ERR_MSG("Cannot open GPIO to export it\n");
		return FAIL;
	}	

	fprintf(exportfp, "%s",gpio_number_string);

	fclose(exportfp);
 
	DBG_MSG("GPIO exported successfully\n");
		
	// Update the direction of the GPIO to be an input only. note that directionfp should be writable	
	sprintf(file_name,"/sys/class/gpio/gpio%d/direction",ZCU100_GPIO_BASE + gpio_switch_pin_number);	
	directionfp = fopen(file_name, "w");
	if (!directionfp)
	{
		ERR_MSG("Cannot open GPIO to direction it - /sys/class/gpio/gpio%d/direction\n",gpio_switch_pin_number);
		return FAIL;
	}
	
	//set the direction of the GPIO pin as in
	fprintf(directionfp, "%s",GPIO_DIRECTION_IN_OUT_STRING[GPIO_DIRECTION_IN]);
	fclose(directionfp);	 
	DBG_MSG("GPIO direction set as input successfully\n");

	USR_MSG("Keep the GPIO Switch pressed...\n");
	sleep(GPIO_SW_PRESSED_RELEASE_DURATION_SEC);//give sometime to tester to press the switch before checking for gpio value.		
	test_result = read_from_gpio(ZCU100_GPIO_BASE + gpio_switch_pin_number,sw_value);
	if(FAIL == test_result) return FAIL;
	USR_MSG("Switch values is %s, switch is %s\n",sw_value,(!strcmp(sw_value,"1")?"Released":"Pressed"));
	
	
	USR_MSG("Release the GPIO Switch...\n");
	sleep(GPIO_SW_PRESSED_RELEASE_DURATION_SEC);//give sometime to tester to release the switch before checking for gpio value.		
	test_result = read_from_gpio(ZCU100_GPIO_BASE + gpio_switch_pin_number,sw_value);	
	if(FAIL == test_result) return FAIL;
	USR_MSG("Switch values is %s, switch is %s\n",sw_value,(!strcmp(sw_value,"1")?"Released":"Pressed"));
		
	unexportfp = fopen("/sys/class/gpio/unexport", "w");
	fprintf(unexportfp,"%s", gpio_number_string);
	fclose(unexportfp); 
 }
 
 TEST_RESULT test_gpio_led(void)
 { 
    FILE* 	exportfp;
	FILE* 	directionfp;
	FILE* 	unexportfp;
	char 	gpio_number_string [10];
	int 	loop_count;
	char 	file_name[COMMAND_LENGTH];
	char 	str_ask_user[COMMAND_LENGTH];	
	TEST_RESULT test_result;
	TEST_RESULT final_test_result;
	
    DBG_MSG("GPIO test running...\n");
 
	test_result = final_test_result = PASS;
	
	for(loop_count = 0; loop_count < NUMBER_OF_GPIO_LEDS; loop_count++)
	{
		DBG_MSG("Testing GPIO LED - %d\n",led_gpio_number[loop_count]);
		sprintf(gpio_number_string,"%d",ZCU100_GPIO_BASE + led_gpio_number[loop_count]);
		
		// The GPIO has to be exported to be able to see it in sysfs
		exportfp = fopen("/sys/class/gpio/export", "w");
		if (!exportfp)
		{
			ERR_MSG("Cannot open GPIO to export it\n");
			return FAIL;
		}
		
		fprintf(exportfp, "%s",gpio_number_string);
		fclose(exportfp);
	 
		DBG_MSG("GPIO exported successfully\n");
		
		// Update the direction of the GPIO to be an output
		sprintf(file_name,"/sys/class/gpio/gpio%d/direction",ZCU100_GPIO_BASE + led_gpio_number[loop_count]);
		directionfp = fopen(file_name, "w");
		if (!directionfp)
		{
			ERR_MSG("Cannot open GPIO to direction it\n");
			return FAIL;
		}
		
		//set the direction of the led as out
		fprintf(directionfp, "%s",GPIO_DIRECTION_IN_OUT_STRING[GPIO_DIRECTION_OUT]);
		fclose(directionfp);	 
		DBG_MSG("GPIO direction set as output successfully\n");

		DBG_MSG("Turning ON GPIO LED\n");
		write_to_gpio_pin(ZCU100_GPIO_BASE + led_gpio_number[loop_count],GPIO_ACTION_LED_ON_1);					
	}
#ifndef CE_TEST	
	sprintf(str_ask_user,"Are all four GPIO LEDs ON? Press 'y' (for ON) or 'n'\t:\t");
	USR_MSG(str_ask_user);	
	test_result = check_user_response();
#else
	usleep(THREAD_SLEEP_USEC*10);
#endif//#ifndef CE_TEST	

	/* Turn All Four GPIO LEDs OFF */
	for(loop_count = 0; loop_count < NUMBER_OF_GPIO_LEDS; loop_count++)
	{		
		DBG_MSG("Turning OFF GPIO LED\n");
		write_to_gpio_pin(ZCU100_GPIO_BASE + led_gpio_number[loop_count],GPIO_ACTION_LED_OFF_0);		
	}
#ifndef CE_TEST	
	sprintf(str_ask_user,"Are all four GPIO LEDs OFF? Press 'y' (for OFF) or 'n'\t:\t");
	USR_MSG(str_ask_user);
	test_result = check_user_response();
#else
	usleep(THREAD_SLEEP_USEC*10);	
#endif//#ifndef CE_TEST		

	for(loop_count = 0; loop_count < NUMBER_OF_GPIO_LEDS; loop_count++)
	{
		unexportfp = fopen("/sys/class/gpio/unexport", "w");
		if(!unexportfp) 		
		{
			ERR_MSG("Cannot open GPIO to unexport\n");
			return FAIL;
		}
		
		fprintf(unexportfp, "%s",gpio_number_string);
		fclose(unexportfp);
		if(FAIL == test_result ) 
			final_test_result =  FAIL;
	}
	return final_test_result;
 }
 
TEST_RESULT open_gpio_for_loopback(void)
{
	FILE* 		exportfp;
	FILE* 		directionfp;
	int 		loop_count;
	char 		file_name[COMMAND_LENGTH];
	char		in_direction;			
	char 		gpio_number_string [COMMAND_LENGTH];
	for(loop_count = 0; loop_count < NUMBER_OF_GPIO_PINS_FOR_LOOPBACK; loop_count++)
	{
		DBG_MSG("Configuring GPIO Pin #%d for GPIO Lookback test\n",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count]);
		sprintf(gpio_number_string,"%d",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count]);
		
		// The GPIO has to be exported to be able to see it in sysfs
		exportfp = fopen("/sys/class/gpio/export", "w");
		if (!exportfp)
		{
			ERR_MSG("Cannot open GPIO to export it\n");
			return FAIL;
		}
		fprintf(exportfp, "%s",gpio_number_string);
		fclose(exportfp);
	 
		DBG_MSG("GPIO exported successfully\n");
		
		// Update the direction of the GPIO to be input or output
		sprintf(file_name,"/sys/class/gpio/gpio%d/direction",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count]);
		directionfp = fopen(file_name, "w");
		if (!directionfp)
		{
			ERR_MSG("Cannot open GPIO to direction it\n");
			return FAIL;
		}
		
		//set the direction of the gpio as in or out
		in_direction = ( (0 == (loop_count % 2) ) ? 0 : 1); //if number is even then it means, it should be set for out, so mark in_direction as 0
		fprintf(directionfp, "%s",GPIO_DIRECTION_IN_OUT_STRING[in_direction]);//if in_direction is 0 or false, it should be set to out
		fclose(directionfp);	 
		DBG_MSG("GPIO direction set as %s successfully\n",GPIO_DIRECTION_IN_OUT_STRING[in_direction]);
	}	
	return PASS;
}	
void close_gpio_for_loopback(void)
{
	FILE*		unexportfp;
	int 		loop_count;
	char 		gpio_number_string [COMMAND_LENGTH];
	
	//unexport all GPIOs are completion of testing
	for(loop_count = 0; loop_count < NUMBER_OF_GPIO_PINS_FOR_LOOPBACK; loop_count++)
	{
		DBG_MSG("unexporting GPIO Pin # - %d at the end of GPIO Lookback test\n",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count]);
		sprintf(gpio_number_string,"%d",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count]);
		
		unexportfp = fopen("/sys/class/gpio/unexport", "w");
		fprintf(unexportfp, "%s",gpio_number_string);
		fclose(unexportfp);
	}
}
TEST_RESULT read_write_gpio_loopback(void)
{
	int loop_count = 0;
	int test_value_loop = 0;
	char read_value[10];
	int write_value = 0;	
	TEST_RESULT test_result = PASS;
	TEST_RESULT final_test_result = PASS;
	char	loopback_over = 0;
	
	//Once all GPIOs are set appropriately in or out, perform GPIO loopback by writing and reading from GPIO pairs one by one
	for(loop_count = 0; loop_count < NUMBER_OF_GPIO_PINS_FOR_LOOPBACK; loop_count += 2)
	{//loop this for half the time as number of pins for loopback are half configured for write and other half for read. 
	//write and read happens in the same loop for loopback. 	
		for(test_value_loop = 0; test_value_loop < 2; test_value_loop++)
		{//first write 0 and then write 1, and make sure that loopback is reading / writing both successfully...					
			
			if(!test_value_loop)
			{
				DBG_MSG("GPIO Looping back - Pin #%03d as out & Pin #%03d as in\n",gpio_number_loopback[loop_count+1],gpio_number_loopback[loop_count]);
			}
			write_value = (0 == test_value_loop)?TURN_OFF:TURN_ON;
			write_to_gpio_pin(ZCU100_GPIO_BASE + gpio_number_loopback[loop_count+1],write_value);	
						
			test_result = read_from_gpio(ZCU100_GPIO_BASE + gpio_number_loopback[loop_count],read_value);						
			if(FAIL == test_result) 
			{				
				final_test_result = FAIL;
				DBG_MSG("Unable to read GPIO Pin # - %d\n",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count]);
				break;
			}
			
			
			if(!strcmp(read_value,GPIO_ACTION_LED_OFF_0_ON_1_STRING[write_value]))
			{//successful read...
				//DBG_MSG("Pin #%d & Pin #%d are having same value\n",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count],ZCU100_GPIO_BASE + gpio_number_loopback[loop_count+1]);				
			}
			else
			{//error loopback problem				
				//DBG_MSG("FAIL : GPIO Pin # %d : Read - %s, Wrote - %d\n",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count],read_value,write_value);
				test_result = final_test_result = FAIL;
				test_value_loop = 1;
				break;
			}			
		}
		if(test_value_loop)
		{
			if(PASS == test_result)//read both the values as expected, so successful loopback
			{
				DBG_MSG("Pin #%d & Pin #%d are loopback successful...\n",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count],ZCU100_GPIO_BASE + gpio_number_loopback[loop_count+1]);				
			}
			else
			{
				ERR_MSG("Pin #%d & Pin #%d are loopback failed...\n",ZCU100_GPIO_BASE + gpio_number_loopback[loop_count],ZCU100_GPIO_BASE + gpio_number_loopback[loop_count+1]);				
			}
		}	
		DBG_MSG("\n\n\n");					
	}
	return final_test_result;
}
TEST_RESULT test_gpio_loopback()
{
	
	int 		loop_count;
	TEST_RESULT test_result = PASS;
	TEST_RESULT final_test_result = PASS;
	
	test_result = open_gpio_for_loopback();	
	if(FAIL == test_result) return FAIL;
	
	final_test_result = read_write_gpio_loopback();
	
	close_gpio_for_loopback();
	
	return final_test_result;
}	

void * thread_gpio_led_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	do
	{
		test_result = test_gpio_led();		
		if(FAIL == test_result)
		{	
			ERR_MSG("GPIO LED Test failed, exiting GPIO LED test\n");
			break;
		}
	}while(keep_threads_running);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}
void * thread_gpio_loopback_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	
	open_gpio_for_loopback();
	
	do
	{
		test_result = read_write_gpio_loopback();
		if(FAIL == test_result)
		{	
			ERR_MSG("GPIO Loopback Test failed, exiting GPIO Loopback test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);
	
	close_gpio_for_loopback();
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}

void * thread_gpio_sw_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	do
	{
		test_result = test_gpio_sw();
		if(FAIL == test_result)
		{	
			ERR_MSG("GPIO Push button Test failed, exiting GPIO Push button test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}
