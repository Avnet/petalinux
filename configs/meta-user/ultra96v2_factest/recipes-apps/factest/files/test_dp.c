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
//  Module Name:         test_dp.c
//  Project Name:        Ultra96-V2 DisplayPort test application
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
#include <linux/fb.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "test_zcu100.h"
#include "typedef.h"

#define COLOR_OFFSET (0x10)

uint32_t pixel_color(uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo *vinfo)
{
	return (r<<vinfo->red.offset) | (g<<vinfo->green.offset) | (b<<vinfo->blue.offset);
}

int test_gpu()
{
	/* For GPU in CE test we are running CTS testsuite using fbdev */
	system("echo -e '#!/bin/bash\ncd ~/OpenGL-ES-CTS/cts/\n./cts-runner --type=es2 > /dev/null' > start_cts.sh");
	system("sh start_cts.sh");
	return 0;
}

void fill_display(void)
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	static uint8_t red = 0;
	static uint8_t green =0;
	static uint8_t blue = 0;

	int fb_fd = 0;
	
	fb_fd = open("/dev/fb0",O_RDWR);

	//Get variable screen information
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	vinfo.grayscale=0;
	vinfo.bits_per_pixel=32;
	
	ioctl(fb_fd, FBIOPUT_VSCREENINFO, &vinfo);
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	
	ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);

	long screensize = vinfo.yres_virtual * finfo.line_length;

	uint8_t *fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, (off_t)0);

	int x,y;

	for (x=0;x<vinfo.xres;x++)
	{
		for (y=0;y<vinfo.yres;y++)
		{
			long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
			*((uint32_t*)(fbp + location)) = pixel_color(red,green,blue, &vinfo);
		}
	}
	red += COLOR_OFFSET; 	if(red > 0xFF) 		red = 0;
	blue += COLOR_OFFSET; 	if(blue > 0xFF) 	blue = 0;
	green += COLOR_OFFSET; 	if(green > 0xFF) 	green = 0;
	
	close(fb_fd);
	return;
}
TEST_RESULT test_dp(void)
{
	char 		cmd_string[COMMAND_LENGTH];
	char 		str_ask_user[COMMAND_LENGTH];
	int 		ret_val = 0;
	TEST_RESULT test_result = PASS;
	int 		loop = 0;
	/* petalinux should display stdout console on the attached DisplayPort monitor */
	sprintf(str_ask_user,"Display should display text console output. \n\rEnter 'y' (for displaying command console on screen) or 'n'\t:\t");
	USR_MSG(str_ask_user);
	test_result = check_user_response();

	return test_result;
}
void *thread_dp_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	int tmp_count = 0;
	do
	{
		test_result = test_gpu();		
		if(FAIL == test_result)
		{	
			ERR_MSG("DP Test failed, exiting DP test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);		
	}while(keep_threads_running);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}
