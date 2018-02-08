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
// Create Date:         Jun 24, 2013
// Tool versions:       SDK 14.5
// File Name:           pl_gpio_udriver.h
// Description:         PL AXI GPIO mmap driver.
//
//----------------------------------------------------------------------------

#ifndef PL_GPIO_UDRIVER_H
#define PL_GPIO_UDRIVER_H

// ---------------------------------------------------------------------------
// System include Files.
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// ---------------------------------------------------------------------------
// Local include Files.
#include "platform.h"
#include "types.h"

// Define the registers in the AXI PL GPIO controller
#define PL_GPIO_DATA         0x00
#define PL_GPIO_TRI          0x04
#define PL_GPIO2_DATA        0x08
#define PL_GPIO2_TRI         0x0C

// Define the possible channels in the AXI PL GPIO controller block.
#define PL_GPIO_CHANNEL1     0
#define PL_GPIO_CHANNEL2     1

// Define the memory size of the AXI PL GPIO controller block.
#define PL_GPIO_SIZE	     CONFIG_DEFAULT_MMAP_MIN_ADDR

// Define register read write macros.
#define REG_WRITE(addr, off, val) (*(volatile int*)(addr+off)=(val))
#define REG_READ(addr,off) (*(volatile int*)(addr+off))

int pl_gpio_data_read(int32u pl_gpio_instance_physical_address, int channel, int32u* data);
int pl_gpio_data_write(int32u pl_gpio_instance_physical_address, int channel, int32u* data);
int pl_gpio_initialize(int32u* pl_gpio_pointer, int32u pl_gpio_instance_physical_address);
int pl_gpio_set_data_direction(int32u pl_gpio_instance_physical_address, int channel, int32u direction_mask);

#endif // PL_GPIO_UDRIVER_H
