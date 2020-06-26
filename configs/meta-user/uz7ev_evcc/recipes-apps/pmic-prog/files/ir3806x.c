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
//  Module Name:         ir3806x.c
//  Project Name:        PMIC PROGRAMMER
//
//  Description:         Application to program Infineon PMICs
//
//  Dependencies:
//
//  Revision:            Jun 16, 2020: 1.0 Initial version
//
// ----------------------------------------------------------------------------

#include "ir3806x.h"
#include "ir3806x-reg.h"
#include "i2c-comm.h"
#include "utils.h"

#include "unistd.h"
#include "ctype.h"

typedef struct
{
    uint8_t reg_addr;
    uint8_t value;
    uint8_t mask;
} registers_t;

static void init_register ( registers_t *array );
static bool parse_file (const char *filename, registers_t *array );
static uint16_t count_diff ( registers_t *arrayA, registers_t *arrayB );
static bool check_crc ( void );
static bool check_mtp_user_left ( uint8_t *next_image_index );
static bool write_all_registers ( const char *input_filename );

/* ***************************************************************************/
/* Public functions *********************************************************/
/* ***************************************************************************/

bool ir3806x_read_byte ( uint16_t address, uint8_t *res )
{
    ASSERT_EMERG(res != NULL, "%s: NULL parameter\n", __FUNCTION__);

    bool ret = false;

    CHECK_RETURN(address <= UINT8_MAX, false, "Error: max address for IR3806x is %02X\n", UINT8_MAX);

    ret = i2c_comm_read_byte(address, res);
    CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %02X\n", address);

    return true;
}

bool ir3806x_write_byte ( uint16_t address, uint8_t value )
{
    bool ret = false;

    CHECK_RETURN(address <= UINT8_MAX, false, "Error: max address for IR3806x is %02X\n", UINT8_MAX);

    // from datasheet, we should never write registers 6Eh, 6F, 71h, 72h
    CHECK_RETURN(address != 0x6E, false, "Error: Forbidden address for IR3806x is %02X\n", address);
    CHECK_RETURN(address != 0x6F, false, "Error: Forbidden address for IR3806x is %02X\n", address);
    CHECK_RETURN(address != 0x71, false, "Error: Forbidden address for IR3806x is %02X\n", address);
    CHECK_RETURN(address != 0x72, false, "Error: Forbidden address for IR3806x is %02X\n", address);

    ret = i2c_comm_write_byte(address, value);
    CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %02X\n", address);

    return true;
}

bool ir3806x_read_all_registers ( const char *output_filename, const char *input_filename )
{
    registers_t registers_storage[NUMBER_OF_REGISTERS] = { 0 };
    registers_t registers_input[NUMBER_OF_REGISTERS] = { 0 };
    uint8_t i = 0;
    bool ret = false;
    int32_t ret_32 = 0;
    uint8_t value = 0;
    FILE *fd_w = NULL;
    uint16_t differences = 0;

    init_register(registers_storage);
    init_register(registers_input);

    for ( i = 0; i < sizeof(r_registers) / sizeof(read_registers_t); i++ )
    {
        ret = ir3806x_read_byte(r_registers[i].reg_addr, &value);
        if ( ret == true )
        {
            // Store the value
            registers_storage[r_registers[i].reg_addr].reg_addr = r_registers[i].reg_addr;
            registers_storage[r_registers[i].reg_addr].value = value;
            registers_storage[r_registers[i].reg_addr].mask = r_registers[i].mask;
        }
        else
        {

            fprintf(stderr, "failed reading value at addr 0x%02X\n", r_registers[i].reg_addr);

            // if reading failed, the array index will not match the reg_addr
        }
    }

    // if no export and input options given, print to console
    if ( output_filename == NULL && input_filename == NULL )
    {
        for ( i = 0; i < NUMBER_OF_REGISTERS; i++ )
        {
            if ( registers_storage[i].reg_addr == i )
            {
                fprintf(stdout, "%02X %02X %02X\n", registers_storage[i].reg_addr, registers_storage[i].value, registers_storage[i].mask);
            }
        }
    }

    if ( output_filename != NULL )
    {
        fd_w = fopen(output_filename, "w+");
        CHECK_RETURN(fd_w != NULL, false, "Error opening/creating output file (%s): %s\n", output_filename, strerror(errno));

        for ( i = 0; i < NUMBER_OF_REGISTERS; i++ )
        {
            if ( registers_storage[i].reg_addr == i )
            {
                fprintf(fd_w, "%02X %02X %02X\n", registers_storage[i].reg_addr, registers_storage[i].value, registers_storage[i].mask);
            }
        }
        ret_32 = fclose(fd_w);
        CHECK_RETURN(ret_32 == 0, false, "Error closing output file (%s): %s\n", output_filename, strerror(errno));
    }

    if ( input_filename != NULL )
    {
        ret = parse_file(input_filename, registers_input);
        CHECK_RETURN(ret == true, false, "Error parsing file (%s)\n", input_filename);

        differences = count_diff(registers_storage, registers_input);
        if ( differences != UINT16_MAX )
        {
            printf("Number of differences between current registers and input file: %d\n", differences);
        }
    }

    // Check CRC flags
    ret = check_crc();
    CHECK_RETURN(ret == true, false, "Error checking CRC flags\n");

    printf("No CRC errors detected\n");

    return true;
}

bool ir3806x_program ( bool dry_run, const char *input_filename )
{
    bool ret = false;
    uint8_t user_slot = 0;
    uint8_t value = 0;
    uint16_t programming_word = 0;
    uint8_t max_waiting_ite = 5;

    ret = check_mtp_user_left(&user_slot);
    CHECK_RETURN(ret == true, false, "Error: failed to check the number of USER programming left\n");
    CHECK_RETURN(user_slot <= USER_PROGRAMMING_MAX_INDEX, false, "Error: no USER MTP left\n");

    printf("Next USER slot to use %u ( %u remaining slots )\n", user_slot, USER_PROGRAMMING_MAX_INDEX + 1 - user_slot);

    if ( input_filename == NULL )
    {
        printf("No input file, nothing else to do\n");
        return true;
    }

    // Step 11: Write all USER section
    ret = write_all_registers(input_filename);
    CHECK_RETURN(ret == true, false, "Error: failed to write all registers from input file (%s)\n", input_filename);

    if ( !dry_run )
    {
        printf("\n");
        CONFIRM(false, "program IR3806x with data from '%s' on USER slot %u.\nThis operation cannot be undone!\n",
                input_filename, user_slot)

        // Step 12: Enable the MTP programming clock by setting register 80h to 01h.
        ret = ir3806x_write_byte(MTP_PROGRAMMING_CLK_ADDR, 0x01);
        CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %02X\n", MTP_PROGRAMMING_CLK_ADDR);

        // Step 13: Write a user section programming command based on Table 5 i.e set register B0h = (40h + new_section_ptr)
        programming_word = (uint8_t)(user_slot + 0x40);
        ret = ir3806x_write_byte(MTP_COMMAND_ADDR, programming_word);
        CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %02X\n", MTP_COMMAND_ADDR);

        do
        {
            // Step 9: sleep 50ms
            usleep(50000);

            ret = ir3806x_read_byte(MTP_COMMAND_ADDR, &value);
            CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %02X\n", MTP_COMMAND_ADDR);

            if ( ((value >> 5) ) == 0 )
            {
                break;
            }

            printf("programming not finished yet\n");

            max_waiting_ite--;

        } while ( max_waiting_ite > 0 );

        CHECK_RETURN(max_waiting_ite != 0, false, "Error: Programming timed out\n");

        CHECK_RETURN(value == 0x08 , false, "Error: Programming FAILED\n");

        printf("Programming SUCCEEDED\n");
        printf("You can now reboot the board, and verify the programming with the read-registers cmd\n");
    }

    return true;
}

/* ***************************************************************************/
/* Private functions *********************************************************/
/* ***************************************************************************/

// Initialize an array, so that the reg_addr never matches the index
static void init_register ( registers_t *array )
{
    ASSERT_EMERG(array != NULL, "%s: NULL parameter\n", __FUNCTION__);

    uint8_t i;

    for ( i = 0; i < NUMBER_OF_REGISTERS; i++ )
    {
        array[i].reg_addr = i + 1;
    }
}

static bool parse_file (const char *filename, registers_t *array )
{
    ASSERT_EMERG(filename != NULL, "%s: NULL parameter\n", __FUNCTION__);
    ASSERT_EMERG(array != NULL, "%s: NULL parameter\n", __FUNCTION__);

    FILE *fd = NULL;
    char line[256] = { 0 };

    uint8_t reg_addr = 0;
    uint8_t value = 0;
    uint8_t mask = 0;

    int32_t ret_32 = 0;
    int32_t count = 0;

    fd = fopen(filename, "r");
    CHECK_RETURN(fd != NULL, false, "Error opening file (%s): %s\n", filename, strerror(errno));

    while ( fgets(line, 255, fd) != NULL )
    {
        count = sscanf(line, "%02hhX %02hhX %02hhX\n", &reg_addr, &value, &mask);
        CHECK_RETURN(count == 3, false, "Error reading in file (%s): %s\n", filename, strerror(errno));


        array[reg_addr].reg_addr = reg_addr;
        array[reg_addr].value = value;
        array[reg_addr].mask = mask;
    }

    ret_32 = fclose(fd);
    CHECK_RETURN(ret_32 == 0, false, "Error closing file (%s): %s\n", filename, strerror(errno));

    return true;
}

static uint16_t count_diff ( registers_t *arrayA, registers_t *arrayB )
{
    ASSERT_EMERG(arrayA != NULL, "%s: NULL parameter\n", __FUNCTION__);
    ASSERT_EMERG(arrayB != NULL, "%s: NULL parameter\n", __FUNCTION__);

    uint16_t i = 0;
    uint16_t reg_addr_i = 0;
    uint16_t count = 0;

    for ( i = 0; i < sizeof(w_registers_ranges) / sizeof(write_registers_ranges_t); i++ )
    {
        CHECK_RETURN(w_registers_ranges[i].first <= w_registers_ranges[i].last, UINT16_MAX, "Error counting differences\n");

        for ( reg_addr_i = w_registers_ranges[i].first; reg_addr_i <= w_registers_ranges[i].last; reg_addr_i++ )
        {
            CHECK_RETURN(reg_addr_i < NUMBER_OF_REGISTERS, UINT16_MAX, "Error counting differences\n");

            if ( arrayA[reg_addr_i].reg_addr == reg_addr_i )
            {
                // reg_addr Differences
                if ( arrayA[reg_addr_i].reg_addr != arrayB[reg_addr_i].reg_addr )
                {
                    count++;
                    continue;
                }

                // mask Differences
                if ( arrayA[reg_addr_i].mask != arrayB[reg_addr_i].mask )
                {
                    count++;
                    continue;
                }

                // value & mask Differences
                if ( (arrayA[reg_addr_i].value & arrayA[reg_addr_i].mask) != (arrayB[reg_addr_i].value & arrayB[reg_addr_i].mask) )
                {
                    count++;
                    continue;
                }
            }
            else
            {
                // no value in arrayA, but value in arrayB
                if ( arrayB[reg_addr_i].reg_addr == reg_addr_i )
                {
                    count++;
                    continue;
                }
            }
        }
    }

    return count;
}

// Check that CRC error flag 0x97[2:0] == 000
static bool check_crc ( void )
{
    bool ret = false;
    uint8_t value = 0;

    ret = ir3806x_read_byte(CRC_CHECK_ADDR, &value);
    CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %02X\n", CRC_CHECK_ADDR);

    CHECK_RETURN((value & 0x04) == 0x00, false, "Error: Trim CRC Flag set (value at 0x%02X : %02X)\n", CRC_CHECK_ADDR, value);
    CHECK_RETURN((value & 0x02) == 0x00, false, "Error: User CRC Flag set (value at 0x%02X : %02X)\n", CRC_CHECK_ADDR, value);
    CHECK_RETURN((value & 0x01) == 0x00, false, "Error: MFR CRC Flag set (value at 0x%02X : %02X)\n", CRC_CHECK_ADDR, value);

    return true;
}

// next_image_index means the image index to use for the next USER programming
static bool check_mtp_user_left ( uint8_t *next_image_index )
{
    ASSERT_EMERG(next_image_index != NULL, "%s: NULL parameter\n", __FUNCTION__);

    bool ret = false;
    uint8_t i = 0;

    uint8_t value = 0;
    uint8_t section_ptr = 0;

    ret = ir3806x_read_byte(NVM_PTR_USER, &value);
    CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %02X\n", NVM_PTR_USER);

    section_ptr = value & 0x0F;

    if ( section_ptr == 15 )
    {
        *next_image_index = 0;
    }
    else if ( section_ptr >= USER_PROGRAMMING_MAX_INDEX)
    {
        // no more USER MTP left
        *next_image_index = USER_PROGRAMMING_MAX_INDEX + 1;
    }
    else
    {
        *next_image_index = section_ptr + 1;
    }

    return true;
}

static bool write_all_registers ( const char *input_filename )
{
    ASSERT_EMERG(input_filename != NULL, "%s: NULL parameter\n", __FUNCTION__);

    bool ret = false;
    registers_t registers_input[NUMBER_OF_REGISTERS] = { 0 };

    uint8_t i = 0;
    uint8_t reg_addr_i = 0;

    init_register(registers_input);

    ret = parse_file(input_filename, registers_input);
    CHECK_RETURN(ret == true, false, "Error parsing file (%s)\n", input_filename);

    for ( i = 0; i < sizeof(w_registers_ranges) / sizeof(write_registers_ranges_t); i++ )
    {
        CHECK_RETURN(w_registers_ranges[i].first <= w_registers_ranges[i].last, false, "Error parsing file (%s)\n", input_filename);

        for ( reg_addr_i = w_registers_ranges[i].first; reg_addr_i <= w_registers_ranges[i].last; reg_addr_i++ )
        {
            CHECK_RETURN(registers_input[reg_addr_i].reg_addr == reg_addr_i, false, "Error: unknown value for reg addr %02X\n", reg_addr_i);

            ret = ir3806x_write_byte(reg_addr_i, registers_input[reg_addr_i].value);
            CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %02X\n", reg_addr_i);
        }
    }

    return true;
}
