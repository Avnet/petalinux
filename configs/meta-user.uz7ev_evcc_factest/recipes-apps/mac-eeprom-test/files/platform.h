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
// ----------------------------------------------------------------------------
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
// Module Name:         platform.h
// Design Name:         PicoZed FMC2 IIC MAC ID EEPROM Demonstration
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
//
//----------------------------------------------------------------------------

#ifndef PLATFORM_H_
#define PLATFORM_H_

/*
 * Define the AXI locations for the PL AXI I2C blocks used for the I2C slave
 * tests.  These come from xparameters.h in the standalone BSP or from the
 * AXI address defined in the hardware platform and these may need to get
 * updated every time this test gets ported to a new platform.
 */
#define XPAR_XIIC_NUM_INSTANCES            1
#define XPAR_AXI_IIC_0_BASEADDR            0x41600000

#define CONFIG_DEFAULT_MMAP_MIN_ADDR   4096

#endif // PLATFORM_H_
