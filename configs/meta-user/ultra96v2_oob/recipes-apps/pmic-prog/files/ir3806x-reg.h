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
//  Module Name:         ir3806x-reg.h
//  Project Name:        PMIC PROGRAMMER
//
//  Description:         Application to program Infineon PMICs
//
//  Dependencies:
//
//  Revision:            Jun 16, 2020: 1.0 Initial version
//
// ----------------------------------------------------------------------------

#ifndef IR3806x_REG_H_
#define IR3806x_REG_H_

#define NUMBER_OF_REGISTERS 0xFF

// Special registers

#define MTP_PROGRAMMING_CLK_ADDR 0x80
#define MTP_COMMAND_ADDR 0xB0

#define CRC_CHECK_ADDR 0x97

#define NVM_PTR_USER 0x99
#define USER_PROGRAMMING_MAX_INDEX 8

typedef struct
{
    uint8_t first;
    uint8_t last;
} write_registers_ranges_t;

static write_registers_ranges_t w_registers_ranges[] =
{
    {0x20, 0x6D},
    {0x70, 0x70},
};

typedef struct
{
    uint8_t reg_addr;
    uint8_t mask;
} read_registers_t;

static read_registers_t r_registers[] =
{
    {0x20, 0xFF},
    {0x21, 0xFF},
    {0x22, 0xFF},
    {0x23, 0xFF},
    {0x24, 0xFF},
    {0x25, 0xFF},
    {0x26, 0xFF},
    {0x27, 0xFF},
    {0x28, 0xFF},
    {0x29, 0xFF},
    {0x2A, 0xFF},
    {0x2B, 0xFF},
    {0x2C, 0xFF},
    {0x2D, 0xFF},
    {0x2E, 0xFF},
    {0x2F, 0xFF},
    {0x30, 0xFF},
    {0x31, 0xFF},
    {0x32, 0xFF},
    {0x33, 0xFF},
    {0x34, 0xFF},
    {0x35, 0xFF},
    {0x36, 0xFF},
    {0x37, 0xFF},
    {0x38, 0xFF},
    {0x39, 0xFF},
    {0x3A, 0xFF},
    {0x3B, 0xFF},
    {0x3C, 0xFF},
    {0x3D, 0xFF},
    {0x3E, 0xFF},
    {0x3F, 0xFF},
    {0x40, 0xFF},
    {0x41, 0xFF},
    {0x42, 0xFF},
    {0x43, 0xFF},
    {0x44, 0xFF},
    {0x45, 0xFF},
    {0x46, 0xFF},
    {0x47, 0xFF},
    {0x48, 0xFF},
    {0x49, 0xFF},
    {0x4A, 0xFF},
    {0x4B, 0xFF},
    {0x4C, 0xFF},
    {0x4D, 0xFF},
    {0x4E, 0xFF},
    {0x4F, 0xFF},
    {0x50, 0xFF},
    {0x51, 0xFF},
    {0x52, 0xFF},
    {0x53, 0xFF},
    {0x54, 0xFF},
    {0x55, 0xFF},
    {0x56, 0xFF},
    {0x57, 0xFF},
    {0x58, 0xFF},
    {0x59, 0xFF},
    {0x5A, 0xFF},
    {0x5B, 0xFF},
    {0x5C, 0xFF},
    {0x5D, 0xFF},
    {0x5E, 0xFF},
    {0x5F, 0xFF},
    {0x60, 0xFF},
    {0x61, 0xFF},
    {0x62, 0xFF},
    {0x63, 0xFF},
    {0x64, 0xFF},
    {0x65, 0xFF},
    {0x66, 0xFF},
    {0x67, 0xFF},
    {0x68, 0xFF},
    {0x69, 0xFF},
    {0x6A, 0xFF},
    {0x6B, 0xFF},
    {0x6C, 0xFF},
    {0x6D, 0xFF},
    {0x70, 0xFF},
};

#endif /* IR3806x_REG_H_ */
