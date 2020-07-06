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
//  Module Name:         main.c
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
#include "ir3806x.h"
#include "i2c-comm.h"
#include "utils.h"

#define VERSION "1.0"

#define STR_CMD_READ "read"
#define STR_CMD_WRITE "write"
#define STR_CMD_DETECT "detect"
#define STR_CMD_READ_REGISTERS "read-registers"
#define STR_CMD_PROGRAM "program"

typedef enum
{
    CMD_UNKNOWN = 0,
    CMD_READ,
    CMD_WRITE,
    CMD_DETECT,
    CMD_READ_REGISTERS,
    CMD_PROGRAM,
} cmd_t;

static void print_version ( void );
static void print_usage ( void );
static bool do_read ( uint8_t i2c_bus, uint8_t i2c_chip_addr, uint16_t reg_addr );
static bool do_write ( uint8_t i2c_bus, uint8_t i2c_chip_addr, uint16_t reg_addr, uint8_t value );
static bool do_detect ( uint8_t i2c_bus );
static bool do_read_all_registers ( uint8_t i2c_bus, uint8_t i2c_chip_addr, const char *output_filename, const char *input_filename );
static bool do_program ( uint8_t i2c_bus, uint8_t i2c_chip_addr, bool dry_run, const char *input_filename );

static bool unsupported_read_byte_fct ( uint16_t address, uint8_t *res );
static bool unsupported_read_word_fct ( uint16_t address, uint16_t *res );
static bool unsupported_write_byte_fct ( uint16_t address, uint8_t value );
static bool unsupported_write_word_fct ( uint16_t address, uint16_t value );
static bool unsupported_read_all_registers_fct ( const char *output_filename, const char *compare_filename );
static bool unsupported_program_fct ( bool dry_run, const char *input_filename );

static bool identify_device ( uint8_t i2c_bus, uint8_t i2c_chip_addr );

static bool (*read_byte_fct) ( uint16_t address, uint8_t *res ) = unsupported_read_byte_fct;
static bool (*read_word_fct) ( uint16_t address, uint16_t *res ) = unsupported_read_word_fct;
static bool (*write_byte_fct) ( uint16_t address, uint8_t value ) = unsupported_write_byte_fct;
static bool (*write_word_fct) ( uint16_t address, uint16_t value ) = unsupported_write_word_fct;
static bool (*read_all_registers_fct) ( const char *output_filename, const char *compare_filename ) = unsupported_read_all_registers_fct;
static bool (*program_fct) ( bool dry_run, const char *input_filename ) = unsupported_program_fct;

/* ***************************************************************************/
/* Public functions *********************************************************/
/* ***************************************************************************/

int main(int argc, char **argv)
{
    cmd_t cmd = CMD_UNKNOWN;

    bool ret = false;

    char *ret_string = NULL;
    uint64_t parsed_i2c_bus = 0;
    uint8_t i2c_bus = 0;
    uint64_t parsed_i2c_chip_addr = 0;
    uint8_t i2c_chip_addr = 0;

    uint64_t parsed_reg_addr = 0;
    uint16_t reg_addr = 0;
    uint64_t parsed_value = 0;
    uint8_t value = 0;

    uint8_t nb_options_parsed =0;

    char *filename_output = NULL;
    char *filename_input = NULL;

    bool dry_run = false;

    print_version();

    if ( argc < 2 )
    {
        fprintf(stderr, "Error: Missing COMMAND\n");
        print_usage();
        exit(-1);
    }

    if ( !strncmp(argv[1], STR_CMD_READ, sizeof(STR_CMD_READ)) )
    {
        cmd = CMD_READ;
    }
    else if ( !strncmp(argv[1], STR_CMD_WRITE, sizeof(STR_CMD_WRITE)) )
    {
        cmd = CMD_WRITE;
    }
    else if ( !strncmp(argv[1], STR_CMD_DETECT, sizeof(STR_CMD_DETECT)) )
    {
        cmd = CMD_DETECT;
    }
    else if ( !strncmp(argv[1], STR_CMD_READ_REGISTERS, sizeof(STR_CMD_READ_REGISTERS)) )
    {
        cmd = CMD_READ_REGISTERS;
    }
    else if ( !strncmp(argv[1], STR_CMD_PROGRAM, sizeof(STR_CMD_PROGRAM)) )
    {
        cmd = CMD_PROGRAM;
    }
    else if ( !strncmp(argv[1], "-h", sizeof("-h")) || !strncmp(argv[1], "--help", sizeof("--help"))  )
    {
        print_usage();
        exit(0);
    }
    else
    {
        fprintf(stderr, "Error: Unknown COMMAND\n");
        print_usage();
        exit(-1);
    }

    // parse I2C_BUS
    if ( argc < 3 )
    {
        printf("Error: Too few or too many arguments.\n");
        print_usage();
        exit(-1);
    }

    parsed_i2c_bus = strtoul(argv[2], &ret_string, 10);
    CHECK_RETURN(errno == 0, -1, "Error: Failed to convert given value: %s\n", strerror(errno));
    CHECK_RETURN(*ret_string == '\0', -1, "Error: Failed to convert given value\n");

    CHECK_RETURN(parsed_i2c_bus <= UINT8_MAX, -1, "Error: I2C_BUS too big, %ld\n", parsed_i2c_bus);
    i2c_bus = (uint8_t) parsed_i2c_bus;


    switch ( cmd )
    {
        case CMD_READ:
            if ( argc != 5 )
            {
                fprintf(stderr, "Error: Too few or too many arguments.\n");
                print_usage();
                exit(-1);
            }

            // parse CHIP_ADDR
            parsed_i2c_chip_addr = strtoul(argv[3], &ret_string, 16);
            CHECK_RETURN(errno == 0, -1, "Error: Failed to convert given value: %s\n", strerror(errno));
            CHECK_RETURN(*ret_string == '\0', -1, "Error: Failed to convert given value\n");

            CHECK_RETURN(parsed_i2c_chip_addr <= UINT8_MAX, -1, "Error: CHIP_ADDR too big, 0x%lX\n", parsed_i2c_chip_addr);
            i2c_chip_addr = (uint8_t) parsed_i2c_chip_addr;

            // parse REG_ADDR
            parsed_reg_addr = strtoul(argv[4], &ret_string, 16);
            CHECK_RETURN(errno == 0, -1, "Error: Failed to convert given value: %s\n", strerror(errno));
            CHECK_RETURN(*ret_string == '\0', -1, "Error: Failed to convert given value\n");

            CHECK_RETURN(parsed_reg_addr <= UINT16_MAX, -1, "Error: REG_ADDR too big, 0x%lX\n", parsed_reg_addr);
            reg_addr = (uint16_t) parsed_reg_addr;

            // ask for confirmation
            CONFIRM(-1, "read byte at address 0x%04X on device 0x%02X on i2c bus %u.\n",
                    reg_addr, i2c_chip_addr, i2c_bus);

            // launch cmd
            ret = do_read(i2c_bus, i2c_chip_addr, reg_addr);
            CHECK_RETURN(ret == true, -1, "Error: failed reading (bus %u, chip 0x%02X, reg addr 0x%04X)\n",
                         i2c_bus, i2c_chip_addr, reg_addr);
            return 0;
        case CMD_WRITE:
            if ( argc != 6 )
            {
                printf("Error: Too few or too many arguments.\n");
                print_usage();
                exit(-1);
            }

            // parse CHIP_ADDR
            parsed_i2c_chip_addr = strtoul(argv[3], &ret_string, 16);
            CHECK_RETURN(errno == 0, -1, "Error: Failed to convert given value: %s\n", strerror(errno));
            CHECK_RETURN(*ret_string == '\0', -1, "Error: Failed to convert given value\n");

            CHECK_RETURN(parsed_i2c_chip_addr <= UINT8_MAX, -1, "Error: CHIP_ADDR too big, 0x%lX\n", parsed_i2c_chip_addr);
            i2c_chip_addr = (uint8_t) parsed_i2c_chip_addr;

            // parse REG_ADDR
            parsed_reg_addr = strtoul(argv[4], &ret_string, 16);
            CHECK_RETURN(errno == 0, -1, "Error: Failed to convert given value: %s\n", strerror(errno));
            CHECK_RETURN(*ret_string == '\0', -1, "Error: Failed to convert given value\n");

            CHECK_RETURN(parsed_reg_addr <= UINT16_MAX, -1, "Error: REG_ADDR too big, 0x%lX\n", parsed_reg_addr);
            reg_addr = (uint16_t) parsed_reg_addr;

            // parse VALUE
            parsed_value = strtoul(argv[5], &ret_string, 16);
            CHECK_RETURN(errno == 0, -1, "Error: Failed to convert given value: %s\n", strerror(errno));
            CHECK_RETURN(*ret_string == '\0', -1, "Error: Failed to convert given value\n");

            CHECK_RETURN(parsed_value <= UINT8_MAX, -1, "Error: VALUE too big, 0x%lX\n", parsed_value);
            value = (uint8_t) parsed_value;

            // ask for confirmation
            CONFIRM(-1, "write value 0x%02X at address 0x%04X on device 0x%02X on i2c bus %u.\n",
                    value, reg_addr, i2c_chip_addr, i2c_bus);

            // launch cmd
            ret = do_write(i2c_bus, i2c_chip_addr, reg_addr, value);
            CHECK_RETURN(ret == true, -1, "Error: failed writing (bus %u, chip 0x%02X, reg addr 0x%04X, value 0x%02X)\n",
                         i2c_bus, i2c_chip_addr, reg_addr, value);
            return 0;
        case CMD_DETECT:
            if ( argc != 3 )
            {
                fprintf(stderr, "Error: Too few or too many arguments.\n");
                print_usage();
                exit(-1);
            }

            // ask for confirmation
            CONFIRM(-1, "detect available PMICs on bus %u.\n",
                    i2c_bus);

            // launch cmd
            ret = do_detect(i2c_bus);
            CHECK_RETURN(ret == true, -1, "Error: failed detection on bus %u\n", i2c_bus);
            return 0;
        case CMD_READ_REGISTERS:
            if ( argc < 4 )
            {
                fprintf(stderr, "Error: Too few arguments.\n");
                print_usage();
                exit(-1);
            }

            // parse CHIP_ADDR
            parsed_i2c_chip_addr = strtoul(argv[3], &ret_string, 16);
            CHECK_RETURN(errno == 0, -1, "Error: Failed to convert given value: %s\n", strerror(errno));
            CHECK_RETURN(*ret_string == '\0', -1, "Error: Failed to convert given value\n");

            CHECK_RETURN(parsed_i2c_chip_addr <= UINT8_MAX, -1, "Error: CHIP_ADDR too big, 0x%lX\n", parsed_i2c_chip_addr);
            i2c_chip_addr = (uint8_t) parsed_i2c_chip_addr;

            nb_options_parsed = 4;

            while ( argc - nb_options_parsed > 0 )
            {
                if ( !strncmp(argv[nb_options_parsed], "-o", sizeof("-o")) )
                {
                    CHECK_RETURN(filename_output == NULL, -1, "'-o' option already parsed\n");

                    nb_options_parsed++;

                    //parse filename
                    CHECK_RETURN(argc > nb_options_parsed, -1, "No filename given with '-o' option\n");

                    filename_output = argv[nb_options_parsed];
                    nb_options_parsed++;
                }
                else if ( !strncmp(argv[nb_options_parsed], "-i", sizeof("-i")) )
                {
                    CHECK_RETURN(filename_input == NULL, -1, "'-i' option already parsed\n");
                    nb_options_parsed++;

                    //parse filename
                    CHECK_RETURN(argc > nb_options_parsed, -1, "No filename given with '-i' option\n");

                    filename_input = argv[nb_options_parsed];
                    nb_options_parsed++;
                }
                else
                {
                    fprintf(stderr, "Error: Unknown option\n");
                    print_usage();
                    exit(-1);
                }
            }

            // ask for confirmation
            CONFIRM(-1, "read all registers on device 0x%02X on i2c bus %u.\n",
                    i2c_chip_addr, i2c_bus);

            // launch cmd
            ret = do_read_all_registers(i2c_bus, i2c_chip_addr, filename_output, filename_input);
            CHECK_RETURN(ret == true, -1, "Error: failed reading all registers(bus %u, chip 0x%02X)\n",
                         i2c_bus, i2c_chip_addr);
            return 0;
        case CMD_PROGRAM:
            if ( argc < 4 )
            {
                fprintf(stderr, "Error: Too few arguments.\n");
                print_usage();
                exit(-1);
            }

            // parse CHIP_ADDR
            parsed_i2c_chip_addr = strtoul(argv[3], &ret_string, 16);
            CHECK_RETURN(errno == 0, -1, "Error: Failed to convert given value: %s\n", strerror(errno));
            CHECK_RETURN(*ret_string == '\0', -1, "Error: Failed to convert given value\n");

            CHECK_RETURN(parsed_i2c_chip_addr <= UINT8_MAX, -1, "Error: CHIP_ADDR too big, 0x%lX\n", parsed_i2c_chip_addr);
            i2c_chip_addr = (uint8_t) parsed_i2c_chip_addr;

            nb_options_parsed = 4;

            while ( argc - nb_options_parsed > 0 )
            {
                if ( !strncmp(argv[nb_options_parsed], "-d", sizeof("-d")) )
                {
                    CHECK_RETURN(dry_run == false, -1, "'-d' option already parsed\n");

                    nb_options_parsed++;

                    dry_run = true;
                }
                else if ( !strncmp(argv[nb_options_parsed], "-i", sizeof("-i")) )
                {
                    CHECK_RETURN(filename_input == NULL, -1, "'-i' option already parsed\n");
                    nb_options_parsed++;

                    //parse filename
                    CHECK_RETURN(argc > nb_options_parsed, -1, "No filename given with '-i' option\n");

                    filename_input = argv[nb_options_parsed];
                    nb_options_parsed++;
                }
                else
                {
                    fprintf(stderr, "Error: Unknown option\n");
                    print_usage();
                    exit(-1);
                }
            }

            // ask for confirmation
            CONFIRM(-1, "program %s device 0x%02X on i2c bus %u.\n",
                    dry_run ? "(dry-run)" : "", i2c_chip_addr, i2c_bus);

            // launch cmd
            ret = do_program(i2c_bus, i2c_chip_addr, dry_run, filename_input);
            CHECK_RETURN(ret == true, -1, "Error: failed programming (bus %u, chip 0x%02X)\n",
                         i2c_bus, i2c_chip_addr);
            return 0;
        default:
            fprintf(stderr, "Error: Unknown COMMAND\n");
            print_usage();
            exit(-1);
    }

    return 0;
}

/* ***************************************************************************/
/* Private functions *********************************************************/
/* ***************************************************************************/

static void print_version ( void )
{
    printf("Version: %s\n\n", VERSION);
}

static void print_usage ( void )
{
    printf("Usage: pmic_prog [-h] COMMANDS\n"
            "  -h = display this help and exit\n\n"
            "COMMANDS:\n"
            "\ndetect I2C_BUS to detect all the PMICs on an i2c bus\n"
            "    with:\n"
            "      I2C_BUS the i2c bus number (decimal: 0-255)\n"
            "    Example:\n"
            "    \" pmic_prog detect 6 \" to detect all the PMICs on an i2c bus 6\n"
            "\nread I2C_BUS CHIP_ADDR REG_ADDR to read a byte at a specific address\n"
            "    with:\n"
            "      I2C_BUS the i2c bus number (decimal: 0-255)\n"
            "      CHIP_ADDR the chip address (hex: 0x00 - 0xFF)\n"
            "      REG_ADDR the address of the register to read  (hex: 0x0000 - 0xFFFF)\n\n"
            "    Example:\n"
            "    \" pmic_prog read 6 0x13 0x069e \" to read the byte at address 0x069e (loop A ot_fault_limit for IRPS5401) of chip 0x13 on i2c bus 6\n"
            "\nwrite I2C_BUS CHIP_ADDR REG_ADDR VALUE to write a byte at a specific address\n"
            "    with:\n"
            "      I2C_BUS the i2c bus number (decimal: 0-255)\n"
            "      CHIP_ADDR the chip address (hex: 0x00 - 0xFF)\n"
            "      REG_ADDR the address of the register to write  (hex: 0x0000 - 0xFFFF)\n"
            "      VALUE the byte to write (hex: 0x00 - 0xFF)\n\n"
            "    Example:\n"
            "    \" pmic_prog write 6 0x13 0x069e 0x7F \" to write 0x7F at address 0x069e (loop A ot_fault_limit for IRPS5401) of chip 0x13 on i2c bus 6\n"
            "\nread-registers I2C_BUS CHIP_ADDR [-o OUTPUT_FILE] [-i INPUT_FILE] to read all the registers\n"
            "    with:\n"
            "      I2C_BUS the i2c bus number (decimal: 0-255)\n"
            "      CHIP_ADDR the chip address (hex: 0x00 - 0xFF)\n"
            "      -o OUTPUT_FILE: Save the register values in the 'OUTPUT_FILE' file (optional)\n"
            "      -i INPUT_FILE: Check the register values from the given 'INPUT_FILE' file, and check the differences (optional)\n\n"
            "    Example:\n"
            "    \" pmic_prog read-registers 6 0x13 -o new_file.txt -i 0x13_Oct_01.txt \" to save the register\n"
            "            values in the file 'new_file.txt' and check the differences with those in '0x13_Oct_01.txt'.\n"
            "\nprogram I2C_BUS CHIP_ADDR [-d] [-i INPUT_FILE] to write the registers and program the PMIC on the first available USER slot\n"
            "    with:\n"
            "      I2C_BUS the i2c bus number (decimal: 0-255)\n"
            "      CHIP_ADDR the chip address (hex: 0x00 - 0xFF)\n"
            "      -d: dry-run The command will only check the number of USER slots available and write registers if input file given (optional)\n"
            "      -i INPUT_FILE: write the registers from this config file (optional)\n\n"
            "    Example:\n"
            "    \" pmic_prog program 6 0x13 -i 0x13_Oct_01.txt \" to program the PMIC with register values from '0x13_Oct_01.txt'.\n"
    );
}

static bool do_read ( uint8_t i2c_bus, uint8_t i2c_chip_addr, uint16_t reg_addr )
{
    bool ret = false;
    uint8_t value = 0;

    ret = identify_device(i2c_bus, i2c_chip_addr);
    CHECK_RETURN(ret == true, false, "Error: failed to identify the i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    ret = i2c_comm_open_device(i2c_bus, i2c_chip_addr);
    CHECK_RETURN(ret == true, false, "Error: failed to open i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    ret = read_byte_fct(reg_addr, &value);
    if ( ret == false )
    {
        fprintf(stderr, "Error: failed to read byte at addr 0x%04X\n", reg_addr);
        ret = i2c_comm_close_device();
        return false;
    }

    printf("0x%04X : 0x%02X\n",reg_addr, value);

    ret = i2c_comm_close_device();
    CHECK_RETURN(ret == true, false, "Error: failed to close i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    return true;
}

static bool do_write ( uint8_t i2c_bus, uint8_t i2c_chip_addr, uint16_t reg_addr, uint8_t value )
{
    bool ret = false;

    ret = identify_device(i2c_bus, i2c_chip_addr);
    CHECK_RETURN(ret == true, false, "Error: failed to identify the i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    ret = i2c_comm_open_device(i2c_bus, i2c_chip_addr);
    CHECK_RETURN(ret == true, false, "Error: failed to open i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    ret = write_byte_fct(reg_addr, value);
    if ( ret == false )
    {
        fprintf(stderr, "Error: failed to write byte at addr 0x%04X\n", reg_addr);
        ret = i2c_comm_close_device();
        return false;
    }

    ret = i2c_comm_close_device();
    CHECK_RETURN(ret == true, false, "Error: failed to close i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    return true;
}

static bool do_detect ( uint8_t i2c_bus )
{
    bool ret = false;
    uint8_t dev_addr[256] = { 0 };
    bool pmic_found = false;
    uint16_t i = 0;
    uint16_t pmbus_addr = 0;

    ret = i2c_comm_detect(i2c_bus, dev_addr);
    CHECK_RETURN(ret == true, false, "Error: i2c_comm_detect failed");

    for ( i = 0; i <= 255; i++ )
    {
        if ( dev_addr[i] == 1 )
        {
            pmbus_addr = (uint16_t) (i + INFINEON_PMBUS_ADDR_OFFSET);
            if ( pmbus_addr  > UINT8_MAX || dev_addr[pmbus_addr] != 1 )
            {
                // no valid pmbus address
                continue;
            }

            dev_id_t dev_id = identify_device_from_pmbus_interface(i2c_bus, (uint8_t) i);

            if ( dev_id == DEV_ID_UNKNOWN )
            {
                continue;
            }

            pmic_found = true;
            printf("Found %s at address 0x%02X (PMBus address = 0x%02X)\n", get_dev_id_string(dev_id), i, pmbus_addr);
        }
    }

    if ( !pmic_found )
    {
        printf("No known PMIC found\n");
    }

    return true;
}

static bool do_read_all_registers ( uint8_t i2c_bus, uint8_t i2c_chip_addr, const char *output_filename, const char *input_filename )
{
    bool ret = false;

    ret = identify_device(i2c_bus, i2c_chip_addr);
    CHECK_RETURN(ret == true, false, "Error: failed to identify the i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    ret = i2c_comm_open_device(i2c_bus, i2c_chip_addr);
    CHECK_RETURN(ret == true, false, "Error: failed to open i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    ret = read_all_registers_fct(output_filename, input_filename);
    if ( ret == false )
    {
        fprintf(stderr, "Error: failed to read all registers\n");
        ret = i2c_comm_close_device();
        return false;
    }

    ret = i2c_comm_close_device();
    CHECK_RETURN(ret == true, false, "Error: failed to close i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    return true;
}

static bool do_program ( uint8_t i2c_bus, uint8_t i2c_chip_addr, bool dry_run, const char *input_filename )
{
    bool ret = false;

    ret = identify_device(i2c_bus, i2c_chip_addr);
    CHECK_RETURN(ret == true, false, "Error: failed to identify the i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    ret = i2c_comm_open_device(i2c_bus, i2c_chip_addr);
    CHECK_RETURN(ret == true, false, "Error: failed to open i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    ret = program_fct(dry_run, input_filename);
    if ( ret == false )
    {
        fprintf(stderr, "Error: failed to program PMIC (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);
        ret = i2c_comm_close_device();
        return false;
    }

    ret = i2c_comm_close_device();
    CHECK_RETURN(ret == true, false, "Error: failed to close i2c device (bus %u, addr 0x%02X)\n", i2c_bus, i2c_chip_addr);

    return true;
}

static bool unsupported_read_byte_fct ( uint16_t address, uint8_t *res )
{
    fprintf(stderr, "Error: Unsupported function for this device (read_byte)\n");
    return false;
}

static bool unsupported_read_word_fct ( uint16_t address, uint16_t *res )
{
    fprintf(stderr, "Error: Unsupported function for this device (read_word)\n");
    return false;
}


static bool unsupported_write_byte_fct ( uint16_t address, uint8_t value )
{
    fprintf(stderr, "Error: Unsupported function for this device (write_byte)\n");
    return false;
}


static bool unsupported_write_word_fct ( uint16_t address, uint16_t value )
{
    fprintf(stderr, "Error: Unsupported function for this device (write_word)\n");
    return false;
}


static bool unsupported_read_all_registers_fct ( const char *output_filename, const char *compare_filename )
{
    fprintf(stderr, "Error: Unsupported function for this device (read_all_registers)\n");
    return false;
}


static bool unsupported_program_fct ( bool dry_run, const char *input_filename )
{
    fprintf(stderr, "Error: Unsupported function for this device (program)\n");
    return false;
}

static bool identify_device ( uint8_t i2c_bus, uint8_t i2c_chip_addr )
{
    dev_id_t dev_id = identify_device_from_pmbus_interface(i2c_bus, i2c_chip_addr);

    switch ( dev_id )
    {
        case DEV_ID_IRPS5401:
            printf("%s successfully detected\n", get_dev_id_string(dev_id));
            // affect functions
            read_byte_fct = irps5401_read_byte;
            read_word_fct = irps5401_read_word;
            write_byte_fct = irps5401_write_byte;
            write_word_fct = irps5401_write_word;
            read_all_registers_fct = irps5401_read_all_registers;
            program_fct = irps5401_program;
            return true;

        case DEV_ID_IR38060:
        case DEV_ID_IR38062:
        case DEV_ID_IR38063:
        case DEV_ID_IR38064:
            printf("%s successfully detected\n", get_dev_id_string(dev_id));
            // affect functions
            read_byte_fct = ir3806x_read_byte;
            write_byte_fct = ir3806x_write_byte;
            read_all_registers_fct = ir3806x_read_all_registers;
            program_fct = ir3806x_program;
            return true;

        case DEV_ID_UNKNOWN:
        default:
            return false;
    }
}

