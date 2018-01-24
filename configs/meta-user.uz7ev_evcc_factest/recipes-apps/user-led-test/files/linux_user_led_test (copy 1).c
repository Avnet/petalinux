//----------------------------------------------------------------------------
//      _____
//     *     *
//    *____   *____
//   * *===*   *==*
//  *___*===*___**  AVNET
//       *======*
//        *====*
//----------------------------------------------------------------------------
//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
//
// Please direct any questions to the PicoZed community support forum:
//    http://www.picozed.org/forum
//
// Product information is available at:
//    http://www.picozed.org/product/picozed
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
// Create Date:         Nov 16, 2016
// Design Name:         LED and PB test application
// Module Name:         linux_user_led_test.c
// Project Name:        LED and PB test application
// Target Devices:      Xilinx Zynq UltraScale+ MPSoC
// Hardware Boards:     UltraZed-EG SOM and UltraZed IO Carrier
//                      UltraZed-EV SOM and EV Carrier Card
//
// Tool versions:       Xilinx Vivado 2016.2
//						Petalinux 2016.2
//
// Description:         User LED test application for Linux.
//
// Dependencies:
//
// Revision:            Dec 04, 2013: 1.00 Initial version
//                      Apr 06, 2016: 1.01 Updated to run under 2015.2
//                                         PetaLinux tools
//						Nov 16, 2016: 1.02 Updated to UltraZed Platform
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* String formats used to build the file name path to specific GPIO
 * instances. */
#define FILE_FORMAT_GPIO_PATH          "/sys/class/gpio"
#define FILE_FORMAT_GPIO_EXPORT        "/export"
#define FILE_FORMAT_GPIO_DIRECTION     "/direction"
#define FILE_FORMAT_GPIO_VALUE         "/value"

#define GPIO_PL_PB_OFFSET				493
#define GPIO_LED_OFFSET					496

/* The LEDx_GPIO_OFFSET and PBx_GPIO_OFFSET definitions are used to indicate
 * the relative offset from the base start of the EMIO GPIO user connections.
 * In a typical reference design, the User IO will be assigned all at once
 * to an EMIO connection and connected externally to via the
 * emio_user_tri_io[] bus in the XDC constraints file and so these offsets
 * here should match the offsets of the hardware constraints as well.
 */
#define LED1_GPIO_OFFSET               ((GPIO_LED_OFFSET) + 0)
#define LED2_GPIO_OFFSET               ((GPIO_LED_OFFSET) + 1)
#define LED3_GPIO_OFFSET               ((GPIO_LED_OFFSET) + 2)
#define LED4_GPIO_OFFSET               ((GPIO_LED_OFFSET) + 3)
#define LED5_GPIO_OFFSET               ((GPIO_LED_OFFSET) + 4)
#define LED6_GPIO_OFFSET               ((GPIO_LED_OFFSET) + 5)
#define LED7_GPIO_OFFSET               ((GPIO_LED_OFFSET) + 6)
#define LED8_GPIO_OFFSET               ((GPIO_LED_OFFSET) + 7)

#define PB1_GPIO_OFFSET                ((GPIO_PL_PB_OFFSET) + 0)
#define PB2_GPIO_OFFSET                ((GPIO_PL_PB_OFFSET) + 1)
#define PB3_GPIO_OFFSET                ((GPIO_PL_PB_OFFSET) + 2)
//TC #define PB4_GPIO_OFFSET                ((GPIO_PL_PB_OFFSET) + 3)

static unsigned int direction = 1;

int set_next_count_pattern(void)
{
	const int char_buf_size = 80;
	static unsigned int count = 0;
	
	char gpio_setting[4];
	
	int test_result = 0;
	char formatted_file_name[char_buf_size];

	FILE  *fp_led1;
	FILE  *fp_led2;
	FILE  *fp_led3;
	FILE  *fp_led4;
	FILE  *fp_led5;
	FILE  *fp_led6;
	FILE  *fp_led7;
	FILE  *fp_led8;

	// Open the LED gpio value properties so that they can be read/written.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED1_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led1 = fopen(formatted_file_name, "r+");

	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED2_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led2 = fopen(formatted_file_name, "r+");

	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED3_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led3 = fopen(formatted_file_name, "r+");

	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED4_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led4 = fopen(formatted_file_name, "r+");

	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED5_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led5 = fopen(formatted_file_name, "r+");

	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED6_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led6 = fopen(formatted_file_name, "r+");

	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED7_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led7 = fopen(formatted_file_name, "r+");

	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED8_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led8 = fopen(formatted_file_name, "r+");

	// Write test pattern to LEDs.
	if ((count == 0) && ((direction == 0) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
			if (direction == 0)
		{
			// Now begin sliding 'up'.
			direction = 1;
		}
	}
	else if (((count & 0x00000001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}

	fwrite(&gpio_setting, sizeof(char), 1, fp_led1);
	fflush(fp_led1);

	if ((count == 1) && ((direction == 0) || (direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
	}
	else if ((((count >> 1) & 0x00000001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	
	fwrite(&gpio_setting, sizeof(char), 1, fp_led2);
	fflush(fp_led2);

	if ((count == 2) && ((direction == 0) || (direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
	}
	else if ((((count >> 2) & 0x00000001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	
	fwrite(&gpio_setting, sizeof(char), 1, fp_led3);
	fflush(fp_led3);

	if ((count == 3) && ((direction == 0) || (direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
	}
	else if ((((count >> 3) & 0x00000001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	
	fwrite(&gpio_setting, sizeof(char), 1, fp_led4);
	fflush(fp_led4);

	if ((count == 4) && ((direction == 0) || (direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
	}
	else if ((((count >> 4) & 0x00000001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	
	fwrite(&gpio_setting, sizeof(char), 1, fp_led5);
	fflush(fp_led5);

	if ((count == 5) && ((direction == 0) || (direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
	}
	else if ((((count >> 5) & 0x00000001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	
	fwrite(&gpio_setting, sizeof(char), 1, fp_led6);
	fflush(fp_led6);

	if ((count == 6) && ((direction == 0) || (direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
	}
	else if ((((count >> 6) & 0x00000001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	
	fwrite(&gpio_setting, sizeof(char), 1, fp_led7);
	fflush(fp_led7);

	if ((count == 7) && ((direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");

		if (direction == 1)
		{
			// Last LED in the set, begin sliding 'down'.
			direction = 0;
		}
	}
	else if ((((count >> 7) & 0x00000001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	
	fwrite(&gpio_setting, sizeof(char), 1, fp_led8);
	fflush(fp_led8);

	if (((direction == 1) & (count < 8)) ||
		(direction == 2))
	{
		// Increment count for next time around.
		count = count + 1;
	}
	else if ((direction == 0) ||
	         (direction == 3))
	{
		// Decrement count for next time around.
		count = count - 1;
	}
	else if (direction == 4)
	{
		// Increment count for next time around.
		if (count == 7)
		{
			count = 0;
		}
		else
		{
			count = count + 1;
		}
	}
	else if (direction == 5)
	{
		// Decrement count for next time around.
		if (count == 0)
		{
			count = 7;
		}
		else
		{
			count = count - 1;
		}
	}
	else
	{
		/*
		 *  Something went wrong keeping track of direction, reset the
		 *  direction to 'up'.
		 */
		direction = 1;
		count = 0;
	}

	// Close the GPIO value property files.
	fclose(fp_led1);
	fclose(fp_led2);
	fclose(fp_led3);
	fclose(fp_led4);
	fclose(fp_led5);
	fclose(fp_led6);
	fclose(fp_led7);
	fclose(fp_led8);

	return test_result;
}

int set_next_input_pattern(void)
{
	const int char_buf_size = 80;
	char gpio_setting[4];
	int test_result = 0;
	char formatted_file_name[char_buf_size];

	FILE  *fp_led1;
	FILE  *fp_led2;
	FILE  *fp_led3;
	FILE  *fp_led4;
	FILE  *fp_led5;
	FILE  *fp_led6;
	FILE  *fp_led7;
	FILE  *fp_led8;
	
	FILE  *fp_pb1;
	FILE  *fp_pb2;
	FILE  *fp_pb3;
//TC	FILE  *fp_pb4;

	// Open the gpio value properties so that they can be read/written.

	// Open the value property file for LED1.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED1_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led1 = fopen(formatted_file_name, "r+");

	// Open the value property file for LED2.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED2_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led2 = fopen(formatted_file_name, "r+");

	// Open the value property file for LED3.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED3_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led3 = fopen(formatted_file_name, "r+");

	// Open the value property file for LED4.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED4_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led4 = fopen(formatted_file_name, "r+");

	// Open the value property file for LED5.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED5_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led5 = fopen(formatted_file_name, "r+");

	// Open the value property file for LED6.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED6_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led6 = fopen(formatted_file_name, "r+");

	// Open the value property file for LED7.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED7_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led7 = fopen(formatted_file_name, "r+");

	// Open the value property file for LED8.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, LED8_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_led8 = fopen(formatted_file_name, "r+");

	// Open the value property file for PB1.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, PB1_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_pb1 = fopen(formatted_file_name, "r+");

	// Open the value property file for PB2.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, PB2_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_pb2 = fopen(formatted_file_name, "r+");

	// Open the value property file for PB3.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, PB3_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_pb3 = fopen(formatted_file_name, "r+");

	// Open the value property file for PB4.
/*TC
 * 	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_VALUE, PB4_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp_pb4 = fopen(formatted_file_name, "r+");
*/

	// Read the current value of the PB1 GPIO input.
	fscanf(fp_pb1, "%s", gpio_setting);

	// Determine whether the PS push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Count LEDs up.
		direction = 2;

		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
		fflush(fp_led4);
		fflush(fp_led5);
		fflush(fp_led6);
		fflush(fp_led7);
		fflush(fp_led8);
	}

	// Read the current value of the PB2 GPIO input.
	fscanf(fp_pb2, "%s", gpio_setting);

	// Determine whether the PL push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Count LEDs down.
		direction = 3;

		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
		fflush(fp_led4);
		fflush(fp_led5);
		fflush(fp_led6);
		fflush(fp_led7);
		fflush(fp_led8);
	}

	// Read the current value of the PB3 GPIO input.
	fscanf(fp_pb3, "%s", gpio_setting);

	// Determine whether the PL push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Slide LED to the right.
		direction = 4;

		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
		fflush(fp_led4);
		fflush(fp_led5);
		fflush(fp_led6);
		fflush(fp_led7);
		fflush(fp_led8);
	}

	// Read the current value of the PB4 GPIO input.
/*
 * 	fscanf(fp_pb4, "%s", gpio_setting);

	// Determine whether the PL push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Slide LED to the left.
		direction = 5;

		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
		fflush(fp_led4);
		fflush(fp_led5);
		fflush(fp_led6);
		fflush(fp_led7);
		fflush(fp_led8);		
	}
*/

	// This test always passes since it requires user interaction.
	test_result = 0;

	// Close the GPIO value property files.
	fclose(fp_led1);
	fclose(fp_led2);
	fclose(fp_led3);
	fclose(fp_led4);
	fclose(fp_led5);
	fclose(fp_led6);
	fclose(fp_led7);
	fclose(fp_led8);
	fclose(fp_pb1);
	fclose(fp_pb2);
	fclose(fp_pb3);
//TC	fclose(fp_pb4);

	return test_result;
}

int main()
{
	char gpio_setting[4];
	int test_result = 0;
	const int char_buf_size = 80;
	char formatted_file_name[char_buf_size];
	FILE  *fp;

	// Display the lab name in the application banner.
	printf(" \n");
	printf("***********************************************************\n");
	printf("*                                                         *\n");
	printf("*   UltraZed-EV EV Carrier Card LED and Push Button Tests *\n");
	printf("*                                                         *\n");
	printf("***********************************************************\n");
	printf(" \n");

	// Open the export file and write the PSGPIO number for each Pmod GPIO
	// signal to the Linux sysfs GPIO export property, then close the file.
	fp = fopen(FILE_FORMAT_GPIO_PATH FILE_FORMAT_GPIO_EXPORT, "w");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/export node\n");
	}
	else
	{
		// Set the value property for the export to the GPIO number for LED1.
		snprintf(gpio_setting, 4, "%d", LED1_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the value property for the export to the GPIO number for LED2.
		snprintf(gpio_setting, 4, "%d", LED2_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the value property for the export to the GPIO number for LED3.
		snprintf(gpio_setting, 4, "%d", LED3_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the value property for the export to the GPIO number for LED4.
		snprintf(gpio_setting, 4, "%d", LED4_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the value property for the export to the GPIO number for LED5.
		snprintf(gpio_setting, 4, "%d", LED5_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the value property for the export to the GPIO number for LED6.
		snprintf(gpio_setting, 4, "%d", LED6_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the value property for the export to the GPIO number for LED7.
		snprintf(gpio_setting, 4, "%d", LED7_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the value property for the export to the GPIO number for LED8.
		snprintf(gpio_setting, 4, "%d", LED8_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the value property for the export to the GPIO number for PB1.
		snprintf(gpio_setting, 4, "%d", PB1_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the value property for the export to the GPIO number for PB2.
		snprintf(gpio_setting, 4, "%d", PB2_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the value property for the export to the GPIO number for PB3.
		snprintf(gpio_setting, 4, "%d", PB3_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the value property for the export to the GPIO number for PB4.
/*TC
 * 		snprintf(gpio_setting, 4, "%d", PB4_GPIO_OFFSET);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
*/
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PB1.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, PB1_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", PB1_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "in".
		strcpy(gpio_setting, "in");
		fwrite(&gpio_setting, sizeof(char), 2, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", PB1_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("OUTPUT\n");
		}
		else
		{
			printf("INPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PB2.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, PB2_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", PB2_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "in".
		strcpy(gpio_setting, "in");
		fwrite(&gpio_setting, sizeof(char), 2, fp);
		fflush(fp);

        fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", PB2_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("OUTPUT\n");

		}
		else
		{
			printf("INPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PB3.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, PB3_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", PB3_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "in".
		strcpy(gpio_setting, "in");
		fwrite(&gpio_setting, sizeof(char), 2, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", PB3_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("OUTPUT\n");

		}
		else
		{
			printf("INPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PB4.
/*TC
 * 	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, PB4_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", PB4_GPIO_OFFSET);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", PB4_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		else
		{
			printf("INPUT\n");
		}
		fclose(fp);
	}
*/

	// Check the direction property of the PSGPIO number for LED1.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, LED1_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", LED1_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "out".
		strcpy(gpio_setting, "out");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", LED1_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("INPUT\n");
			
		}
		else
		{
			printf("OUTPUT\n");
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for LED2.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, LED2_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", LED2_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "out".
		strcpy(gpio_setting, "out");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", LED2_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("INPUT\n");
			
		}
		else
		{
			printf("OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for LED3.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, LED3_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", LED3_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "out".
		strcpy(gpio_setting, "out");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", LED3_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("INPUT\n");

		}
		else
		{
			printf("OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for LED4.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, LED4_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", LED4_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "out".
		strcpy(gpio_setting, "out");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", LED4_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("INPUT\n");

		}
		else
		{
			printf("OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for LED5.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, LED5_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", LED5_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "out".
		strcpy(gpio_setting, "out");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", LED5_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("INPUT\n");

		}
		else
		{
			printf("OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for LED6.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, LED6_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", LED6_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "out".
		strcpy(gpio_setting, "out");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", LED6_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("INPUT\n");

		}
		else
		{
			printf("OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for LED7.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, LED7_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", LED7_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "out".
		strcpy(gpio_setting, "out");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", LED7_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("INPUT\n");

		}
		else
		{
			printf("OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for LED8.
	test_result = snprintf(formatted_file_name, (char_buf_size - 1), FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION, LED8_GPIO_OFFSET);
	if ((test_result < 0) ||
		(test_result == (char_buf_size - 1)))
	{
		printf("Error formatting string, check the GPIO specified\r\n");
		printf(formatted_file_name);
		return -1;
	}
	fp = fopen(formatted_file_name, "r+");
	if (fp == NULL)
	{
		printf("Error opening "FILE_FORMAT_GPIO_PATH"/gpio%d"FILE_FORMAT_GPIO_DIRECTION" node\n", LED8_GPIO_OFFSET);
	}
	else
	{
		// Set the direction property to "out".
		strcpy(gpio_setting, "out");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fscanf(fp, "%s", gpio_setting);
		printf("gpio%d set as ", LED8_GPIO_OFFSET);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("INPUT\n");

		}
		else
		{
			printf("OUTPUT\n");
		}
		fclose(fp);
	}

	// Perform LED pattern generation.
	printf("LED Pattern Generation on UltraZed-EV EV Carrier Card\n");

	// This test always passes since it requires user interaction.
	test_result = 0;

	while (test_result == 0)
	{
		test_result = set_next_count_pattern();
		test_result = set_next_input_pattern();
		usleep(110000);
	}

	printf("LED Pattern Generation Complete...");
	
	if (test_result == 0)
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}

    exit(test_result);
}
