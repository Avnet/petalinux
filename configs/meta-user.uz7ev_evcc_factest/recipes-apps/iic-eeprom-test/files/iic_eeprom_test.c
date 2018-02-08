// ----------------------------------------------------------------------------
//
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
// ----------------------------------------------------------------------------//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
//
// Please direct any questions to the PicoZed community support forum:
//    http://www.picozed.org/forum
//
// Product information is available at:
//    http://www.picozed.org/product/picozed
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2015 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Nov 20, 2015
// Design Name:         PicoZed IIC EEPROM Test
// Module Name:         iic_eeprom_test.c
// Project Name:        PicoZed IIC EEPROM Test
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         EEPROM Test for PicoZed FMC2 Carrier
//
// Dependencies:
//
// Revision:            Nov 20, 2015: 1.00 Initial version
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include "platform.h"

#include "iic_eeprom_demo.h"

void print(const char *str);

// This is the top level demo instance.
iic_eeprom_demo_t demo;

int run_iic_eeprom_test(){
	int ret;
	int pass_fail = 0;

	   printf("\n\r");

	   printf("---------------------------------------------------------------------------\n\r");
	   printf("--                                                                       --\n\r");
	   printf("--                   PZCC-FMC2 IIC EEPROM Test Application               --\n\r");
	   printf("--                                                                       --\n\r");
	   printf("---------------------------------------------------------------------------\n\r");
	   printf("\n\r");

    /* Assign the address for the IIC controller which comes from the address
     * map for hardware peripherals on this platform.
     */
    demo.uBaseAddr_IIC_RTC        = XPAR_AXI_IIC_0_BASEADDR;

    // Initialize hardware design and prepare devices for normal operation.
    ret = zed_iic_eeprom_demo_init(&demo);
    if (ret != 0){
    	 printf("zed_iic_eeprom_demo_init failed! Test ABORT!\n\r");
    	 pass_fail = -1;
    }

    ret = validate_iic_eeprom_memory(&demo);

    ret = dump_iic_eeprom_memory(&demo);
    if (ret != 0){
    	 printf("dump_iic_eeprom_memory failed! Test ABORT!\n\r");
    	 pass_fail = -1;
    }

    ret = erase_iic_eeprom_memory(&demo);
	if (ret != 0){
		 printf("erase_iic_eeprom_memory failed! Test ABORT!\n\r");
		 pass_fail = -1;
	}

	ret = dump_iic_eeprom_memory(&demo);
	if (ret != 0){
		 printf("dump_iic_eeprom_memory failed! Test ABORT!\n\r");
		 pass_fail = -1;
	}

	ret = default_iic_eeprom_memory(&demo);
	if (ret != 0){
		 printf("erase_iic_eeprom_memory failed! Test ABORT!\n\r");
		 pass_fail = -1;
	}

	ret = dump_iic_eeprom_memory(&demo);
	if (ret != 0){
		 printf("dump_iic_eeprom_memory failed! Test ABORT!\n\r");
		 pass_fail = -1;
	}
    /* Determine overall test result and print results to test operator.
     */
    printf("\r\n");
    printf("IIC EEPROM Test: ");

    if (pass_fail == 0)
    {
        printf("\033[32mPASSED\033[0m\r\n");
        return 0;
    }
    else
    {
        printf("\033[5mFAILED\033[0m\r\n");
        return -1;
    }

    return pass_fail;
}
