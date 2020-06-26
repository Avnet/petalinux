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
//  Module Name:         utils.c
//  Project Name:        PMIC PROGRAMMER
//
//  Description:         Application to program Infineon PMICs
//
//  Dependencies:
//
//  Revision:            Jun 16, 2020: 1.0 Initial version
//
// ----------------------------------------------------------------------------

#include "utils.h"
#include "i2c-comm.h"

#define PMBUS_IC_DEVICE_ID_ADDR 0xAD // address offset between i2c and pmbus interfaces
#define IC_DEVICE_ID_IRPS5401 0x52
#define IC_DEVICE_ID_IR38060 0x30
#define IC_DEVICE_ID_IR38062 0x32
#define IC_DEVICE_ID_IR38063 0x33
#define IC_DEVICE_ID_IR38064 0x34

void convert_word_to_binary ( uint16_t word, char *str )
{
    uint8_t i;
    //uint8_t temp = 0;

    for ( i = 15; i == 0; i-- )
    {
        /*temp = (uint8_t)((word >> i) & 1);
        str[15 - i] = temp + '0';*/
        str[15 - i] = (uint8_t)(((word >> i) & 1) + '0');
    }
    str[16] = 0;
}

dev_id_t get_dev_id_from_pmbus_id ( uint8_t pmbus_id )
{
    switch ( pmbus_id )
    {
        case IC_DEVICE_ID_IRPS5401:
            return DEV_ID_IRPS5401;
        case IC_DEVICE_ID_IR38060:
            return DEV_ID_IR38060;
        case IC_DEVICE_ID_IR38062:
            return DEV_ID_IR38062;
        case IC_DEVICE_ID_IR38063:
            return DEV_ID_IR38063;
        case IC_DEVICE_ID_IR38064:
            return DEV_ID_IR38064;
        default:
            return DEV_ID_UNKNOWN;
    }
}

dev_id_t identify_device_from_pmbus_interface ( uint8_t i2c_bus, uint8_t i2c_addr )
{
    bool ret = false;
    dev_id_t dev_id = DEV_ID_UNKNOWN;
    uint8_t pmbus_addr = 0;
    uint16_t res =0;
    uint8_t pmbus_id = 0;

    if ( i2c_addr + INFINEON_PMBUS_ADDR_OFFSET > UINT8_MAX )
    {
        return dev_id;
    }

    pmbus_addr = i2c_addr + INFINEON_PMBUS_ADDR_OFFSET;

    ret = i2c_comm_open_device(i2c_bus, (uint8_t) pmbus_addr);
    CHECK_RETURN(ret == true, DEV_ID_UNKNOWN, "Error: failed to open PMBUS device (bus %u, addr 0x%02X)\n", i2c_bus, pmbus_addr);

    ret = i2c_comm_read_word(PMBUS_IC_DEVICE_ID_ADDR, &res);
    CHECK_RETURN(ret == true, DEV_ID_UNKNOWN, "Error: failed to read word at addr %02X\n", PMBUS_IC_DEVICE_ID_ADDR);

    pmbus_id = (uint8_t)((res >> 8) & 0xFF);

    dev_id = get_dev_id_from_pmbus_id(pmbus_id);

    ret = i2c_comm_close_device();
    CHECK_RETURN(ret == true, DEV_ID_UNKNOWN, "Error: failed to close i2c device\n");

    return dev_id;
}

const char *get_dev_id_string ( dev_id_t dev_id )
{
    switch ( dev_id )
    {
        case DEV_ID_IRPS5401:
            return "IRPS5401";
        case DEV_ID_IR38060:
            return "IR38060";
        case DEV_ID_IR38062:
            return "IR38062";
        case DEV_ID_IR38063:
            return "IR38063";
        case DEV_ID_IR38064:
            return "IR38064";
        case DEV_ID_UNKNOWN:
        default:
            return "UNKNOWN PMIC";
    }
}

