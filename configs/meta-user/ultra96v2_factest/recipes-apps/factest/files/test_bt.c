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
//  Module Name:         test_bt.c
//  Project Name:        Ultra96-V2 Bluetooth test application
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
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "test_zcu100.h"
#include "typedef.h"

//#define BT_SERVER_MAC_ID		"A4:D5:78:1C:66:83"; //perform "hciconfig -a" on the server, it shall give MAC ID. 
#define BT_SERVER_MAC_ID_FILE	"bluetooth_server_id.txt" //specify server's ID in this file
#define BT_SERVER_MAC_ID_SIZE	(18)
#define	BT_DATA_XFER_STRING		"This is bluetooth test data"
#define BT_SCAN_DUMP_TXT		"bluetooth_scan_data.txt"
#define BT_SCAN_RESULT_TAG		":"
#define BT_DEV_DUMP_TXT		"bluetooth_dev.txt"
#define BT_DEV_RESULT_TAG	"hci"

#define BT_LED_GPIO         510
#define LED_ON                1
#define LED_OFF               0

TEST_RESULT scan_bt_device(STRING_SEARCH 	*string_search)
{
	char  			cmd_string[COMMAND_LENGTH];
	FILE 			*fpBTScanResult = NULL;	 
	char			loop_count = 0;	
	TEST_RESULT 	test_result = PASS;
	char  			buffer[COMMAND_LENGTH];
	char  			temp[COMMAND_LENGTH];
	int				ret_val;	
	
	test_result = FAIL;
	*string_search = STRING_NOT_FOUND;
	
	for(loop_count = 0; loop_count < 5; loop_count++)
	{
		sprintf(cmd_string,"hcitool scan > %s",BT_SCAN_DUMP_TXT);
		system(cmd_string);
		
		fpBTScanResult = fopen(BT_SCAN_DUMP_TXT, "r");
		if (!fpBTScanResult)
		{	
			ERR_MSG("error in creating temp file for BT Scan Result\n");
			return FAIL;
		}
			
		/* Parse the text file to find the required information */
		while (fgets(buffer,COMMAND_LENGTH, fpBTScanResult)!= NULL) 
		{		
			if((strstr(buffer, BT_SCAN_RESULT_TAG)) != NULL)
			{
				strcpy(temp, buffer);		
				*string_search = STRING_FOUND;
				DBG_MSG("BT Scan Found Device(s):\n%s\n",temp);						
				test_result = PASS;		
				break;
			}				
		}
		
		fclose(fpBTScanResult);
		ret_val = remove(BT_SCAN_DUMP_TXT);
		if(ret_val == 0)
		{
			DBG_MSG("Removed Test File - \"%s\"\n",BT_SCAN_DUMP_TXT);
		}
		else
		{
			ERR_MSG("Error Removing file - \"%s\"\n",BT_SCAN_DUMP_TXT);
		}	
		if(STRING_FOUND == string_search)
			break;
		sleep(2);
	}
	return test_result;
}

TEST_RESULT get_bt_device(STRING_SEARCH 	*string_search)
{
	char  			cmd_string[COMMAND_LENGTH];
	FILE 			*fpBTDevResult = NULL;	 
	char			loop_count = 0;	
	TEST_RESULT 	test_result = PASS;
	char  			buffer[COMMAND_LENGTH];
	char  			temp[COMMAND_LENGTH];
	int				ret_val;	
	
	test_result = FAIL;
	*string_search = STRING_NOT_FOUND;

	sprintf(cmd_string,"hcitool dev > %s",BT_DEV_DUMP_TXT);
	system(cmd_string);
	
	fpBTDevResult = fopen(BT_DEV_DUMP_TXT, "r");
	if (!fpBTDevResult)
	{	
		ERR_MSG("error in creating temp file for BT Dev Result\n");
		return FAIL;
	}
		
	/* Parse the text file to find the required information */
	while (fgets(buffer,COMMAND_LENGTH, fpBTDevResult)!= NULL) 
	{		
		if((strstr(buffer, BT_DEV_RESULT_TAG)) != NULL)
		{
			strcpy(temp, buffer);		
			*string_search = STRING_FOUND;
			DBG_MSG("BT Found Device(s):\n%s\n",temp);						
			test_result = PASS;		
			break;
		}				
	}
	
	fclose(fpBTDevResult);
	ret_val = remove(BT_DEV_DUMP_TXT);
	if(ret_val == 0)
	{
		DBG_MSG("Removed Test File - \"%s\"\n",BT_DEV_DUMP_TXT);
	}
	else
	{
		ERR_MSG("Error Removing file - \"%s\"\n",BT_DEV_DUMP_TXT);
	}	
	return test_result;
}

TEST_RESULT read_server_mac_id(char *server_mac_id)
{
	FILE *fp = NULL;
	
	fp = fopen(BT_SERVER_MAC_ID_FILE, "r");
	if (!fp)
	{
		ERR_MSG("Error opening %s file\n",BT_SERVER_MAC_ID_FILE);			
		return FAIL;
	}	
	
	/* Parse the text file to find the required information */
	fscanf(fp,"%s",server_mac_id);
	DBG_MSG("Bluetooth Server MAC ID is - %s\n",server_mac_id);
	
	fclose(fp);
	return PASS;
}
TEST_RESULT start_bt(void)
{

/*
 * This test assumes the device driver for the Wi-Fi/BT module 
 * has been previously loaded.  This is done during the 
 * start_wifi() procedure in the test_wifi.c source file.
 */
 
	TEST_RESULT test_result = PASS;
	char 		str_cmd[COMMAND_LENGTH];
	int 		ret_val = 0;
	
	sprintf(str_cmd,"sudo rfkill unblock bluetooth");	
	DBG_MSG("Executing command - %s\n",str_cmd);
	ret_val = system(str_cmd);	
	if(ret_val != 0)
	{
		ERR_MSG("Error running %s command\n",str_cmd);			
		return FAIL;
	}
	
//tc	sprintf(str_cmd,"/etc/init.d/wl18xx start > /dev/null");

   //Some Bluetooth init housekeeping
   system("echo BT_POWER_UP > /dev/wilc_bt");
   system("echo BT_DOWNLOAD_FW > /dev/wilc_bt");
   system("echo BT_FW_CHIP_WAKEUP > /dev/wilc_bt");
   system("stty -F /dev/ttyPS1 115200");
   system("stty -F /dev/ttyPS1 crtscts");

   /*
    * Attach the Bluetooth interface to the UART.  User will see
    * "Can't set device: Device or resource busy" and
    * "Can't initialize device: Device or resource busy" messages
    * if this test is run repeatedly without rebooting
    */
   sprintf(str_cmd,"hciattach /dev/ttyPS1 -t 10 any 115200 noflow nosleep > /dev/null");
	DBG_MSG("Executing command - %s\n",str_cmd);
	system(str_cmd);	
	sleep(2);


	sprintf(str_cmd,"hciconfig hci0 up > /dev/null");	
	DBG_MSG("Executing command - %s\n",str_cmd);
	ret_val = system(str_cmd);	
	if(ret_val != 0)
	{
		ERR_MSG("Error running %s command\n",str_cmd);			
		return FAIL;
	}
	return test_result;
}


TEST_RESULT stop_bt(void)
{
	TEST_RESULT test_result = PASS;
	char 		str_cmd[COMMAND_LENGTH];
	int 		ret_val = 0;
	
	//not sure whether following sleep is required. 
	sleep(1);//to make sure that all data communication is completed successfully.
	//sprintf(str_cmd,"hciconfig hci1 down");
//tc	sprintf(str_cmd,"/etc/init.d/wl18xx stop > /dev/null");
	sprintf(str_cmd,"hciconfig hci0 down > /dev/null");
	DBG_MSG("Executing command - %s\n",str_cmd);
	ret_val = system(str_cmd);	
	if(ret_val != 0)
	{
		ERR_MSG("Error running %s command\n",str_cmd);			
		return FAIL;
	} 
	return test_result;
}


TEST_RESULT turn_onoff_bt_led(ON_OFF_ACTION on_off_action)
{
	FILE*	   valuefp;
   FILE* 	exportfp;
	FILE* 	directionfp;
	FILE* 	unexportfp;
   
	char     file_name[COMMAND_LENGTH];
   char 		gpio_number_string[10];

   TEST_RESULT test_result = PASS;
	int 		ret_val = 0;
	char 		str_cmd[COMMAND_LENGTH];
	char		str_ask_user[COMMAND_LENGTH];
	
/*
 	if(TURN_OFF == on_off_action)
	{
		test_result = stop_bt();
		if(FAIL == test_result)
		{
			ERR_MSG("Failed in turning OFF BT LED \n");
			return test_result;
		}
	}
	else if(TURN_ON == on_off_action)
	{
		test_result = start_bt();
		if(FAIL == test_result)
		{
			ERR_MSG("Failed in turning ON BT LED \n");
			return test_result;
		}
	}

	printf("\n\n");
	sprintf(str_cmd,"Turning %s Bluetooth, Observe LED. LED should be %s\n",ON_OFF_ACTION_DISPLAY_STRING[on_off_action],ON_OFF_ACTION_DISPLAY_STRING[on_off_action]);
	USR_MSG(str_cmd);

#ifndef CE_TEST		
	sprintf(str_ask_user,"Is LED %s? Enter 'y' (for %s) or 'n'\t:\t",ON_OFF_ACTION_DISPLAY_STRING[on_off_action],ON_OFF_ACTION_DISPLAY_STRING[on_off_action]);
	USR_MSG(str_ask_user);
	test_result = check_user_response();	
#endif	//#ifndef CE_TEST		
*/


	USR_MSG("Turning %s Bluetooth LED. Observe the LED. Bluetooth LED should be %s\n",ON_OFF_ACTION_DISPLAY_STRING[on_off_action],ON_OFF_ACTION_DISPLAY_STRING[on_off_action]);
   
	DBG_MSG("Testing GPIO Bluetooth LED - sysfs GPIO #%d\n", BT_LED_GPIO);
	sprintf(gpio_number_string,"%d",BT_LED_GPIO);

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
	sprintf(file_name,"/sys/class/gpio/gpio%d/direction",BT_LED_GPIO);
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
	sprintf(file_name,"/sys/class/gpio/gpio%d/value",BT_LED_GPIO);
	valuefp = fopen(file_name, "w");
	if (!valuefp)
	{
		ERR_MSG("Cannot open GPIO value\n");
		return FAIL;
	}

	DBG_MSG("GPIO Pin #%d : Writing - %s\n",BT_LED_GPIO,GPIO_ACTION_LED_OFF_0_ON_1_STRING[on_off_action]);

	//perform the action on the GPIO LED - on or off
	fprintf(valuefp,"%s",GPIO_ACTION_LED_OFF_0_ON_1_STRING[on_off_action]);
	 
	fclose (valuefp);

#ifndef CE_TEST			
	sprintf(str_ask_user,"Is Bluetooth LED %s? Enter 'y' (for %s) or 'n'\t:\t",ON_OFF_ACTION_DISPLAY_STRING[on_off_action],ON_OFF_ACTION_DISPLAY_STRING[on_off_action]);
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

TEST_RESULT test_bt_led()
{
	ON_OFF_ACTION on_off_action;
	TEST_RESULT test_result = PASS;
	TEST_RESULT result = PASS;
	char 		str_cmd[COMMAND_LENGTH];

	sprintf(str_cmd,"Performing Bluetooth LED testing...\n\n");
	USR_MSG(str_cmd);

	//Turning OFF BT LED
	result = turn_onoff_bt_led(TURN_OFF);
	test_result = (FAIL == result)? FAIL : test_result;	
	usleep(BT_LED_ON_OFF_SLEEP_USEC);

	//Turning ON BT LED
	result = turn_onoff_bt_led(TURN_ON);
	test_result = (FAIL == result)? FAIL : test_result;	
	usleep(BT_LED_ON_OFF_SLEEP_USEC);

	return test_result;	
}
TEST_RESULT connect_bt_server(int *server_socket)
{
	//system(sprintf("sudo hciconfig hci%d piscan",hci_number));
	TEST_RESULT test_result = PASS;
	struct sockaddr_rc addr = { 0 };
    int status;    
    char dest[18];

	test_result = read_server_mac_id(dest);
	if(FAIL == test_result)
	{
		ERR_MSG("Bluetooth Server MAC ID is not specified, unable to continue test\n");
		return FAIL;
	}
	
    // allocate a socket
    *server_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(*server_socket, (struct sockaddr *)&addr, sizeof(addr));
    if( status < 0 ) 
	{
		DBG_MSG("unable to connect to Bluetooth server\n");
		test_result = FAIL;		
	}	
	else
	{
		DBG_MSG("Connected with bluetooth Server\n");
	}
	return test_result;
}
TEST_RESULT disconnect_bt_server(int *server_socket)
{
	if(*server_socket != 0)
	{
		DBG_MSG("Disconnecting from Bluetooth Server\n");
		close(*server_socket); 	
		*server_socket = 0;
	}
}

TEST_RESULT transfer_data_over_bt(int *server_socket)
{
	TEST_RESULT test_result = PASS;
	int status = 0;
	char buf[50];
	int bytes_read = 0; 
	DBG_MSG("Writing to bluetooth server\n");
	// send a message    
	status = write(*server_socket, BT_DATA_XFER_STRING, strlen(BT_DATA_XFER_STRING));
	if( status < 0 ) 
	{
		ERR_MSG("Unable to send data to Bluetooth server\n");
		test_result = FAIL;
	}
	return test_result;
};


TEST_RESULT test_bt(void) 
{
	TEST_RESULT 	test_result = PASS;
	char 			str_cmd[COMMAND_LENGTH];
	int				server_socket;
	STRING_SEARCH 	string_search = STRING_NOT_FOUND;
	
	sprintf(str_cmd,"Performing Bluetooth testing, this may take a few seconds...\n");
	USR_MSG(str_cmd);

	test_result = start_bt();
	if(FAIL == test_result)
	{
		ERR_MSG("Failed bringing up Bluetooth device \n");
		return test_result;
	}

	test_result = get_bt_device(&string_search); 	
	
	if(STRING_FOUND == string_search)
	{
		DBG_MSG("Bluetooth device(s) found...\n");
		test_result = PASS;		
	}
	else if(STRING_NOT_FOUND == string_search)
	{	
		ERR_MSG("Bluetooth device not found, please make sure that there are discoverable bluetooth devices nearby.\n");
		test_result = FAIL;	
	}
   
   test_result = stop_bt();
	if(FAIL == test_result)
	{
		ERR_MSG("Failed shutting down Bluetooth device \n");
		return test_result;
	}
   
	return test_result;
}
void *thread_bt_led_function(void *void_thread_message)
{		
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	
	start_bt();
	do
	{	
		test_result = test_bt_led();		
		if(FAIL == test_result)
		{	
			ERR_MSG("Bluetooth LED Test failed, exiting Bluetooth LED test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);
	thread_message->test_result = test_result;	
	
	//test_result = stop_bt();
	return void_thread_message;	
}


void * thread_bt_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	int server_socket;
	
	start_bt();
	test_result  = connect_bt_server(&server_socket);
	if(FAIL == test_result)
	{
		ERR_MSG("Unable to connect to Bluetooth Server, test failed\n");
		thread_message->test_result = test_result;	
		return void_thread_message;	
	}
	do
	{
		test_result = transfer_data_over_bt(&server_socket);	
		if(FAIL == test_result)
		{	
			ERR_MSG("Bluetooth Test failed, exiting Bluetooth test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC*50);
	}while(keep_threads_running);
	
	disconnect_bt_server(&server_socket);

	thread_message->test_result = test_result;	
	return void_thread_message;	
}
