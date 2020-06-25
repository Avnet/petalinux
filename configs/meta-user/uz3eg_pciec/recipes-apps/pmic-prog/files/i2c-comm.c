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
//  Module Name:         i2c-comm.c
//  Project Name:        PMIC PROGRAMMER
//
//  Description:         Application to program Infineon PMICs
//
//  Dependencies:
//
//  Revision:            Jun 16, 2020: 1.0 Initial version
//
// ----------------------------------------------------------------------------

#include "i2c-comm.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <i2c/smbus.h>

#define DETECT_FIRST_ADDR   0x03
#define DETECT_LAST_ADDR    0x6F

static int i2c_dev_file = 0;

static bool open_i2c_bus ( uint8_t i2c_bus );
static bool close_i2c_bus ( void );
static bool set_slave_dev ( uint8_t device_addr );

/* ***************************************************************************/
/* Public functions *********************************************************/
/* ***************************************************************************/

bool i2c_comm_open_device ( uint8_t i2c_bus, uint8_t device_addr )
{
    CHECK_RETURN(i2c_dev_file == 0, false, "Error: an i2c device is already opened\n");

    bool ret = false;

    ret = open_i2c_bus(i2c_bus);
    CHECK_RETURN(ret == true, false, "Error opening i2c bus (%d): %s\n", i2c_bus, strerror(errno));

    ret = set_slave_dev(device_addr);
    if ( ret == false )
    {
        fprintf(stderr, "Error setting slave addr (0x%02X): %s\n", device_addr, strerror(errno));
        close_i2c_bus();
        return false;
    }

    return true;
}

bool i2c_comm_close_device ( void )
{
    return close_i2c_bus();
}

bool i2c_comm_detect ( uint8_t i2c_bus, uint8_t *dev_addresses_table )
{
    ASSERT_EMERG(dev_addresses_table != NULL, "%s: NULL parameter\n", __FUNCTION__);

    bool ret = false;
    int res;
    uint8_t addr = 0 ;

    ret = open_i2c_bus(i2c_bus);
    CHECK_RETURN(ret == true, false, "Error opening i2c bus (%d): %s\n", i2c_bus, strerror(errno));

    printf("i2c devices: ");
    for ( addr = DETECT_FIRST_ADDR; addr <= DETECT_LAST_ADDR; addr++ )
    {
        if ( addr >= 0x50 && addr <= 0x57 )
        {
            // skip EEPROM addresses
            continue;
        }

        ret = set_slave_dev(addr);
        if ( ret == false )
        {
            fprintf(stderr, "Error setting slave addr (0x%02X): %s\n", addr, strerror(errno));
            close_i2c_bus();
            return false;
        }

        res = i2c_smbus_read_byte(i2c_dev_file);

        if ( res < 0 )
        {
            dev_addresses_table[addr] = 0;
        }
        else
        {
            printf("0x%02X ", addr);
            dev_addresses_table[addr] = 1;
        }
    }

    printf("\n\n");

    close_i2c_bus();
    return true;
}

bool i2c_comm_read_byte ( uint8_t address, uint8_t *res )
{
    ASSERT_EMERG(res != NULL, "%s: NULL parameter\n", __FUNCTION__);

    int32_t res32 = 0;

    CHECK_RETURN(i2c_dev_file > 0, false, "Error: No i2c device opened\n");

    res32 = i2c_smbus_read_byte_data(i2c_dev_file, address);
    if ( res32 < 0 )
    {
        return false;
    }

    *res = (uint8_t) (res32 & 0xFF);
    return true;
}

bool i2c_comm_read_word ( uint8_t address, uint16_t *res )
{
    ASSERT_EMERG(res != NULL, "%s: NULL parameter\n", __FUNCTION__);

    int32_t res32 = 0;

    CHECK_RETURN(i2c_dev_file > 0, false, "Error: No i2c device opened\n");

    res32 = i2c_smbus_read_word_data(i2c_dev_file, address);
    if ( res32 < 0 )
    {
        return false;
    }

    *res = (uint16_t) (res32 & 0xFFFF);
    return true;
}

bool i2c_comm_write_byte ( uint8_t address, uint8_t value )
{
    int32_t res32 = 0;

    CHECK_RETURN(i2c_dev_file > 0, false, "Error: No i2c device opened\n");

    res32 = i2c_smbus_write_byte_data(i2c_dev_file, address, value);
    if ( res32 < 0 )
    {
        return false;
    }

    return true;
}

bool i2c_comm_write_word ( uint8_t address, uint16_t value )
{
    int32_t res32 = 0;

    CHECK_RETURN(i2c_dev_file > 0, false, "Error: No i2c device opened\n");

    res32 = i2c_smbus_write_word_data(i2c_dev_file, address, value);
    if ( res32 < 0 )
    {
        return false;
    }

    return true;
}

/* ***************************************************************************/
/* Private functions *********************************************************/
/* ***************************************************************************/

static bool open_i2c_bus ( uint8_t i2c_bus )
{
    CHECK_RETURN(i2c_dev_file == 0, false, "Error: an i2c device is already opened\n");

    char filename[30] = { 0 };

    sprintf(filename, "/dev/i2c-%u", i2c_bus);

    i2c_dev_file = open(filename, O_RDWR);
    CHECK_RETURN(i2c_dev_file > 0, false, "Error opening i2c bus (%s): %s\n", filename, strerror(errno));

    return true;
}

static bool close_i2c_bus ( void )
{
    CHECK_RETURN(i2c_dev_file != 0, false, "Error: no i2c device opened\n");

    close(i2c_dev_file);
    i2c_dev_file = 0;
    return true;
}

static bool set_slave_dev ( uint8_t device_addr )
{
    CHECK_RETURN(i2c_dev_file != 0, false, "Error: no i2c bus opened\n");

    int32_t ret = 0;

    ret = ioctl(i2c_dev_file, I2C_SLAVE_FORCE, device_addr);
    CHECK_RETURN(ret >= 0, false, "Error configuring i2c device (0x%02x) %s\n", device_addr, strerror(errno));

    return true;
}
