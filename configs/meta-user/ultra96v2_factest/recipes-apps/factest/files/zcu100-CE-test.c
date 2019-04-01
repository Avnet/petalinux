#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

#include "typedef.h"
#include "test_zcu100.h"

int input_test_choice = 0;
typedef enum _TEST_INTERFACES
{
	TEST_ALL,
	
//	TEST_WIFI_LED,
//	TEST_BT_LED,
	TEST_GPIO_LED,
//	TEST_GPIO_SW,
	TEST_WIFI,
	TEST_BT,
	TEST_SD,
	TEST_USB3,
	TEST_USB2,
	TEST_USB_UPSTREAM,
	TEST_GPIO_LOOPBACK,
	TEST_I2C_EEPROM,
	TEST_SPI_FLASH,
	TEST_INA226,
	TEST_DP,
//	TEST_PMIC,
	TEST_WIFI_BT,
	TEST_OUT_RANGE
		
}TEST_INTERFACES;
typedef void *(*function_pointer) (void *);

sig_atomic_t volatile keep_threads_running = TRUE;
void sigint(int signo) {
    (void)signo;
	USR_MSG("\nCtrl + C Pressed, Exiting test application. Wait for few seconds...\n");
	keep_threads_running = FALSE;
}

TEST_RESULT start_thread(pthread_t *thread_handle,function_pointer ptr_func,THREAD_MESSAGE *thread_message)
{
	TEST_RESULT test_result = PASS;
	
	/* Create independent threads each of which will execute function */		
	thread_message->test_result		= PASS;
	test_result = pthread_create(thread_handle,NULL,ptr_func,(void*)thread_message);
	if(FAIL == test_result)
	{
		ERR_MSG("Error Creating thread...\n");
		*thread_handle = (pthread_t) NULL;
		return FALSE;
	}
	
	return test_result;
}
void stop_thread(pthread_t *pthread_handle)
{
	pthread_t thread_handle = *pthread_handle;
	
	DBG_MSG("Exiting threads\n");
	if((pthread_t)NULL != *pthread_handle)
	{
		pthread_cancel(thread_handle);
		pthread_join( thread_handle, NULL);
		*pthread_handle = (pthread_t)NULL;
	}	
}

//to be tested for CE testing
TEST_RESULT create_thread(void)
{	
	TEST_RESULT test_result = PASS;
	int loop_input_test_choice = 0 ;
	
	pthread_t thread_wlen_led, thread_bt_led;
	pthread_t thread_gpio_led; 
	pthread_t thread_gpio_sw;
	pthread_t thread_wifi;
	pthread_t thread_bt;
	pthread_t thread_sd; 
	pthread_t thread_usb3_0; 
	pthread_t thread_usb3_1;	
	pthread_t thread_usb2;
	pthread_t thread_usb_upstream;
	pthread_t thread_gpio_loopback;
	pthread_t thread_hs_eeprom_0,thread_hs_eeprom_1 ,thread_ls_eeprom_0,thread_ls_eeprom_1;
	pthread_t thread_SPI_flash;
	pthread_t thread_ina226;
	pthread_t thread_dp;
	pthread_t thread_pmic;	
	
	THREAD_MESSAGE thread_wlen_led_message, thread_bt_led_message;
	THREAD_MESSAGE thread_gpio_led_message;
	THREAD_MESSAGE thread_gpio_sw_message;
	THREAD_MESSAGE thread_wifi_message; 
	THREAD_MESSAGE thread_bt_message; 
	THREAD_MESSAGE thread_sd_message;
	THREAD_MESSAGE thread_usb3_0_message;
	THREAD_MESSAGE thread_usb3_1_message;	
	THREAD_MESSAGE thread_usb2_message;
	THREAD_MESSAGE thread_usb_upstream_message;
	THREAD_MESSAGE thread_gpio_loopback_message;
	THREAD_MESSAGE thread_hs_eeprom_0_message,thread_hs_eeprom_1_message,thread_ls_eeprom_0_message,thread_ls_eeprom_1_message;
	THREAD_MESSAGE thread_SPI_flash_message;
	THREAD_MESSAGE thread_ina226_message;
	THREAD_MESSAGE thread_dp_message;
	THREAD_MESSAGE thread_pmic_message;
	
	// Block the SIGINT signal. The threads will inherit the signal mask.
    // This will avoid them catching SIGINT instead of this thread.
    sigset_t sigset, oldset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    pthread_sigmask(SIG_BLOCK, &sigset, &oldset);
	
	
	for(loop_input_test_choice = TEST_ALL; loop_input_test_choice < TEST_OUT_RANGE; loop_input_test_choice++)
	{
		if(TEST_ALL != input_test_choice) 
			loop_input_test_choice = input_test_choice;
		switch(loop_input_test_choice)
		{	
/*		
			case TEST_WIFI_LED:					
				test_result = start_thread(&thread_wlen_led,&thread_wlen_led_function,(void*)&thread_wlen_led_message);			
				if(FAIL == test_result)
					return test_result;			
				break;
			case TEST_BT_LED:					
				test_result = start_thread(&thread_bt_led,&thread_bt_led_function,(void*)&thread_bt_led_message);
				if(FAIL == test_result)
					return test_result;			
				break;
*/
			case TEST_GPIO_LED:
				test_result = start_thread(&thread_gpio_led,&thread_gpio_led_function,(void*)&thread_gpio_led_message);			
				if(FAIL == test_result)
					return test_result;	
				break;			
/*
			case TEST_GPIO_SW:
				test_result = start_thread(&thread_gpio_sw,NULL,&thread_gpio_sw_function,(void*)&thread_gpio_sw_message);
				if(FAIL == test_result)
					return test_result;	
				break;			
*/	
			case TEST_WIFI:
				test_result = start_thread(&thread_wifi,&thread_wifi_function,(void*)&thread_wifi_message);
				if(FAIL == test_result)
					return test_result;	
				break;			
			case TEST_BT:
				test_result = start_thread(&thread_bt,&thread_bt_function,(void*)&thread_bt_message);
				if(FAIL == test_result)
					return test_result;	
				break;	
			case TEST_SD:
				test_result = start_thread(&thread_sd,&thread_sd_function,(void*)&thread_sd_message);
				if(FAIL == test_result)
					return test_result;	
				break;	
			case TEST_USB3:
/*
				thread_usb3_0_message.function_data 	= USB3_PORT_0;
				test_result = start_thread(&thread_usb3_0,&thread_usb_function,(void*)&thread_usb3_0_message);
				if(FAIL == test_result)
					return test_result;	
*/				
				thread_usb3_1_message.function_data 	= USB3_PORT_1;
				test_result = start_thread(&thread_usb3_1,&thread_usb_function,(void*)&thread_usb3_1_message);
				if(FAIL == test_result)
					return test_result;	

				break;		
			case TEST_USB2:
				thread_usb2_message.function_data 		= USB2_PORT;
				test_result = start_thread(&thread_usb2,&thread_usb_function,(void*)&thread_usb2_message);
				if(FAIL == test_result)
					return test_result;	
				break;		
			case TEST_USB_UPSTREAM:
				test_result = start_thread(&thread_usb_upstream,&thread_usb_upstream_function,(void*)&thread_usb_upstream_message);
				if(FAIL == test_result)
					return test_result;	
				break;			
			case TEST_GPIO_LOOPBACK:
				test_result = start_thread(&thread_gpio_loopback,&thread_gpio_loopback_function,(void*)&thread_gpio_loopback_message);			
				if(FAIL == test_result)
					return test_result;	
				break;			
			case TEST_I2C_EEPROM:
				thread_hs_eeprom_0_message.function_data 	= HS_I2C_EEPROM_0;
				test_result = start_thread(&thread_hs_eeprom_0,&thread_eeprom_function,(void*)&thread_hs_eeprom_0_message);
				if(FAIL == test_result)
					return test_result;	
				
				thread_hs_eeprom_1_message.function_data 	= HS_I2C_EEPROM_1;
				test_result = start_thread(&thread_hs_eeprom_1,&thread_eeprom_function,(void*)&thread_hs_eeprom_1_message);
				if(FAIL == test_result)
					return test_result;	

				thread_ls_eeprom_0_message.function_data 	= LS_I2C_EEPROM_0;
				test_result = start_thread(&thread_ls_eeprom_0,&thread_eeprom_function,(void*)&thread_ls_eeprom_0_message);
				if(FAIL == test_result)
					return test_result;	

				thread_ls_eeprom_1_message.function_data 	= LS_I2C_EEPROM_1;
				test_result = start_thread(&thread_ls_eeprom_1,&thread_eeprom_function,(void*)&thread_ls_eeprom_1_message);
				if(FAIL == test_result)
					return test_result;	

				break;		
			case TEST_SPI_FLASH:				
				test_result = start_thread(&thread_SPI_flash,&thread_flash_function,(void*)&thread_SPI_flash_message);
				if(FAIL == test_result)
					return test_result;	
				break;			

			case TEST_INA226:
				test_result = start_thread(&thread_ina226,&thread_ina226_function,(void*)&thread_ina226_message);
				if(FAIL == test_result)
					return test_result;	
				break;			

			case TEST_DP:
				test_result = start_thread(&thread_dp,&thread_dp_function,(void*)&thread_dp_message);
				if(FAIL == test_result)
					return test_result;	
				break;			
/*
			case TEST_PMIC:
				test_result = start_thread(&thread_pmic,&thread_pmic_function,(void*)&thread_pmic_message);
				if(FAIL == test_result)
					return test_result;	
				break;		
*/
			case TEST_WIFI_BT:
				if(TEST_ALL != input_test_choice) 
				{
					test_result = start_thread(&thread_wifi,&thread_wifi_function,(void*)&thread_wifi_message);
					if(FAIL == test_result)
						return test_result;	
					test_result = start_thread(&thread_bt,&thread_bt_function,(void*)&thread_bt_message);
					if(FAIL == test_result)
						return test_result;	
				}
				break;
			case TEST_ALL:
			default:
				break;
		}
		if(TEST_ALL != input_test_choice) 
			break;//go out of the for loop if we dont have to start all the tests...
	}	
	
	// Install the signal handler for SIGINT.
    struct sigaction s;
    s.sa_handler = (void(*) (int))sigint;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;
    sigaction(SIGINT, &s, NULL);

    // Restore the old signal mask only for this thread.
    pthread_sigmask(SIG_SETMASK, &oldset, NULL);

    // Wait for SIGINT to arrive.
    pause();

	for(loop_input_test_choice = TEST_ALL; loop_input_test_choice < TEST_OUT_RANGE; loop_input_test_choice++)
	{
		if(TEST_ALL != input_test_choice) 
			loop_input_test_choice = input_test_choice;
		switch(input_test_choice)
		{			
/*		
			case TEST_WIFI_LED:					
				stop_thread(&thread_wlen_led);
				break;
			case TEST_BT_LED:
				stop_thread(&thread_bt_led);
				break;
*/	
			case TEST_GPIO_LED:
				stop_thread(&thread_gpio_led);
				break;	
/*	
			case TEST_GPIO_SW:
				stop_thread(&thread_gpio_sw);
				break;
*/
			case TEST_WIFI:
				stop_thread(&thread_wifi);
				break;
			case TEST_BT:
				stop_thread(&thread_bt);
				break;
			case TEST_SD:
				stop_thread(&thread_sd);
				break;
			case TEST_USB3:
//				stop_thread(&thread_usb3_0);
				stop_thread(&thread_usb3_1);
				break;
			case TEST_USB2:
				stop_thread(&thread_usb2);
				break;
			case TEST_USB_UPSTREAM:
				stop_thread(&thread_usb_upstream);
				break;
			case TEST_GPIO_LOOPBACK:
				stop_thread(&thread_gpio_loopback);
				break;
			case TEST_I2C_EEPROM:
				stop_thread(&thread_hs_eeprom_0);
				stop_thread(&thread_hs_eeprom_1);
				stop_thread(&thread_ls_eeprom_0);
				stop_thread(&thread_ls_eeprom_1);
				break;
			case TEST_SPI_FLASH:
				stop_thread(&thread_SPI_flash);
				break;
			case TEST_INA226:
				stop_thread(&thread_ina226);
				break;
	
			case TEST_DP:
				stop_thread(&thread_dp);
				break;
/*			
			case TEST_PMIC:
				stop_thread(&thread_pmic);
				break;
*/		
			case TEST_WIFI_BT:
				if(TEST_ALL != input_test_choice) 
				{
					stop_thread(&thread_wifi);
					stop_thread(&thread_bt);
				}
				break;
			case TEST_ALL:
			default:
				break;
		}
		if(TEST_ALL != input_test_choice) 
			break;//go out of the for loop if we dont have to start all the tests...
	}
}
void choose_test_to_perform(void)
{		
	int index = 0;
	
	print_double_line();
	
	USR_MSG("\tZCU100 - Interfaces to be Tested are\t:\t\n");
	USR_MSG("\t\t%d.\tALL INTERFACES\n",TEST_ALL);	
//	USR_MSG("\t\t%d.\tWi-Fi LED\n",TEST_WIFI_LED);
//	USR_MSG("\t\t%d.\tBluetooth LED\n",TEST_BT_LED);
	USR_MSG("\t\t%d.\tGPIO LED\n",TEST_GPIO_LED);
	//USR_MSG("\t\t%d.\tGPIO Pushbutton\n",TEST_GPIO_SW);
	USR_MSG("\t\t%d.\tWi-Fi\n",TEST_WIFI);
	USR_MSG("\t\t%d.\tBluetooth\n",TEST_BT);
	USR_MSG("\t\t%d.\tSD Card\n",TEST_SD);
	USR_MSG("\t\t%d.\tUSB 3.0\n",TEST_USB3);
	USR_MSG("\t\t%d.\tUSB 2.0\n",TEST_USB2);
	USR_MSG("\t\t%d.\tUSB Mass Storage Gadget\n",TEST_USB_UPSTREAM);
	USR_MSG("\t\t%d.\tGPIO Loopback\n",TEST_GPIO_LOOPBACK);
	USR_MSG("\t\t%d.\tI2C EEPROM\n",TEST_I2C_EEPROM);
	USR_MSG("\t\t%d.\tSPI FLASH\n",TEST_SPI_FLASH);
	USR_MSG("\t\t%d.\tINA226\n",TEST_INA226);
	USR_MSG("\t\t%d.\tDisplay Port\n",TEST_DP);
	USR_MSG("\t\t%d.\tWi-Fi & Bluetooth\n",TEST_WIFI_BT);
//	USR_MSG("\t\t%d.\tPMIC\n",TEST_PMIC);
	
	USR_MSG("\nPlease Enter the appropriate number for the interface to test.\n");
	do{
		fflush(stdout);		
		USR_MSG("Please enter value between %d - %d\t:\t",TEST_ALL,TEST_OUT_RANGE -1);
		scanf(" %d",&input_test_choice);			
		if(TEST_ALL <= input_test_choice < TEST_OUT_RANGE) 
			break;
	}while( FOREVER_LOOP );
	USR_MSG("Press Ctrl + C to stop the test & wait for few seconds...\n");
	print_double_line();	
}
//create threads for CE testing
void main(void)
{	
	unsigned int 	test_loop_count = 0;	//TO DO = this will overflow in the infinite loop. it does not make sense to do testing for infinite...
	TEST_RESULT 	test_result = PASS;
	
	//disable system messages being redirected to console when test is going on...
	DBG_MSG("Disabling system messages\n");
	system("dmesg -D");

	/*
	 * Print banner.
	 */
	print_double_line();
	USR_MSG("\n\r\n\t\t\t\t\t\tZCU 100 CE TEST");
	USR_MSG("\n\r\t\t\t\t\tPerformed on %s\n\r\n", __DATE__);
	print_double_line();

	USR_MSG("\n\n\nPlease make sure that all the devices are connected as required for CE/FCC Testing...\n");
	
	//NOTE: detect the USB devices before starting of the test. 
	//this wont allow the tester to add/remove usb device once the testing starts.
	//if wanted this can be added inside the USB thread.
	get_usb_devices_details();
	
	choose_test_to_perform();
	
	create_thread();
	
	//enable system messages being redirected to console when test is going on...
	system("dmesg -E");
	
	//to discard characters from the STDIO buffer...
	{	
		char c;
		while((c = getchar()) != '\n' && c != EOF)
			/* discard */ ;
	}
	
	USR_MSG("\n\n");
	print_double_line();	
	USR_MSG("Production Test Completed, Shutting down board\n");
	USR_MSG("Press any key to continue shutdown...\n");
	print_double_line();	

	getchar();	
	//system("shutdown 0");	//TO DO - Uncomment this in final code - Jignesh
}
