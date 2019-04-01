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
//  Module Name:         common_util.c
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
#include "test_zcu100.h"
#include "typedef.h"
TEST_RESULT check_user_response(void)
{
	TEST_RESULT test_result = PASS;
	char		user_input;
	
	do
	{
		user_input = getchar();
		if (user_input == '\n') 
		{
			user_input = getchar();			
		}
		while(user_input != 'n' && user_input != 'N' && user_input != 'y' && user_input != 'Y')
		{
			USR_MSG("\ninvalid input, enter the choice(y/Y/n/N) again : ");
			user_input = getchar();			
			if (user_input == '\n') 
			{
				user_input = getchar();				
			}
		}
	}while(user_input != 'n' && user_input != 'N' && user_input != 'y' && user_input != 'Y');
	if(user_input == 'n' || user_input == 'N') 
		test_result = FAIL;
	USR_MSG("\n");	
	return test_result;
}
void print_single_line(void)
{
	char loop = 0;
	
	for(loop = 0;loop < LINE_PRINT_LENGTH; loop++)
	{
		USR_MSG("-");
	}
	USR_MSG("\n");
}
void print_double_line(void)
{
	char loop = 0;
	
	for(loop = 0;loop < LINE_PRINT_LENGTH; loop++)
	{
		USR_MSG("=");
	}
	USR_MSG("\n");
}

