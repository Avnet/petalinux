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
//  Module Name:         test_eeprom.c
//  Project Name:        Ultra96-V2 I2C EEPROM test application
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
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include "typedef.h"


const char *I2C_EEPROM_name_string[] = {"2-0054","3-0054","4-0054","5-0054"};

TEST_RESULT perform_read_write_verify_eeprom(char *file_name)
{
	unsigned char read_eeprom_data;
	unsigned char write_eeprom_data = TEST_PATTERN;
	int  		bytes_write_read;
	int  		i;
	FILE 		*fpEEPROM = NULL;
	TEST_RESULT	test_result = PASS;

	/* Create the eeprom Image */
	DBG_MSG("opening eeprom file %s, for writing\n",file_name);
	fpEEPROM = fopen(file_name,"w");		
	if (!fpEEPROM )
	{
		ERR_MSG("error in opening file %s\n",file_name);
		return FAIL;
	}
	for (i = 0; i < EEPROM_TEST_SIZE; i++)
	{	
		DBG_MSG("wrote 0x%x, for %d times\n",write_eeprom_data,i);
		fprintf(fpEEPROM,"%c",write_eeprom_data);
	}
	if(fpEEPROM) 
	{
		fclose(fpEEPROM);
		fpEEPROM = NULL;
	}
	DBG_MSG("closing eeprom file %s, after writing\n",file_name);

	DBG_MSG("opening eeprom file %s, for reading\n",file_name);
	fpEEPROM = fopen(file_name,"r");
	if (!fpEEPROM )
	{
		ERR_MSG("error in opening file %s\n",file_name);
		return FAIL;
	}
	for (i = 0; i < EEPROM_TEST_SIZE; i++)
	{
		fscanf(fpEEPROM,"%c",&read_eeprom_data);					
		if (read_eeprom_data != TEST_PATTERN)
		{
			ERR_MSG("Data Read - 0x%x, expected - 0x%x\n",read_eeprom_data,TEST_PATTERN);
			test_result = FAIL;
			break;
		}
		DBG_MSG("EEPROM Data Matched - 0x%x\n",read_eeprom_data);
	}
	if(fpEEPROM) 
	{
		fclose(fpEEPROM);
		fpEEPROM = NULL;
	}
	DBG_MSG("closing eeprom file %s, after reading\n",file_name);
		
	return test_result;
}

TEST_RESULT test_eeprom(I2C_EEPROM i2c_eeprom_number)
{	
	int 	ret_val = 0;	
	char 	file_name[COMMAND_LENGTH];	
	TEST_RESULT test_result = PASS;
	
	sprintf(file_name,"%s/%s/%s",EEPROM_PATH,I2C_EEPROM_name_string[i2c_eeprom_number],EEPROM_FILE_NAME);
	
	DBG_MSG("performing read/write/verify in EEPROM - %d\n",i2c_eeprom_number);		
	test_result = perform_read_write_verify_eeprom(file_name);
	
	return test_result;
	
}

void * thread_eeprom_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	do
	{
		test_result = test_eeprom(thread_message->function_data);		
		if(FAIL == test_result)
		{	
			ERR_MSG("I2C EEPROM Test failed, exiting I2C EEPROM test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}
