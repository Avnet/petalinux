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
// Tool versions:       Vivado 2016.2
//						Petalinux 2016.2
// File Name:           gpio.h
// Description:         PL GPIO loopback Linux test routines.
//
//----------------------------------------------------------------------------

#ifndef GPIO_H
#define GPIO_H

// ---------------------------------------------------------------------------
// Local include Files.
#include "pl_gpio_udriver.h"
#include "platform.h"
#include "types.h"

// Constant definitions.
// ---------------------------------------------------------------------------
#define GPIO3_BIT_MASK					0x000000FF
#define GPIO3_BIT_WIDTH				8

#define GPIO4_BIT_MASK					0x0003FFFF
#define GPIO4_BIT_WIDTH				18

#define GPIO5_BIT_MASK					0x0003FFFF
#define GPIO5_BIT_WIDTH				18

// ---------------------------------------------------------------------------
// Function prototype declarations.
int gpio_setup(int32u verbosity);
int gpio_test(int32u test_pattern, int32u verbosity);
int gpio_walking_ones(int32u verbosity);
int gpio_walking_zeros(int32u verbosity);
int gpio_all_zeros(int32u verbosity);
int gpio_all_ones(int32u verbosity);

#endif // GPIO_H
