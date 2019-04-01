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
//  Module Name:         test_wifi.c
//  Project Name:        Ultra96-V2 Wi-Fi test application
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

#include "test_zcu100.h"
#include "typedef.h"

#define WIFI_SIGNAL_DUMP_TXT 	"wifi_signal_dump.txt"
#define	WIFI_SIGNAL_TAG		"signal:"
#define WIFI_INTERFACE_TXT 	"wifi_interface_up.txt"
#define	WIFI_INTERFACE_TAG	"wlan"

#define WIFI_LED_GPIO         511
#define LED_ON                1
#define LED_OFF               0

TEST_RESULT start_wifi(void)
{
	TEST_RESULT test_result = PASS;
	char 		str_cmd[COMMAND_LENGTH];
	int 		ret_val = 0;
	
	sprintf(str_cmd,"modprobe wilc > /dev/null");	
	DBG_MSG("Executing command - %s\n",str_cmd);
	ret_val = system(str_cmd);	
	if(ret_val != 0)
	{
		ERR_MSG("Error running %s command\n",str_cmd);			
		return FAIL;
	}

	sprintf(str_cmd,"modprobe wilc-sdio > /dev/null");	
	DBG_MSG("Executing command - %s\n",str_cmd);
	ret_val = system(str_cmd);	
	if(ret_val != 0)
	{
		ERR_MSG("Error running %s command\n",str_cmd);			
		return FAIL;
	}

	sprintf(str_cmd,"ifconfig wlan0 up > /dev/null");	
	DBG_MSG("Executing command - %s\n",str_cmd);
	ret_val = system(str_cmd);	
	if(ret_val != 0)
	{
		ERR_MSG("Error running %s command\n",str_cmd);			
		return FAIL;
	}
	
	sleep(2);
	return test_result;
}


TEST_RESULT stop_wifi(void)
{
	TEST_RESULT test_result = PASS;
	char 		str_cmd[COMMAND_LENGTH];
	int 		ret_val = 0;
	
	//not sure whether following sleep is required. 
	//to make sure that all data communication is completed successfully.
   sleep(1);
	sprintf(str_cmd,"ifconfig wlan0 down > /dev/null");
	DBG_MSG("Executing command - %s\n",str_cmd);
	ret_val = system(str_cmd);	
	if(ret_val != 0)
	{
		ERR_MSG("Error running %s command\n",str_cmd);			
		return FAIL;
	} 
	return test_result;
}

TEST_RESULT get_wifi_signal_strength(void)
{
	char  			cmd_string[COMMAND_LENGTH];
	FILE 			*fpWiFiSignalStrenth = NULL;
	STRING_SEARCH 	string_search = STRING_NOT_FOUND;
	char			loop_count = 0;	
	TEST_RESULT 	test_result = PASS;
	char  			buffer[COMMAND_LENGTH];
	char  			temp[COMMAND_LENGTH];
	int				ret_val;	
	
	test_result = FAIL;
	for(loop_count = 0; loop_count < 5; loop_count++)
	{
		sprintf(cmd_string,"iw dev wlan0 scan | egrep 'signal' > %s",WIFI_SIGNAL_DUMP_TXT);
		system(cmd_string);
		
		fpWiFiSignalStrenth = fopen(WIFI_SIGNAL_DUMP_TXT, "r");
		if (!fpWiFiSignalStrenth)
		{	
			ERR_MSG("error in creating temp file for Wi-Fi Signal Strength\n");
			return FAIL;
		}
			
		/* Parse the text file to find the required information */
		while (fgets(buffer,COMMAND_LENGTH, fpWiFiSignalStrenth)!= NULL) 
		{		
			if((strstr(buffer, WIFI_SIGNAL_TAG)) != NULL)
			{
				strcpy(temp, buffer);		
				string_search = STRING_FOUND;
				DBG_MSG("Wi-Fi Signal Found: %s\n",temp);						
				test_result = PASS;
			}				
		}
		
		fclose(fpWiFiSignalStrenth);
		ret_val = remove(WIFI_SIGNAL_DUMP_TXT);
		if(ret_val == 0)
		{
			DBG_MSG("Removed Test File - \"%s\"\n",WIFI_SIGNAL_DUMP_TXT);
		}
		else
		{
			ERR_MSG("Error Removing file - \"%s\"\n",WIFI_SIGNAL_DUMP_TXT);
		}	
		if(STRING_FOUND == string_search)
			break;
		sleep(5);
	}
	return test_result;
}

TEST_RESULT get_wifi_interface(void)
{
	char  			cmd_string[COMMAND_LENGTH];
	FILE 			*fpWiFiInterfaceUp = NULL;
	STRING_SEARCH 	string_search = STRING_NOT_FOUND;
	char			loop_count = 0;	
	TEST_RESULT 	test_result = PASS;
	char  			buffer[COMMAND_LENGTH];
	char  			temp[COMMAND_LENGTH];
	int				ret_val;	
	
	test_result = FAIL;

	sprintf(cmd_string,"iw dev | egrep 'wlan' > %s",WIFI_INTERFACE_TXT);
	system(cmd_string);
	sleep(10);
	
	fpWiFiInterfaceUp = fopen(WIFI_INTERFACE_TXT, "r");
	if (!fpWiFiInterfaceUp)
	{	
		ERR_MSG("error in creating temp file for Wi-Fi interface up\n");
		return FAIL;
	}
		
	/* Parse the text file to find the required information */
	while (fgets(buffer,COMMAND_LENGTH, fpWiFiInterfaceUp)!= NULL) 
	{		
		if((strstr(buffer, WIFI_INTERFACE_TAG)) != NULL)
		{
			strcpy(temp, buffer);		
			string_search = STRING_FOUND;
			DBG_MSG("Wi-Fi interface Found: %s\n",temp);						
			test_result = PASS;
		}				
	}
	
	fclose(fpWiFiInterfaceUp);
	ret_val = remove(WIFI_INTERFACE_TXT);
	if(ret_val == 0)
	{
		DBG_MSG("Removed Test File - \"%s\"\n",WIFI_INTERFACE_TXT);
	}
	else
	{
		ERR_MSG("Error Removing file - \"%s\"\n",WIFI_INTERFACE_TXT);
	}	
return test_result;
}	

void turn_onoff_wifi(ON_OFF_ACTION on_off_action)
{
	char  		cmd_string[COMMAND_LENGTH];
	
	if(TURN_ON == on_off_action)
	{
		
		sprintf(cmd_string,"rfkill unblock all",ENABLE_DISABLE_ACTION_STRING[on_off_action]);
		system(cmd_string);	
		sleep(1);
		
		sprintf(cmd_string,"ip link set wlan0 %s",UP_DOWN_ACTION_STRING[on_off_action]);
		system(cmd_string);
		sleep(1);		
	
	}	
	
	sprintf(cmd_string,"ifconfig wlan0 %s",ON_OFF_ACTION_STRING[on_off_action]);
	system(cmd_string);
	sleep(1);
}
TEST_RESULT turn_onoff_wifi_led(ON_OFF_ACTION on_off_action)
{
  	FILE*	      valuefp;
   FILE* 		exportfp;
	FILE* 		directionfp;
	FILE* 		unexportfp;
   
	char file_name[COMMAND_LENGTH];
   char 		gpio_number_string[10];
	char  		cmd_string[COMMAND_LENGTH];
	char        str_ask_user[COMMAND_LENGTH];
	int         ret_val = 0;
	TEST_RESULT test_result;

	USR_MSG("Turning %s Wi-Fi LED, Observe the LED. Wi-Fi LED should be %s\n",ON_OFF_ACTION_DISPLAY_STRING[on_off_action],ON_OFF_ACTION_DISPLAY_STRING[on_off_action]);
//tc	turn_onoff_wifi(on_off_action);
   
	DBG_MSG("Testing GPIO Wi-Fi LED - sysfs GPIO #%d\n", WIFI_LED_GPIO);
	sprintf(gpio_number_string,"%d",WIFI_LED_GPIO);

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
	sprintf(file_name,"/sys/class/gpio/gpio%d/direction",WIFI_LED_GPIO);
	directionfp = fopen(file_name, "w");
	if (!directionfp)
	{
		ERR_MSG("Cannot open GPIO to direction it\n");
		return FAIL;
	}
		
	//set the direction of the led as out
	fprintf(directionfp, "%s","out");
	fclose(directionfp);	 
	DBG_MSG("GPIO direction set as output successfully\n");
   	
	// Get the GPIO value ready to be toggled
	sprintf(file_name,"/sys/class/gpio/gpio%d/value",WIFI_LED_GPIO);
	valuefp = fopen(file_name, "w");
	if (!valuefp)
	{
		ERR_MSG("Cannot open GPIO value\n");
		return FAIL;
	}

	DBG_MSG("GPIO Pin #%d : Writing - %s\n",WIFI_LED_GPIO,GPIO_ACTION_LED_OFF_0_ON_1_STRING[on_off_action]);

	//perform the action on the GPIO LED - on or off
	fprintf(valuefp,"%s",GPIO_ACTION_LED_OFF_0_ON_1_STRING[on_off_action]);
	 
	fclose (valuefp);

#ifndef CE_TEST			
	sprintf(str_ask_user,"Is Wi-Fi LED %s? Enter 'y' (for %s) or 'n'\t:\t",ON_OFF_ACTION_DISPLAY_STRING[on_off_action],ON_OFF_ACTION_DISPLAY_STRING[on_off_action]);
	USR_MSG(str_ask_user);
	test_result = check_user_response();
#else
	usleep(THREAD_SLEEP_USEC*10);
#endif //#ifndef CE_TEST	

	unexportfp = fopen("/sys/class/gpio/unexport", "w");
	if(!unexportfp) 		
	{
		ERR_MSG("Cannot open GPIO to unexport\n");
		return FAIL;
	}
	
	fprintf(unexportfp, "%s",gpio_number_string);
	fclose(unexportfp);
	if(FAIL == test_result ) 
		test_result =  FAIL;

	return test_result;
}

TEST_RESULT test_wlen_led(void)
{
	TEST_RESULT test_result = PASS;
	TEST_RESULT result = PASS;
	char 		str_cmd[COMMAND_LENGTH];

	sprintf(str_cmd,"Performing Wi-Fi LED testing...\n\n");
	USR_MSG(str_cmd);

   //turn off Wi-Fi LED
   result = turn_onoff_wifi_led(TURN_OFF);
	test_result = (FAIL == result)? FAIL : test_result;
	usleep(WI_FI_LED_ON_OFF_SLEEP_USEC);

	//turn on Wi-Fi LED
	result = turn_onoff_wifi_led(TURN_ON);
	test_result = (FAIL == result)? FAIL : test_result;
	usleep(WI_FI_LED_ON_OFF_SLEEP_USEC);

	return test_result;
}


TEST_RESULT test_wifi(void) 
{	
	char 		cmd_string[COMMAND_LENGTH];
	int 		ret_val = 0;
	TEST_RESULT test_result = PASS;
   
	sprintf(cmd_string,"Performing Wi-Fi testing, this may take a few seconds...\n");
	USR_MSG(cmd_string);
	
	test_result = start_wifi();
	if(FAIL == test_result)
	{
		ERR_MSG("Failed bringing up Wi-Fi device \n");
		return test_result;
	}
   
#ifdef CE_TEST	
	sprintf(cmd_string,"iperf3 -c %s -d -t 20 -i 1",REMOTE_HOST);
	DBG_MSG("Executing command - %s\n",cmd_string);	
	system(cmd_string);		
#else
   //For production test	
	test_result = get_wifi_interface();	
#endif //#ifdef CE_TEST

   test_result = stop_wifi();
	if(FAIL == test_result)
	{
		ERR_MSG("Failed shutting down Wi-Fi device \n");
		return test_result;
	}
	
	return test_result;
}

void * thread_wlen_led_function(void *void_thread_message)
{		
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	ON_OFF_ACTION on_off_action = TURN_ON;
	
	USR_MSG("Toggling Wi-Fi LED\n");			
	do
	{			
		test_result = test_wlen_led();
		if(FAIL == test_result)
		{	
			ERR_MSG("Wi-Fi LED Test failed, exiting Wi-Fi LED test\n");
			break;
		}		
		usleep(THREAD_SLEEP_USEC*100);		
	}while(keep_threads_running);
	
	USR_MSG("Exiting Wi-Fi LED thread\n");
	thread_message->test_result = test_result;	
	return void_thread_message;	
}


void * thread_wifi_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	
	turn_onoff_wifi(TURN_ON);	
	do
	{
		test_result = test_wifi();		
		if(FAIL == test_result)
		{	
			ERR_MSG("Wi-Fi Test failed, exiting Wi-Fi test\n");
			break;
		}	
	}while(keep_threads_running);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}
