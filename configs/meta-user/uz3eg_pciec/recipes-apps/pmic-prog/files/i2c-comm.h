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
//  This design is the property of Avnet.  Publication of this
//  design is not authorized without written consent from Avnet.
//
//  Please direct any questions to the Zed community support forum:
//     http://zedboard.org/forums/zed-english-forum
//
//  Product information is available at:
//     http://zedboard.org/
//
//  Disclaimer:
//     Avnet, Inc. makes no warranty for the use of this code or design.
//     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//     any errors, which may appear in this code, nor does it make a commitment
//     to update the information contained herein. Avnet, Inc specifically
//     disclaims any implied warranties of fitness for a particular purpose.
//                      Copyright(c) 2018 Avnet, Inc.
//                              All rights reserved.
//
// ----------------------------------------------------------------------------
//
//  Create Date:         Jun 16, 2020
//  Design Name:         PMIC PROGRAMMER
//  Module Name:         i2c-comm.h
//  Project Name:        PMIC PROGRAMMER
//
//  Description:         Application to program Infineon PMICs
//
//  Dependencies:
//
//  Revision:            Jun 16, 2020: 1.0 Initial version
//
// ----------------------------------------------------------------------------

#ifndef I2C_COMM_H_
#define I2C_COMM_H_

#include <stdint-gcc.h>
#include <stdbool.h>

bool i2c_comm_open_device ( uint8_t i2c_bus, uint8_t device_addr );
bool i2c_comm_detect ( uint8_t i2c_bus, uint8_t *dev_addresses_table );
bool i2c_comm_close_device ( void );
bool i2c_comm_read_byte ( uint8_t address, uint8_t *res );
bool i2c_comm_read_word ( uint8_t address, uint16_t *res );
bool i2c_comm_write_byte ( uint8_t address, uint8_t value );
bool i2c_comm_write_word ( uint8_t address, uint16_t value );

#endif /* I2C_COMM_H_ */
