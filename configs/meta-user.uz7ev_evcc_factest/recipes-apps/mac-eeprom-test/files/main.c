//----------------------------------------------------------------------------
//      _____
//     *     *
//    *____   *____
//   * *===*   *==*
//  *___*===*___**  AVNET
//       *======*
//        *====*
//----------------------------------------------------------------------------
//
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
// Create Date:         Nov 24, 2015
// Design Name:         PicoZed FMC2 IIC MAC ID EEPROM Demonstration
// Module Name:         main.c
// Project Name:        PicoZed FMC2 IIC MAC ID EEPROM Demonstration
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         I2C MAC ID Demonstration for PicoZed FMC2 Carrier
//
// Dependencies:
//
// Revision:            Nov 24, 2015: 1.00 Initial version
//                      Apr 06, 2016: 1.01 Adapted to run under Linux
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include "platform.h"

// Number of times an interrupt is received before the test passes.
#define TEST_INTERRUPT_COUNT                     10
//#define TEST_INTERRUPT_COUNT                     (60 * 60 * 24)

void print(const char *str);

int run_mac_eeprom_test();

int main()
{
	int ret_val = 0;

	ret_val = run_mac_eeprom_test();

    return ret_val;
}
