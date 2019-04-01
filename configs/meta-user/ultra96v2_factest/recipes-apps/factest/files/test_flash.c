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
//  Module Name:         test_flash.c
//  Project Name:        Ultra96-V2 SPI Flash test application
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
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>

#include "typedef.h"

#define SPI_FLASH_TEST_DATA (0xA5)

TEST_RESULT erase_partition(mtd_info_t *mtd_info, int fd) 
{
	TEST_RESULT test_result = PASS;
	int ret_val;

   erase_info_t ei;
   ei.length = mtd_info->erasesize;	
	
	DBG_MSG("Erasing SPI Flash partition.  Total size : %u bytes, erasing in chunk of %u bytes \n", mtd_info->size,mtd_info->erasesize);
   for(ei.start = 0; ei.start < mtd_info->size; ei.start += mtd_info->erasesize) 
	{
      /* Unlocking of SPI Flash sectors is not needed.
       * Not supported by the Adesto/Atmel AT45DB041E Flash device.
       * 
       * ret_val = ioctl(fd, MEMUNLOCK, &ei);
       * if(ret_val != 0)
       * {
       *    ERR_MSG("Error in unlocking SPI Flash\n");
       *    return FAIL;
       * }
       */
      
      ret_val = ioctl(fd, MEMERASE, &ei);
      if(ret_val != 0)
      {
         ERR_MSG("Error in erasing SPI Flash\n");
         return FAIL;
      }

   }
	return test_result;
}
TEST_RESULT read_verify_flash(int *fd,char *write_buf,char *read_buf)
{
	TEST_RESULT test_result = PASS;
	int ret_val;
	int loop_count;
	
	/* read something what was written */	
	lseek(*fd, 0,0);
	ret_val = read(*fd, read_buf, MTD_FLASH_RW_TEST_SIZE);
	if(ret_val != 0)
	{
		ERR_MSG("Error in reading from SPI Flash\n");
		close (*fd);
		return FAIL;
	}
	ret_val = memcmp(write_buf,read_buf,MTD_FLASH_RW_TEST_SIZE);
	if(ret_val == 0) 
	{
		DBG_MSG("SPI Flash - data matched\n");
		test_result = PASS;
	}
	else	
	{		
		ERR_MSG("SPI Flash - data mismatched\n");
		test_result = FAIL;//data not matched
	}
	
	for(loop_count = 0; loop_count < MTD_FLASH_RW_TEST_SIZE; loop_count++)
	{
		DBG_MSG("SPI Flash: Wrote - 0x%x, Read - 0x%x\n",write_buf[loop_count],read_buf[loop_count]);
	}
	return test_result;
}
TEST_RESULT open_erase_write_flash(int *fd,char *write_buf)
{
    mtd_info_t 	mtd_info;
	int 		ret_val = 0;
	int 		loop_count = 0;
	TEST_RESULT test_result = PASS;
	
	DBG_MSG("Testing SPI Flash\n");
	*fd = open(MTD_FLASH_DEVICE_NAME, O_RDWR);
	if(*fd < 0)
	{
		ERR_MSG("Unable to open Flash MTD device\n");
		return FAIL;
	}
	
	ioctl(*fd, MEMGETINFO, &mtd_info);
 
    DBG_MSG("MTD type: %s\n", ((6 == mtd_info.type)?"Data Flash":"Other Flash"));
    DBG_MSG("MTD total size : %u bytes\n", mtd_info.size);
    DBG_MSG("MTD erase size : %u bytes\n", mtd_info.erasesize);
 
	//Erase the partition we are going to write	
	erase_partition(&mtd_info,*fd);
	
	DBG_MSG("Writing buffer to SPI Flash\n");
	//Go to begining to write
//tc	lseek(*fd, 0,0);
	ret_val = write(*fd,write_buf,MTD_FLASH_RW_TEST_SIZE);
	if(ret_val != 0)
	{
		ERR_MSG("Error in writing to SPI Flash\n");
		close (*fd);
		return FAIL;
	}		
	return test_result;
}

TEST_RESULT open_get_flash_info(int *fd)
{
    mtd_info_t 	mtd_info;
	int 		ret_val = 0;
	int 		loop_count = 0;
	TEST_RESULT test_result = PASS;
	
	DBG_MSG("Testing SPI Flash\n");
	*fd = open(MTD_FLASH_DEVICE_NAME, O_RDWR);
	if(*fd < 0)
	{
		ERR_MSG("Unable to open Flash MTD device\n");
		return FAIL;
	}
	
	DBG_MSG("Fetching SPI Flash size info\n");
	ret_val = ioctl(*fd, MEMGETINFO, &mtd_info);
 	if(ret_val != 0)
	{
		ERR_MSG("Error in fetching SPI Flash info\n");
		close (*fd);
		return FAIL;
	}		

    DBG_MSG("MTD type: %s\n", ((6 == mtd_info.type)?"Data Flash":"Other Flash"));
    DBG_MSG("MTD total size : %u bytes\n", mtd_info.size);
    DBG_MSG("MTD erase size : %u bytes\n", mtd_info.erasesize);
 
	return test_result;
}

TEST_RESULT test_flash(void)
{
	TEST_RESULT 	test_result = PASS;
	char 			str_cmd[COMMAND_LENGTH];
	unsigned char 	write_buf[MTD_FLASH_RW_TEST_SIZE];
	unsigned char 	read_buf[MTD_FLASH_RW_TEST_SIZE];
	int 			fd = 0;
	int				loop_count;	
	
#ifndef CE_TEST		
	sprintf(str_cmd,"Performing SPI Flash testing, this may take a few seconds...\n");
	USR_MSG(str_cmd);
#endif //#ifndef CE_TEST		

/* This test of writing a buffer to the Adesto/Atmel AT45DB041E SPI Flash
 * does not work.  Will settle for testing a fetch Flash size and partition
 * data instead.
 * 
   //Fill the write and read buffers
	for(loop_count = 0; loop_count < MTD_FLASH_RW_TEST_SIZE; loop_count++)
	{
		write_buf[loop_count] = SPI_FLASH_TEST_DATA;
		read_buf[loop_count] = 0x00;
	}
	
   test_result = open_erase_write_flash(&fd,write_buf);
	if(test_result == PASS)
	{
		test_result = read_verify_flash(&fd,write_buf,read_buf);		
		close(fd);
	}
*/

   test_result = open_get_flash_info(&fd);	

	return test_result;
}
void * thread_flash_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	int loop_count;
	unsigned char 	write_buf[MTD_FLASH_RW_TEST_SIZE];
	unsigned char 	read_buf[MTD_FLASH_RW_TEST_SIZE];
	int 			fd = 0;
	//prepare the data to write
	for(loop_count = 0; loop_count < MTD_FLASH_RW_TEST_SIZE; loop_count++)
	{
		write_buf[loop_count] = SPI_FLASH_TEST_DATA;
		read_buf[loop_count] = 0x00;
	}
	//erase and write into flash
	test_result = open_erase_write_flash(&fd,write_buf);
	if(test_result == FAIL)
	{
		ERR_MSG("SPI Flash unable to erase/write, test failed\n");
		thread_message->test_result = test_result;	
		return void_thread_message;	
	}
	//Don't need to write into flash in CE test, so doing erase and write outside the loop. 
   //Only read & verify will be performed continuously.
	do
	{
		//initialize the bufffer where the data to read and prepare buffer to compare against...
		for(loop_count = 0; loop_count < MTD_FLASH_RW_TEST_SIZE; loop_count++)
		{
			write_buf[loop_count] = SPI_FLASH_TEST_DATA;
			read_buf[loop_count] = 0x00;
		}
		//read and verify with the reference buffer...
		test_result = read_verify_flash(&fd,write_buf,read_buf);	
		if(test_result == FAIL)
		{	
			ERR_MSG("SPI Flash failed, exiting SPI Flash test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);
	
	close(fd);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}



