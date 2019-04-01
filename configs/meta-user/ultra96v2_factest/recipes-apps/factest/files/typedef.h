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
//  Module Name:         typedef.h
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
#ifndef __typedef_h__
#define __typedef_h__

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

//uncomment following line for building source code for CE / FCC testing. 
//comment following line for building source code for production testing. 

//tc #define CE_TEST

#ifdef CE_TEST
	#define PRINT_DBG_MSG 	(FALSE)
	#define PRINT_ERR_MSG 	(TRUE)
	#define PRINT_USR_MSG 	(TRUE)
#else //production test
	#define PRINT_DBG_MSG 	(FALSE)
	#define PRINT_ERR_MSG 	(TRUE)
	#define PRINT_USR_MSG 	(TRUE)
#endif//CE_TEST

extern sig_atomic_t volatile keep_threads_running;

#define LINE_PRINT_LENGTH	(85)
#define FOREVER_LOOP 		(1)
#define TRUE				(1)
#define FALSE				(0)

#define COMMAND_LENGTH		(256)
#define THREAD_SLEEP_USEC	(10000)//10 msec.
#define MOUNT_FS_TYPE		"vfat"
typedef enum _TEST_RESULT
{	
	FAIL = -1,	
	PASS = 0	
}TEST_RESULT;
typedef enum _STRING_SEARCH
{
	STRING_NOT_FOUND 	= -1,
	STRING_FOUND 		= 0
}STRING_SEARCH;
typedef struct _THREAD_MESSAGE
{
	int	 			iteration_count;
	int				function_data;		
	TEST_RESULT		test_result;
}THREAD_MESSAGE;
typedef enum _ON_OFF_ACTION
{
	TURN_OFF	= 0,
	TURN_ON 	= 1
}ON_OFF_ACTION;
static const char *START_STOP_ACTION_STRING[] 		= {"stop","start"};//required for Wi-Fi & BT
static const char *ENABLE_DISABLE_ACTION_STRING[] 	= {"disable","enable"};//required for Wi-Fi & BT
static const char *UP_DOWN_ACTION_STRING[] 			= {"down","up"};//required for Wi-Fi & BT
static const char *ON_OFF_ACTION_STRING[] 			= {"down","up"};//required for Wi-Fi & BT
static const char *ON_OFF_ACTION_DISPLAY_STRING[] 	= {"OFF","ON"};//required for Wi-Fi & BT
static const char *GPIO_ACTION_LED_OFF_0_ON_1_STRING[] = {"0","1"};


/****************************** Wi-Fi *************************/
#define WI_FI_LED_ON_OFF_SLEEP_USEC		(500000)//0.5 sec = 500 msec
#define REMOTE_HOST 					"192.168.8.155"

/****************************** Bluetooth *************************/
#define BT_LED_ON_OFF_SLEEP_USEC		(500000)//0.5 sec = 500 msec

/****************************** SD ******************************/
//Uncomment following line for SD BOOT systems.
//*IF* the system has booted from SD card there is no need to test the 
//SD card interface since its function is verified by the fact
//the system has booted.
//*ELSE* need to mount sd card (comment following line)
//for example, the system was booted from QSPI (not possible for Ultra96)
#define	SD_BOOT					(1)

//This need to be adjusted properly.if sdboot is there then mmcblk0p1 is storing boot partition so dont want to write there.
//mmcblk0p2 is rootfs and should be good to write.
#ifdef SD_BOOT
	#define SD_DEV					"/dev/mmcblk0p2"
#else
	#define SD_DEV					"/dev/mmcblk0p1"
#endif//#ifdef SD_BOOT

#define TEST_STRING_MESSAGE 	"this is test string"
#if (SD_BOOT)
	#define MOUNT_DIR			""
#else
	#define MOUNT_DIR			"/mnt/sdcard"
#endif//SD_BOOT			

/****************************** GPIO LED ******************************/
#define ZCU100_GPIO_BASE				(336)

//Total four GPIO LEDs present on the board
#define NUMBER_OF_GPIO_LEDS 		(4)

#define GPIO_LED_ON_DURATION_USEC	(500000)//0.5 sec = 500 msec

/****************************** GPIO SWITCH ******************************/
#define GPIO_SW_PRESSED_RELEASE_DURATION_SEC	(5)

 
/****************************** GPIO LOOPBACK ******************************/



/********************************* I2C - EEPROM  ***********************************/
	
#define EEPROM_TEST_SIZE_BYTES	(24)
#define EEPROM_TEST_SIZE        (EEPROM_TEST_SIZE_BYTES / sizeof(unsigned char))
#define OUT_FILE_NAME			"/tmp/testeeprom.log"
#define EEPROM_PATH        		"/sys/bus/i2c/devices"
#define EEPROM_FILE_NAME   		"eeprom"
#define TEST_PATTERN 			(0xA5)

typedef enum _I2C_EEPROM
{//based on dts file, ls eeprom 0 is 1st and others are following.
	LS_I2C_EEPROM_0 = 0,
	LS_I2C_EEPROM_1 = 1,
	HS_I2C_EEPROM_0 = 2,
	HS_I2C_EEPROM_1 = 3
}I2C_EEPROM;

/********************************* SPI - FLASH  ***********************************/

#define MTD_FLASH_DEVICE_NAME			"/dev/mtd0"
#define MTD_FLASH_RW_TEST_SIZE			(264)

/********************************* USB  ***********************************/
#define MAX_USB_HOST_PORTS				(3)

typedef enum _USB_PORTS
{
	INVALID_USB_PORT 	= -1,
	USB3_PORT_0 		= 0,
	USB3_PORT_1 		= 1,
	USB2_PORT 			= 2
}USB_PORTS;

/*
USB 1.0 Speed is 12Mbps
USB 2.0 Speed is 480Mbps
USB 3.0 Speed is 5000Mbps
USB 3.1 Speed is 10000Mbps
*/
typedef enum _USB_PORT_SPEED_VERSION
{
	INVALID_USB_SPEED 	= -1,
	USB_1_x				= 0,
	USB_2_x				= 1,
	USB_3_0				= 2,
	USB_3_1				= 3
}USB_PORT_SPEED_VERSION;

#define USB_MASS_STORAGE_STRING	"usb-storage"
#define USB_TEST_FILE 			"test_usb.txt"
#define USB_MOUNT_DIR 			"/mnt/usbstick"
#define USB_DRIVE_TMP_FILE		"usb_drive.txt"
#define LOOP_DEV 				"/dev/loop0"		//required to create loop device for MSG , may be we will remove this one
#define USB_MSG_DEV 			"/tmp/usbmsgdev"		//required to create loop device for MSG , may be we will remove this one
#define USB_MSG_MNT_DIR			"/mnt/usb_msg_test"	//this is where mass storage gadget device after connecting with USB host port in loopback is mounted
#define USB_MSG_TEST_FILE		"test_usb_msg.txt"
#define USB_GADGET_MODULE_PATH	"./gadget"			//for installing gadget driver, all the loadable modules are stored here


#define DBG_MSG(...) {if(TRUE == PRINT_DBG_MSG) fprintf(stderr,##__VA_ARGS__);}
#define ERR_MSG(...) {if(TRUE == PRINT_ERR_MSG) fprintf(stderr,##__VA_ARGS__);}
#define USR_MSG(...) {if(TRUE == PRINT_USR_MSG) fprintf(stderr,##__VA_ARGS__);}


#endif//__typedef_h__
