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
//  Module Name:         test_sd.c
//  Project Name:        Ultra96-V2 micro SD card test application
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
#include <string.h>
#include <sys/mount.h>

#include "typedef.h"
#include "test_zcu100.h"


TEST_RESULT perform_read_write_verify(FILE *fp)
{
	char write_string[COMMAND_LENGTH];
	char read_string[COMMAND_LENGTH];
	int  bytes_write_read;
	
	strcpy(write_string,TEST_STRING_MESSAGE);
	bytes_write_read = 0;
	bytes_write_read = fwrite(write_string,1,strlen(write_string)+1,fp);
	if(bytes_write_read < strlen(write_string)+1)
	{
		ERR_MSG("Unable to write to File, wrote %d, had to write - %d\n",bytes_write_read,strlen(write_string)+1);
		return FAIL;
	}

	//take the file pointer back so that we can read what we wrote. Seek to the beginning of the file */
	fseek(fp, SEEK_SET, 0);
	bytes_write_read = 0;
	bytes_write_read = fread(read_string,1,strlen(write_string)+1,fp);
	if(bytes_write_read < strlen(write_string)+1)
	{
		ERR_MSG("Unable to read from File, read %d, had to read - %d\n",bytes_write_read,strlen(write_string)+1);
		return FAIL;
	}
	
	if(!strcmp(write_string,read_string))
	{
		DBG_MSG("Data matched\n");
		return PASS;
	}
	else
	{
		ERR_MSG("Data Mismatched\n");
		return FAIL;
	}
}
TEST_RESULT mount_sd(FILE **fp, char *file_name,char 	mount_dir)
{
	TEST_RESULT test_result = PASS;
	char system_command[COMMAND_LENGTH];
	int ret_val;
	
	if(!strcmp(MOUNT_DIR,""))
	{
		DBG_MSG("System booted from SD card, so no need to mount SD card.\n");
		DBG_MSG("Root FS is RAM disk or SD card partition\n");
	}
	else
   {
      DBG_MSG("System not booted from SD card, so need to mount SD card\n");
		mount_dir = TRUE;
   }

	if(mount_dir == TRUE)
	{//not booting from sd and rootfs is not on SD.  Need to mount SD.
		DBG_MSG("Need to mount SD card\n");
		sprintf(system_command,"mkdir %s",MOUNT_DIR);
		ret_val = system(system_command);
		if(ret_val != 0 )
		{
			ERR_MSG("Unable to create Dir %s",MOUNT_DIR);
			return FAIL;
		}

		ret_val = mount(SD_DEV,MOUNT_DIR,MOUNT_FS_TYPE,0,0) ;				
		if(0 != ret_val)	
		{
			ERR_MSG("Unable to mount %s on %s\n",SD_DEV,MOUNT_DIR);
			
			sprintf(system_command,"sudo rm -rf %s",MOUNT_DIR);
			DBG_MSG("removing mount directory \"%s\"\n",system_command);			
			system(system_command);

			return FAIL;
		}
	}

	sprintf(file_name,"%s/sd_test_file.txt",MOUNT_DIR);
	DBG_MSG("creating test file - %s\n",file_name);

	*fp = fopen(file_name,"w+");
	if(!*fp)
	{
		ERR_MSG("Unable to create File - %s\n",file_name);
		if(TRUE == mount_dir)
		{
			sprintf(system_command,"umount %s",MOUNT_DIR);
			system(system_command);
			
			sprintf(system_command,"rm -rf %s",MOUNT_DIR);
			system(system_command);
		}
		return FAIL;
	}
	return test_result;
}
TEST_RESULT unmount_sd(FILE *fp,char *file_name,char 	mount_dir)
{
	char system_command[COMMAND_LENGTH];
	int ret_val;
	TEST_RESULT test_result = PASS;
	if(fp)
	{
		fclose(fp);
		ret_val = remove(file_name);
		if(ret_val == 0)
		{
			DBG_MSG("Removed Test File - %s\n",file_name);
		}
		else
		{
			ERR_MSG("Error Removing file - %s\n",file_name);
		}
	}

	if(TRUE == mount_dir)
	{
		sprintf(system_command,"umount %s",MOUNT_DIR);
		system(system_command);
		
		sprintf(system_command,"rm -rf %s",MOUNT_DIR);
		system(system_command);
	}
	return test_result;
}
TEST_RESULT test_sd(void)
{
	FILE 	*fp = NULL;
	char 	file_name[COMMAND_LENGTH];
	char 	system_command[COMMAND_LENGTH];
	int		ret_val;
	char 	mount_dir = FALSE;	
	TEST_RESULT test_result = PASS;
	
	DBG_MSG("Make sure that SD card is inserted in the board\n");	
	
	test_result = mount_sd(&fp,file_name,mount_dir);
	if(FAIL == test_result)
	{
		ERR_MSG("Unable to mount SD Card and Create File on SD Card\n");
		return test_result;
	}
	
	test_result = perform_read_write_verify(fp);
	
	unmount_sd(fp,file_name,mount_dir);
	return test_result;
}


void * thread_sd_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT 	test_result = PASS;
	char 			file_name[COMMAND_LENGTH];
	FILE			*fp = NULL;
	char 	mount_dir = FALSE;
	test_result = mount_sd(&fp,file_name,mount_dir);
	do
	{
		test_result = perform_read_write_verify(fp);		
		if(FAIL == test_result)
		{	
			ERR_MSG("SD Test failed, exiting SD test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);
	unmount_sd(fp,file_name,mount_dir);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}

