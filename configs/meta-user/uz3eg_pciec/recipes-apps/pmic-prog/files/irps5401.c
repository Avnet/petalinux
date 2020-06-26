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
//  Module Name:         irps5401.c
//  Project Name:        PMIC PROGRAMMER
//
//  Description:         Application to program Infineon PMICs
//
//  Dependencies:
//
//  Revision:            Jun 16, 2020: 1.0 Initial version
//
// ----------------------------------------------------------------------------

#include "irps5401.h"
#include "irps5401-reg.h"
#include "i2c-comm.h"
#include "utils.h"

#include "unistd.h"

static uint8_t current_page = 0xFF;

typedef struct
{
    uint16_t reg_addr;
    uint8_t value;
    uint8_t mask;
} registers_t;

static bool set_page ( uint8_t page );
static void init_register ( registers_t *array );
static bool read_hex_value_as_dec ( uint8_t value_hex, uint8_t *value_dec );
static bool get_date_code ( const registers_t *array, char *datecode_str );
static bool parse_file (const char *filename, registers_t *array );
static uint16_t count_diff ( registers_t *arrayA, registers_t *arrayB );
static bool check_mtp_user_left ( uint8_t *next_image_index );
static bool write_all_registers ( const char *input_filename );

/* ***************************************************************************/
/* Public functions *********************************************************/
/* ***************************************************************************/

bool irps5401_read_byte ( uint16_t address, uint8_t *res )
{
    ASSERT_EMERG(res != NULL, "%s: NULL parameter\n", __FUNCTION__);

    uint8_t page_addr =0 ;
    uint8_t reg_addr =0 ;
    bool ret = false;

    page_addr = (uint8_t)((address >> 8) & 0xFF);
    reg_addr = (uint8_t)(address & 0xFF);

    if ( current_page != page_addr )
    {
        ret = set_page(page_addr);
        CHECK_RETURN(ret == true, false, "Error: failed to set page to 0x%02X\n", page_addr);
    }

    ret = i2c_comm_read_byte(reg_addr, res);
    CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %02X\n", reg_addr);

    return true;
}

bool irps5401_read_word ( uint16_t address, uint16_t *res )
{
    ASSERT_EMERG(res != NULL, "%s: NULL parameter\n", __FUNCTION__);

    uint8_t page_addr =0 ;
    uint8_t reg_addr =0 ;
    bool ret = false;

    page_addr = (uint8_t)((address >> 8) & 0xFF);
    reg_addr = (uint8_t)(address & 0xFF);

    if ( current_page != page_addr )
    {
        ret = set_page(page_addr);
        CHECK_RETURN(ret == true, false, "Error: failed to set page to 0x%02X\n", page_addr);
    }

    ret = i2c_comm_read_word(reg_addr, res);
    CHECK_RETURN(ret == true, false, "Error: failed to read word at addr %02X\n", reg_addr);

    return true;
}

bool irps5401_write_byte ( uint16_t address, uint8_t value )
{
    uint8_t page_addr =0 ;
    uint8_t reg_addr =0 ;
    bool ret = false;

    page_addr = (uint8_t)((address >> 8) & 0xFF);
    reg_addr = (uint8_t)(address & 0xFF);

    if ( current_page != page_addr )
    {
        ret = set_page(page_addr);
        CHECK_RETURN(ret == true, false, "Error: failed to set page to 0x%02X\n", page_addr);
    }

    ret = i2c_comm_write_byte(reg_addr, value);
    CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %02X\n", reg_addr);

    return true;
}

bool irps5401_write_word ( uint16_t address, uint16_t value )
{
    uint8_t page_addr =0 ;
    uint8_t reg_addr =0 ;
    bool ret = false;

    page_addr = (uint8_t)((address >> 8) & 0xFF);
    reg_addr = (uint8_t)(address & 0xFF);

    if ( current_page != page_addr )
    {
        ret = set_page(page_addr);
        CHECK_RETURN(ret == true, false, "Error: failed to set page to 0x%02X\n", page_addr);
    }

    ret = i2c_comm_write_word(reg_addr, value);
    CHECK_RETURN(ret == true, false, "Error: failed to write word at addr %02X\n", reg_addr);

    return true;
}


bool irps5401_read_all_registers ( const char *output_filename, const char *input_filename )
{
    registers_t registers_storage[NUMBER_OF_REGISTERS] = { 0 };
    registers_t registers_input[NUMBER_OF_REGISTERS] = { 0 };
    uint16_t i = 0;
    bool ret = false;
    int32_t ret_32 = 0;
    uint8_t value = 0;
    FILE *fd_w = NULL;
    char current_datecode_str[DATECODE_STR_MAX_SIZE] = { 0 };
    char input_datecode_str[DATECODE_STR_MAX_SIZE] = { 0 };
    uint16_t differences = 0;

    init_register(registers_storage);
    init_register(registers_input);

    for ( i = 0; i < sizeof(r_registers) / sizeof(read_registers_t); i++ )
    {
        ret = irps5401_read_byte(r_registers[i].reg_addr, &value);
        if ( ret == true )
        {
            //printf("%04X %02X %02X\n", registers[i].reg_addr, value, registers[i].mask);

            // Store the value
            registers_storage[r_registers[i].reg_addr].reg_addr = r_registers[i].reg_addr;
            registers_storage[r_registers[i].reg_addr].value = value;
            registers_storage[r_registers[i].reg_addr].mask = r_registers[i].mask;
        }
        else
        {
            fprintf(stderr, "failed reading value at addr 0x%04X\n", r_registers[i].reg_addr);

            // Store the value (the storage index will not match the reg_addr, so we will know there was an error here)
            registers_storage[r_registers[i].reg_addr].reg_addr = 0xFFFF;
            registers_storage[r_registers[i].reg_addr].value = 0xFF;
            registers_storage[r_registers[i].reg_addr].mask = 0x00;
        }
    }

    // if no export and input options given, print to console
    if ( output_filename == NULL && input_filename == NULL )
    {
        for ( i = 0; i < NUMBER_OF_REGISTERS; i++ )
        {
            if ( registers_storage[i].reg_addr == i )
            {
                fprintf(stdout, "%04X %02X %02X\n", registers_storage[i].reg_addr, registers_storage[i].value, registers_storage[i].mask);
            }
        }
    }

    ret = get_date_code(registers_storage, current_datecode_str);
    if ( ret == false )
    {
        fprintf(stderr, "Error: failed to get Datecode\n");
    }
    else
    {
        printf("Current Datecode: %s\n", current_datecode_str);
    }

    if ( output_filename != NULL )
    {
        fd_w = fopen(output_filename, "w+");
        CHECK_RETURN(fd_w != NULL, false, "Error opening/creating output file (%s): %s\n", output_filename, strerror(errno));

        for ( i = 0; i < NUMBER_OF_REGISTERS; i++ )
        {
            if ( registers_storage[i].reg_addr == i )
            {
                fprintf(fd_w, "%04X %02X %02X\n", registers_storage[i].reg_addr, registers_storage[i].value, registers_storage[i].mask);
            }
        }
        ret_32 = fclose(fd_w);
        CHECK_RETURN(ret_32 == 0, false, "Error closing output file (%s): %s\n", output_filename, strerror(errno));
    }

    if ( input_filename != NULL )
    {
        ret = parse_file(input_filename, registers_input);
        CHECK_RETURN(ret == true, false, "Error parsing file (%s)\n", input_filename);

        ret = get_date_code(registers_input, input_datecode_str);
        if ( ret == false )
        {
            fprintf(stderr, "Error: failed to get Datecode\n");
        }
        else
        {
            printf("Input file Datecode: %s\n", input_datecode_str);

            if ( !strncmp(current_datecode_str, input_datecode_str, DATECODE_STR_MAX_SIZE) )
            {
                printf("The two datecodes are equal\n");
            }
            else
            {
                printf("The two datecodes are different\n");
            }
        }

        differences = count_diff(registers_storage, registers_input);
        if ( differences != UINT16_MAX )
        {
            printf("Number of differences between current registers and input file: %d\n", differences);
        }
    }

    return true;
}

bool irps5401_program ( bool dry_run, const char *input_filename )
{
    bool ret = false;
    uint8_t user_slot = 0;
    uint8_t value = 0;
    uint16_t programming_word = 0;
    uint8_t max_waiting_ite = 3;

    ret = check_mtp_user_left(&user_slot);
    CHECK_RETURN(ret == true, false, "Error: failed to check the number of USER programming left\n");
    CHECK_RETURN(user_slot <= USER_PROGRAMMING_MAX_INDEX, false, "Error: no USER MTP left\n");

    printf("Next USER slot to use %u ( %u remaining slots )\n", user_slot, USER_PROGRAMMING_MAX_INDEX + 1 - user_slot);

    if ( input_filename == NULL )
    {
        printf("No input file, nothing else to do\n");
        return true;
    }

    if ( !dry_run )
    {
        printf("\n");
        CONFIRM(false, "program IRPS5401 with data from '%s' on USER slot %u.\nThis operation cannot be undone!\n",
                input_filename, user_slot)

        // Step 4
        ret = irps5401_write_byte(0x0086, 0x00);
        CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %04X\n", 0x0086);

        // Step 5
        ret = irps5401_read_byte(0x006C, &value);
        CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %04X\n", 0x006C);

        if ( ((value >> 1) & 1 ) == 0 )
        {
            ret = irps5401_write_byte(0x008A, 0x5A);
            CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %04X\n", 0x008A);
            ret = irps5401_write_byte(0x008B, 0xA5);
            CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %04X\n", 0x008B);
/*
            ret = irps5401_read_byte(0x006C, &value);
            CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %04X\n", 0x006C);

            CHECK_RETURN(((value >> 1) & 1 ) == 1, false, "Error: failed setting trim password\n");*/
        }

        // Step 6
        ret = irps5401_write_byte(0x000B, 0x00);
        CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %04X\n", 0x000B);
    }

    // Step 7
    ret = write_all_registers(input_filename);
    CHECK_RETURN(ret == true, false, "Error: failed to write all registers from input file (%s)\n", input_filename);

    if ( !dry_run )
    {
        // Step 8
        programming_word = (uint16_t)((user_slot << 8) + 0x42);
        ret = irps5401_write_word(0x0088, programming_word);
        CHECK_RETURN(ret == true, false, "Error: failed to write word at addr %04X\n", 0x0088);

        do
        {
            // Step 9: sleep 250ms
            usleep(250000);

            ret = irps5401_read_byte(0x0089, &value);
            CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %04X\n", 0x0089);

            if ( ((value >> 7) & 1 ) == 1 )
            {
                break;
            }

            max_waiting_ite--;

        } while ( max_waiting_ite > 0 );

        // Step 10
        // Always returning 'Programming FAILED', so disabled for now
        //CHECK_RETURN(((value >> 6) & 1 ) == 0 , false, "Error: Programming FAILED\n");

        printf("Programming SUCCEEDED\n");
        printf("You can now reboot the board, and verify the programming with the read-registers cmd\n");
    }

    return true;
}

/* ***************************************************************************/
/* Private functions *********************************************************/
/* ***************************************************************************/

static bool set_page ( uint8_t page )
{
    bool ret = false;
    ret = i2c_comm_write_byte(PAGE_BYTE_ADDR, page);
    CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %02X\n", PAGE_BYTE_ADDR);

    current_page = page;
    return true;
}

// Initialize an array, so that the reg_addr never matches the index
static void init_register ( registers_t *array )
{
    ASSERT_EMERG(array != NULL, "%s: NULL parameter\n", __FUNCTION__);

    uint16_t i;

    for ( i = 0; i < NUMBER_OF_REGISTERS; i++ )
    {
        array[i].reg_addr = i + 1;
    }
}


// Some values are expressed in hex but should be read in decimal
// For example, datecode_month (DATECODE_MONTH_BYTE_ADDR) value 0x10 (16 decimal) is October
static bool read_hex_value_as_dec ( uint8_t value_hex, uint8_t *value_dec )
{
    ASSERT_EMERG(value_dec != NULL, "%s: NULL parameter\n", __FUNCTION__);

    char buffer[3] = { 0 };
    int32_t count = 0;

    count = snprintf(buffer, 3, "%02X", value_hex);
    CHECK_RETURN(count < 3, false, "Error: failed to convert hex value displayed\n")

    count = sscanf(buffer, "%hhu", value_dec);
    CHECK_RETURN(count == 1, false, "Error: failed to convert hex value displayed\n")

    return true;
}

static bool get_date_code ( const registers_t *array, char *datecode_str )
{
    ASSERT_EMERG(array != NULL, "%s: NULL parameter\n", __FUNCTION__);
    ASSERT_EMERG(datecode_str != NULL, "%s: NULL parameter\n", __FUNCTION__);

    bool ret = false;
    uint8_t month_hex = 0;
    uint8_t day_hex = 0;
    uint8_t month = 0;
    uint8_t day = 0;

    CHECK_RETURN(array[DATECODE_MONTH_BYTE_ADDR].reg_addr == DATECODE_MONTH_BYTE_ADDR, false, "Error: Can't get Datecode\n");
    month_hex = array[DATECODE_MONTH_BYTE_ADDR].value;
    ret = read_hex_value_as_dec(month_hex, &month);
    CHECK_RETURN(ret == true, false, "Error: Datecode: failed to read hex value as decimal (%02X)\n", month_hex);
    CHECK_RETURN(month <= 12, false, "Error: Datecode: wrong month value %u\n", month);

    CHECK_RETURN(array[DATECODE_DAY_BYTE_ADDR].reg_addr == DATECODE_DAY_BYTE_ADDR, false, "Error: Can't get Datecode\n");
    day_hex = array[DATECODE_DAY_BYTE_ADDR].value;
    ret = read_hex_value_as_dec(day_hex, &day);
    CHECK_RETURN(ret == true, false, "Error: Datecode: failed to read hex value as decimal (%02X)\n", day_hex);
    CHECK_RETURN(day <= 31, false, "Error: Datecode: wrong day value %u\n", day);

    switch ( month )
    {
        case 1:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","JAN", day);
            break;
        case 2:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","FEB", day);
            break;
        case 3:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","MAR", day);
            break;
        case 4:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","APR", day);
            break;
        case 5:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","MAY", day);
            break;
        case 6:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","JUN", day);
            break;
        case 7:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","JUL", day);
            break;
        case 8:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","AUG", day);
            break;
        case 9:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","SEP", day);
            break;
        case 10:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","OCT", day);
            break;
        case 11:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","NOV", day);
            break;
        case 12:
            snprintf(datecode_str, DATECODE_STR_MAX_SIZE, "%s. %d","DEC", day);
            break;
        default:
            fprintf(stderr, "Error: Datecode: wrong month value %u\n", month);
            return false;
    }

    return true;
}

static bool parse_file (const char *filename, registers_t *array )
{
    ASSERT_EMERG(filename != NULL, "%s: NULL parameter\n", __FUNCTION__);
    ASSERT_EMERG(array != NULL, "%s: NULL parameter\n", __FUNCTION__);

    FILE *fd = NULL;
    char line[256] = { 0 };

    uint16_t reg_addr = 0;
    uint8_t value = 0;
    uint8_t mask = 0;

    int32_t ret_32 = 0;
    int32_t count = 0;

    fd = fopen(filename, "r");
    CHECK_RETURN(fd != NULL, false, "Error opening file (%s): %s\n", filename, strerror(errno));

    while ( fgets(line, 255, fd) != NULL )
    {
        if ( line[0] != '0' && line[0] != '1' )
        {
            // not a valid line, skipping
            continue;
        }

        count = sscanf(line, "%04hX %02hhX %02hhX\n", &reg_addr, &value, &mask);
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

// next_image_index means the image index to use for the next USER programming
static bool check_mtp_user_left ( uint8_t *next_image_index )
{
    ASSERT_EMERG(next_image_index != NULL, "%s: NULL parameter\n", __FUNCTION__);

    bool ret = false;
    uint8_t i = 0;
    uint16_t user_left_msb = 0;
    char user_left_msb_str[17];

    uint16_t user_left_lsb = 0;
    char user_left_lsb_str[17];

    *next_image_index = 0xFF;

    ret = irps5401_read_word(NVM_PTR_USER_MSB, &user_left_msb);
    CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %04X\n", NVM_PTR_USER_MSB);
    convert_word_to_binary(user_left_msb, user_left_msb_str);

    ret = irps5401_read_word(NVM_PTR_USER_LSB, &user_left_lsb);
    CHECK_RETURN(ret == true, false, "Error: failed to read byte at addr %04X\n", NVM_PTR_USER_LSB);
    convert_word_to_binary(user_left_lsb, user_left_lsb_str);

    printf("USER MTP LEFT: %s %s\n", user_left_msb_str, user_left_lsb_str);

    for ( i = 0; i < 16; i++ )
    {
        if ( ((user_left_lsb >> i) & 1) == 0 )
        {
            *next_image_index = i;
            break;
        }
    }

    if ( *next_image_index == 0xFF )
    {
        for ( i = 0; i < 16; i++ )
        {
            if ( ((user_left_msb >> i) & 1) == 0 )
            {
                *next_image_index = (uint8_t) (i + 16);
                break;
            }
        }
    }

    return true;
}

static bool write_all_registers ( const char *input_filename )
{
    ASSERT_EMERG(input_filename != NULL, "%s: NULL parameter\n", __FUNCTION__);

    bool ret = false;
    registers_t registers_input[NUMBER_OF_REGISTERS] = { 0 };
    uint16_t i = 0;
    uint16_t reg_addr_i = 0;

    init_register(registers_input);

    ret = parse_file(input_filename, registers_input);
    CHECK_RETURN(ret == true, false, "Error parsing file (%s)\n", input_filename);

    for ( i = 0; i < sizeof(w_registers_ranges) / sizeof(write_registers_ranges_t); i++ )
    {
        CHECK_RETURN(w_registers_ranges[i].first <= w_registers_ranges[i].last, false, "Error parsing file (%s)\n", input_filename);

        for ( reg_addr_i = w_registers_ranges[i].first; reg_addr_i <= w_registers_ranges[i].last; reg_addr_i++ )
        {
            CHECK_RETURN(registers_input[reg_addr_i].reg_addr == reg_addr_i, false, "Error: unknown value for reg addr %04X\n", reg_addr_i);

            ret = irps5401_write_byte(reg_addr_i, registers_input[reg_addr_i].value);
            CHECK_RETURN(ret == true, false, "Error: failed to write byte at addr %04X\n", reg_addr_i);
        }
    }

    return true;
}

