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
// Please direct any questions to:  technical.support@avnet.com
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2013 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Nov 17, 2016
// Tool versions:       Vivado 2016.2 / Petalinux 2016.2
// File Name:           platform.h
// Description:         Programmable logic hardware platform definitions file.
//
//----------------------------------------------------------------------------
#ifndef PLATFORM_H_
#define PLATFORM_H_

/*
 * Define the AXI locations for all the PL GPIO blocks used for the GPIO
 * loopback tests.  These come from xparameters.h in the standalone BSP or
 * from the AXI address defined in XPS and these may need to get updated
 * everytime this test gets ported to a new platform.
 */
#define GPIO3_DEVICE_ADDRESS      0xA0030000
#define GPIO4_DEVICE_ADDRESS      0xA0040000
#define GPIO5_DEVICE_ADDRESS      0xA0050000

#define CONFIG_DEFAULT_MMAP_MIN_ADDR   4096

#endif // PLATFORM_H_
