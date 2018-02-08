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
//  Please direct any questions to the UltraZed community support forum:
//     http://www.ultrazed.org/forum
// 
//  Product information is available at:
//     http://www.ultrazed.org
// 
//  Disclaimer:
//     Avnet, Inc. makes no warranty for the use of this code or design.
//     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//     any errors, which may appear in this code, nor does it make a commitment
//     to update the information contained herein. Avnet, Inc specifically
//     disclaims any implied warranties of fitness for a particular purpose.
//                      Copyright(c) 2016 Avnet, Inc.
//                              All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Nov 18, 2016
// Design Name:         PS Pmod loopback test
// Module Name:         linux_ps_pmod_loopback_test.c
// Project Name:        PS Pmod loopback test
// Target Devices:      Xilinx Zynq and Zynq UltraScale+ MPSoC
// Hardware Boards:     UltraZed-EG SOM and I/O Carrier
//                      UltraZed-EV SOM and EV Carrier
//
// Tool versions:       Xilinx Vivado 2017.2
//                      Petalinux 2017.2
//
// Description:         PS Pmod loopback test application for Linux.
//
// Dependencies:
//
// Revision:            Nov 18, 2016: 1.00 Initial version
//                      Jan 03, 2018: 1.10 Updated for UltraZed-EV
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*****************************************************************************
* 
* Here is the PS GPIO MIO <-> PMOD_D[7:0] loopback map 
* Same for UltraZed-EG SOM + IO Carrier Card 
* and UltraZed-EV SOM + EV Carrier Card 
*
*     PMOD_D0 (MIO41) <---> PMOD_D4 (MIO36)
*     PMOD_D1 (MIO43) <---> PMOD_D5 (MIO37)
*     PMOD_D2 (MIO42) <---> PMOD_D6 (MIO39)
*     PMOD_D3 (MIO38) <---> PMOD_D7 (MIO40)
*
*
*****************************************************************************/
/*
 * The base PS GPIO is gpio231, 
 * so when PMOD_D0 = MIO41
 * then the PS GPIO for PMOD_D0 is 231+41=272, etc.
 */

#define PS_GPIO_OFFSET       231
 
#define PMOD_D0_GPIO	     ((PS_GPIO_OFFSET) + 41) // MIO41 = GPIO272 - out
#define PMOD_D1_GPIO	     ((PS_GPIO_OFFSET) + 43) // MIO43 = GPIO274 - out
#define PMOD_D2_GPIO	     ((PS_GPIO_OFFSET) + 42) // MIO42 = GPIO273 - out
#define PMOD_D3_GPIO	     ((PS_GPIO_OFFSET) + 38) // MIO38 = GPIO269 - out
#define PMOD_D4_GPIO	     ((PS_GPIO_OFFSET) + 36) // MIO36 = GPIO267 - in
#define PMOD_D5_GPIO	     ((PS_GPIO_OFFSET) + 37) // MIO37 = GPIO268 - in
#define PMOD_D6_GPIO	     ((PS_GPIO_OFFSET) + 39) // MIO39 = GPIO270 - in
#define PMOD_D7_GPIO	     ((PS_GPIO_OFFSET) + 40) // MIO40 = GPIO271 - in

#define PMOD_D0_SYSFS_PATH   "/sys/class/gpio/gpio272/"
#define PMOD_D1_SYSFS_PATH   "/sys/class/gpio/gpio274/"
#define PMOD_D2_SYSFS_PATH   "/sys/class/gpio/gpio273/"
#define PMOD_D3_SYSFS_PATH   "/sys/class/gpio/gpio269/"
#define PMOD_D4_SYSFS_PATH   "/sys/class/gpio/gpio267/"
#define PMOD_D5_SYSFS_PATH   "/sys/class/gpio/gpio268/"
#define PMOD_D6_SYSFS_PATH   "/sys/class/gpio/gpio270/"
#define PMOD_D7_SYSFS_PATH   "/sys/class/gpio/gpio271/"

int first_walking_ones_step(void)
{
	char gpio_setting[5];
	int pmod_d0_value; //pmod_d0
	int pmod_d1_value; //pmod_d1
	int pmod_d2_value; //pmod_d2
	int pmod_d3_value; //pmod_d3
	int pmod_d4_value; //pmod_d4
	int pmod_d5_value; //pmod_d5
	int pmod_d6_value; //pmod_d6
	int pmod_d7_value; //pmod_d7
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen(PMOD_D0_SYSFS_PATH "value", "r+");
	fp_pmod_d1 = fopen(PMOD_D1_SYSFS_PATH "value", "r+");
	fp_pmod_d2 = fopen(PMOD_D2_SYSFS_PATH "value", "r+");
	fp_pmod_d3 = fopen(PMOD_D3_SYSFS_PATH "value", "r+");
	fp_pmod_d4 = fopen(PMOD_D4_SYSFS_PATH "value", "r+");
	fp_pmod_d5 = fopen(PMOD_D5_SYSFS_PATH "value", "r+");
	fp_pmod_d6 = fopen(PMOD_D6_SYSFS_PATH "value", "r+");
	fp_pmod_d7 = fopen(PMOD_D7_SYSFS_PATH "value", "r+");

	// First ones step on IO.
	pmod_d0_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	pmod_d1_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	pmod_d2_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	pmod_d3_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &pmod_d4_value);
	fscanf(fp_pmod_d5, "%d", &pmod_d5_value);
	fscanf(fp_pmod_d6, "%d", &pmod_d6_value);
	fscanf(fp_pmod_d7, "%d", &pmod_d7_value);

	if (pmod_d0_value != pmod_d4_value)
	{
		test_result = -11;
		printf("Walking ones test failure on Pmod D0 to Pmod D4: %d\n", test_result);
	}
	if (pmod_d1_value != pmod_d5_value)
	{
		test_result = -12;
		printf("Walking ones test failure on Pmod D1 to Pmod D5: %d\n", test_result);
	}
	if (pmod_d2_value != pmod_d6_value)
	{
		test_result = -13;
		printf("Walking ones test failure on Pmod D2 to Pmod D6: %d\n", test_result);
	}
	if (pmod_d3_value != pmod_d7_value)
	{
		test_result = -14;
		printf("Walking ones test failure on Pmod D3 to Pmod D7: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int second_walking_ones_step(void)
{
	char gpio_setting[5];
	int pmod_d4_value;
	int pmod_d5_value;
	int pmod_d1_value;
	int pmod_d2_value;
	int pmod_d3_value;
	int pmod_d0_value;
	int pmod_d6_value;
	int pmod_d7_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen(PMOD_D0_SYSFS_PATH "value", "r+");
	fp_pmod_d1 = fopen(PMOD_D1_SYSFS_PATH "value", "r+");
	fp_pmod_d2 = fopen(PMOD_D2_SYSFS_PATH "value", "r+");
	fp_pmod_d3 = fopen(PMOD_D3_SYSFS_PATH "value", "r+");
	fp_pmod_d4 = fopen(PMOD_D4_SYSFS_PATH "value", "r+");
	fp_pmod_d5 = fopen(PMOD_D5_SYSFS_PATH "value", "r+");
	fp_pmod_d6 = fopen(PMOD_D6_SYSFS_PATH "value", "r+");
	fp_pmod_d7 = fopen(PMOD_D7_SYSFS_PATH "value", "r+");

	// Second ones step on IO.
	pmod_d0_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	pmod_d1_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	pmod_d2_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	pmod_d3_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &pmod_d4_value);
	fscanf(fp_pmod_d5, "%d", &pmod_d5_value);
	fscanf(fp_pmod_d6, "%d", &pmod_d6_value);
	fscanf(fp_pmod_d7, "%d", &pmod_d7_value);

	if (pmod_d0_value != pmod_d4_value)
	{
		test_result = -21;
		printf("Walking ones test failure on Pmod D0 to Pmod D4: %d\n", test_result);
	}
	if (pmod_d1_value != pmod_d5_value)
	{
		test_result = -22;
		printf("Walking ones test failure on Pmod D1 to Pmod D5: %d\n", test_result);
	}
	if (pmod_d2_value != pmod_d6_value)
	{
		test_result = -23;
		printf("Walking ones test failure on Pmod D2 to Pmod D6: %d\n", test_result);
	}
	if (pmod_d3_value != pmod_d7_value)
	{
		test_result = -24;
		printf("Walking ones test failure on Pmod D3 to Pmod D7: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int third_walking_ones_step(void)
{
	char gpio_setting[5];
	int pmod_d4_value;
	int pmod_d5_value;
	int pmod_d1_value;
	int pmod_d2_value;
	int pmod_d3_value;
	int pmod_d0_value;
	int pmod_d6_value;
	int pmod_d7_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen(PMOD_D0_SYSFS_PATH "value", "r+");
	fp_pmod_d1 = fopen(PMOD_D1_SYSFS_PATH "value", "r+");
	fp_pmod_d2 = fopen(PMOD_D2_SYSFS_PATH "value", "r+");
	fp_pmod_d3 = fopen(PMOD_D3_SYSFS_PATH "value", "r+");
	fp_pmod_d4 = fopen(PMOD_D4_SYSFS_PATH "value", "r+");
	fp_pmod_d5 = fopen(PMOD_D5_SYSFS_PATH "value", "r+");
	fp_pmod_d6 = fopen(PMOD_D6_SYSFS_PATH "value", "r+");
	fp_pmod_d7 = fopen(PMOD_D7_SYSFS_PATH "value", "r+");

	// Third ones step on third IO pair.
	pmod_d0_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	pmod_d1_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	pmod_d2_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	pmod_d3_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &pmod_d4_value);
	fscanf(fp_pmod_d5, "%d", &pmod_d5_value);
	fscanf(fp_pmod_d6, "%d", &pmod_d6_value);
	fscanf(fp_pmod_d7, "%d", &pmod_d7_value);

	if (pmod_d0_value != pmod_d4_value)
	{
		test_result = -31;
		printf("Walking ones test failure on Pmod D0 to Pmod D4: %d\n", test_result);
	}
	if (pmod_d1_value != pmod_d5_value)
	{
		test_result = -32;
		printf("Walking ones test failure on Pmod D1 to Pmnod D5: %d\n", test_result);
	}
	if (pmod_d2_value != pmod_d6_value)
	{
		test_result = -33;
		printf("Walking ones test failure on Pmod D2 to Pmod D6: %d\n", test_result);
	}
	if (pmod_d3_value != pmod_d7_value)
	{
		test_result = -34;
		printf("Walking ones test failure on Pmod D3 to Pmod D7: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int fourth_walking_ones_step(void)
{
	char gpio_setting[5];
	int pmod_d4_value;
	int pmod_d5_value;
	int pmod_d1_value;
	int pmod_d2_value;
	int pmod_d3_value;
	int pmod_d0_value;
	int pmod_d6_value;
	int pmod_d7_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen(PMOD_D0_SYSFS_PATH "value", "r+");
	fp_pmod_d1 = fopen(PMOD_D1_SYSFS_PATH "value", "r+");
	fp_pmod_d2 = fopen(PMOD_D2_SYSFS_PATH "value", "r+");
	fp_pmod_d3 = fopen(PMOD_D3_SYSFS_PATH "value", "r+");
	fp_pmod_d4 = fopen(PMOD_D4_SYSFS_PATH "value", "r+");
	fp_pmod_d5 = fopen(PMOD_D5_SYSFS_PATH "value", "r+");
	fp_pmod_d6 = fopen(PMOD_D6_SYSFS_PATH "value", "r+");
	fp_pmod_d7 = fopen(PMOD_D7_SYSFS_PATH "value", "r+");

	// Fourth ones step on IO pair.
	pmod_d0_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	pmod_d1_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	pmod_d2_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	pmod_d3_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &pmod_d4_value);
	fscanf(fp_pmod_d5, "%d", &pmod_d5_value);
	fscanf(fp_pmod_d6, "%d", &pmod_d6_value);
	fscanf(fp_pmod_d7, "%d", &pmod_d7_value);

	if (pmod_d0_value != pmod_d4_value)
	{
		test_result = -41;
		printf("Walking ones test failure on Pmod D0 to Pmod D4: %d\n", test_result);
	}
	if (pmod_d1_value != pmod_d5_value)
	{
		test_result = -42;
		printf("Walking ones test failure on Pmod D1 to Pmod D5: %d\n", test_result);
	}
	if (pmod_d2_value != pmod_d6_value)
	{
		test_result = -43;
		printf("Walking ones test failure on Pmod D2 to Pmod D6: %d\n", test_result);
	}
	if (pmod_d3_value != pmod_d7_value)
	{
		test_result = -44;
		printf("Walking ones test failure on Pmod D3 to Pmod D7: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}


int first_walking_zeros_step(void)
{
	char gpio_setting[5];
	int pmod_d4_value;
	int pmod_d5_value;
	int pmod_d1_value;
	int pmod_d2_value;
	int pmod_d3_value;
	int pmod_d0_value;
	int pmod_d6_value;
	int pmod_d7_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen(PMOD_D0_SYSFS_PATH "value", "r+");
	fp_pmod_d1 = fopen(PMOD_D1_SYSFS_PATH "value", "r+");
	fp_pmod_d2 = fopen(PMOD_D2_SYSFS_PATH "value", "r+");
	fp_pmod_d3 = fopen(PMOD_D3_SYSFS_PATH "value", "r+");
	fp_pmod_d4 = fopen(PMOD_D4_SYSFS_PATH "value", "r+");
	fp_pmod_d5 = fopen(PMOD_D5_SYSFS_PATH "value", "r+");
	fp_pmod_d6 = fopen(PMOD_D6_SYSFS_PATH "value", "r+");
	fp_pmod_d7 = fopen(PMOD_D7_SYSFS_PATH "value", "r+");

	// First ones step on IO.
	pmod_d0_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	pmod_d1_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	pmod_d2_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	pmod_d3_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &pmod_d4_value);
	fscanf(fp_pmod_d5, "%d", &pmod_d5_value);
	fscanf(fp_pmod_d6, "%d", &pmod_d6_value);
	fscanf(fp_pmod_d7, "%d", &pmod_d7_value);

	if (pmod_d0_value != pmod_d4_value)
	{
		test_result = -51;
		printf("Walking zeros test failure on Pmod D0 to Pmod D4: %d\n", test_result);
	}
	if (pmod_d1_value != pmod_d5_value)
	{
		test_result = -52;
		printf("Walking zeros test failure on Pmod D1 to Pmod D5: %d\n", test_result);
	}
	if (pmod_d2_value != pmod_d6_value)
	{
		test_result = -53;
		printf("Walking zeros test failure on Pmod D2 to Pmod D6: %d\n", test_result);
	}
	if (pmod_d3_value != pmod_d7_value)
	{
		test_result = -54;
		printf("Walking zeros test failure on Pmod D3 to Pmod D7: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int second_walking_zeros_step(void)
{
	char gpio_setting[5];
	int pmod_d4_value;
	int pmod_d5_value;
	int pmod_d1_value;
	int pmod_d2_value;
	int pmod_d3_value;
	int pmod_d0_value;
	int pmod_d6_value;
	int pmod_d7_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen(PMOD_D0_SYSFS_PATH "value", "r+");
	fp_pmod_d1 = fopen(PMOD_D1_SYSFS_PATH "value", "r+");
	fp_pmod_d2 = fopen(PMOD_D2_SYSFS_PATH "value", "r+");
	fp_pmod_d3 = fopen(PMOD_D3_SYSFS_PATH "value", "r+");
	fp_pmod_d4 = fopen(PMOD_D4_SYSFS_PATH "value", "r+");
	fp_pmod_d5 = fopen(PMOD_D5_SYSFS_PATH "value", "r+");
	fp_pmod_d6 = fopen(PMOD_D6_SYSFS_PATH "value", "r+");
	fp_pmod_d7 = fopen(PMOD_D7_SYSFS_PATH "value", "r+");

	// Second ones step on IO.
	pmod_d0_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	pmod_d1_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	pmod_d2_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	pmod_d3_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &pmod_d4_value);
	fscanf(fp_pmod_d5, "%d", &pmod_d5_value);
	fscanf(fp_pmod_d6, "%d", &pmod_d6_value);
	fscanf(fp_pmod_d7, "%d", &pmod_d7_value);

	if (pmod_d0_value != pmod_d4_value)
	{
		test_result = -61;
		printf("Walking zeros test failure on Pmod D0 to Pmod D4: %d\n", test_result);
	}
	if (pmod_d1_value != pmod_d5_value)
	{
		test_result = -62;
		printf("Walking zeros test failure on Pmod D1 to Pmod D5: %d\n", test_result);
	}
	if (pmod_d2_value != pmod_d6_value)
	{
		test_result = -63;
		printf("Walking zeros test failure on Pmod D2 to Pmod D6: %d\n", test_result);
	}
	if (pmod_d3_value != pmod_d7_value)
	{
		test_result = -64;
		printf("Walking zeros test failure on Pmod D3 to Pmod D7: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int third_walking_zeros_step(void)
{
	char gpio_setting[5];
	int pmod_d4_value;
	int pmod_d5_value;
	int pmod_d1_value;
	int pmod_d2_value;
	int pmod_d3_value;
	int pmod_d0_value;
	int pmod_d6_value;
	int pmod_d7_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen(PMOD_D0_SYSFS_PATH "value", "r+");
	fp_pmod_d1 = fopen(PMOD_D1_SYSFS_PATH "value", "r+");
	fp_pmod_d2 = fopen(PMOD_D2_SYSFS_PATH "value", "r+");
	fp_pmod_d3 = fopen(PMOD_D3_SYSFS_PATH "value", "r+");
	fp_pmod_d4 = fopen(PMOD_D4_SYSFS_PATH "value", "r+");
	fp_pmod_d5 = fopen(PMOD_D5_SYSFS_PATH "value", "r+");
	fp_pmod_d6 = fopen(PMOD_D6_SYSFS_PATH "value", "r+");
	fp_pmod_d7 = fopen(PMOD_D7_SYSFS_PATH "value", "r+");

	// Third ones step on third IO pair.
	pmod_d0_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	pmod_d1_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	pmod_d2_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	pmod_d3_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &pmod_d4_value);
	fscanf(fp_pmod_d5, "%d", &pmod_d5_value);
	fscanf(fp_pmod_d6, "%d", &pmod_d6_value);
	fscanf(fp_pmod_d7, "%d", &pmod_d7_value);

	if (pmod_d0_value != pmod_d4_value)
	{
		test_result = -71;
		printf("Walking zeros test failure on Pmod D0 to Pmod D4: %d\n", test_result);
	}
	if (pmod_d1_value != pmod_d5_value)
	{
		test_result = -72;
		printf("Walking zeros test failure on Pmod D1 to Pmod D5: %d\n", test_result);
	}
	if (pmod_d2_value != pmod_d6_value)
	{
		test_result = -73;
		printf("Walking zeros test failure on Pmod D2 to Pmod D6: %d\n", test_result);
	}
	if (pmod_d3_value != pmod_d7_value)
	{
		test_result = -74;
		printf("Walking zeros test failure on Pmod D3 to Pmod D7: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int fourth_walking_zeros_step(void)
{
	char gpio_setting[5];
	int pmod_d4_value;
	int pmod_d5_value;
	int pmod_d1_value;
	int pmod_d2_value;
	int pmod_d3_value;
	int pmod_d0_value;
	int pmod_d6_value;
	int pmod_d7_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen(PMOD_D0_SYSFS_PATH "value", "r+");
	fp_pmod_d1 = fopen(PMOD_D1_SYSFS_PATH "value", "r+");
	fp_pmod_d2 = fopen(PMOD_D2_SYSFS_PATH "value", "r+");
	fp_pmod_d3 = fopen(PMOD_D3_SYSFS_PATH "value", "r+");
	fp_pmod_d4 = fopen(PMOD_D4_SYSFS_PATH "value", "r+");
	fp_pmod_d5 = fopen(PMOD_D5_SYSFS_PATH "value", "r+");
	fp_pmod_d6 = fopen(PMOD_D6_SYSFS_PATH "value", "r+");
	fp_pmod_d7 = fopen(PMOD_D7_SYSFS_PATH "value", "r+");

	// Fourth ones step on IO pair.
	pmod_d0_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	pmod_d1_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	pmod_d2_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	pmod_d3_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &pmod_d4_value);
	fscanf(fp_pmod_d5, "%d", &pmod_d5_value);
	fscanf(fp_pmod_d6, "%d", &pmod_d6_value);
	fscanf(fp_pmod_d7, "%d", &pmod_d7_value);

	if (pmod_d0_value != pmod_d4_value)
	{
		test_result = -81;
		printf("Walking zeros test failure on Pmod D0 to Pmod D4: %d\n", test_result);
	}
	if (pmod_d1_value != pmod_d5_value)
	{
		test_result = -82;
		printf("Walking zeros test failure on Pmod D1 to Pmod D5: %d\n", test_result);
	}
	if (pmod_d2_value != pmod_d6_value)
	{
		test_result = -83;
		printf("Walking zeros test failure on Pmod D2 to Pmod D6: %d\n", test_result);
	}
	if (pmod_d3_value != pmod_d7_value)
	{
		test_result = -84;
		printf("Walking zeros test failure on Pmod D3 to Pmod D7: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int main()
{
	char gpio_setting[5];
    
	int test_result = 0;
	FILE  *fp;


	// Display the lab name in the application banner.
	printf("\n");
	printf("***********************************************************\n");
	printf("*                                                         *\n");
	printf("*          UltraZed-EV SOM + EV Carrier Card              *\n"); 
	printf("*                PS Pmod Loopback Test                    *\n");
	printf("*                                                         *\n");
	printf("* Pmod loopback cable must be installed on JPS1 connector *\n");
	printf("*                                                         *\n");
	printf("***********************************************************\n");
	printf("\n");
	
	// Open the export file and write the PSGPIO number for each Pmod GPIO
	// signal to the Linux sysfs GPIO export property, then close the file.
	fp = fopen("/sys/class/gpio/export", "w");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/export node\n");
	}
	else
	{
		// Set the export property for gpio272 - PMOD_D0.
        snprintf(gpio_setting, 4, "%d", PMOD_D0_GPIO);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the export property for gpio274 - PMOD_D1.
        snprintf(gpio_setting, 4, "%d", PMOD_D1_GPIO);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio273 - PMOD_D2.
        snprintf(gpio_setting, 4, "%d", PMOD_D2_GPIO);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio269 - PMOD_D3.
        snprintf(gpio_setting, 4, "%d", PMOD_D3_GPIO);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio267 - PMOD_D4.
        snprintf(gpio_setting, 4, "%d", PMOD_D4_GPIO);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio268 - PMOD_D5.
        snprintf(gpio_setting, 4, "%d", PMOD_D5_GPIO);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio270 - PMOD_D6.
        snprintf(gpio_setting, 4, "%d", PMOD_D6_GPIO);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio271 - PMOD_D7.
        snprintf(gpio_setting, 4, "%d", PMOD_D7_GPIO);
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PMOD_D0.
	fp = fopen(PMOD_D0_SYSFS_PATH "direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio%d/direction node\n", PMOD_D0_GPIO);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D0 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO %d set as INPUT\n", PMOD_D0_GPIO);
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("GPIO %d set as OUTPUT\n", PMOD_D0_GPIO);
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for PMOD_D1.
	fp = fopen(PMOD_D1_SYSFS_PATH "direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio%d/direction node\n", PMOD_D1_GPIO);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D1 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO %d set as INPUT\n", PMOD_D1_GPIO);
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("GPIO %d set as OUTPUT\n", PMOD_D1_GPIO);
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for PMOD_D2.
	fp = fopen(PMOD_D2_SYSFS_PATH "direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio%d/direction node\n", PMOD_D2_GPIO);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D2 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO %d set as INPUT\n", PMOD_D2_GPIO);
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("GPIO %d set as OUTPUT\n", PMOD_D2_GPIO);
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for PMOD_D3.
	fp = fopen(PMOD_D3_SYSFS_PATH "direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio%d/direction node\n", PMOD_D3_GPIO);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D3 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO %d set as INPUT\n", PMOD_D3_GPIO);
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("GPIO %d set as OUTPUT\n", PMOD_D3_GPIO);
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PMOD_D4.
	fp = fopen(PMOD_D4_SYSFS_PATH "direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio%d/direction node\n", PMOD_D4_GPIO);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D4 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO %d set as INPUT\n", PMOD_D4_GPIO);
		}
		else
		{
			printf("GPIO %d set as OUTPUT\n", PMOD_D4_GPIO);

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for PMOD_D5.
	fp = fopen(PMOD_D5_SYSFS_PATH "direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio%d/direction node\n", PMOD_D5_GPIO);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D5 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO %d set as INPUT\n", PMOD_D5_GPIO);
		}
		else
		{
			printf("GPIO %d set as OUTPUT\n", PMOD_D5_GPIO);

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PMOD_D6.
	fp = fopen(PMOD_D6_SYSFS_PATH "direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio%d/direction node\n", PMOD_D6_GPIO);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D6 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO %d set as INPUT\n", PMOD_D6_GPIO);
		}
		else
		{
			printf("GPIO %d set as OUTPUT\n", PMOD_D6_GPIO);

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PMOD_D7.
	fp = fopen(PMOD_D7_SYSFS_PATH "direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio%d/direction node\n", PMOD_D7_GPIO);
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D7 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO %d set as INPUT\n", PMOD_D7_GPIO);
		}
		else
		{
			printf("GPIO %d set as OUTPUT\n", PMOD_D7_GPIO);

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		fclose(fp);
	}

	// Perform a walking ones test on the Pmod GPIOs.
	printf("Walking ones test on PS Pmod JSP1\n");

	if (test_result == 0)
	{
		test_result = first_walking_ones_step();
	}

	if (test_result == 0)
	{
		test_result = second_walking_ones_step();
	}

	if (test_result == 0)
	{
		test_result = third_walking_ones_step();
	}

	if (test_result == 0)
	{
		test_result = fourth_walking_ones_step();
	}

	// Perform a walking zeros test on the Pmod GPIOs.
	printf("Walking zeros test on PS Pmod JSP1\n");

	if (test_result == 0)
	{
		test_result = first_walking_zeros_step();
	}

	if (test_result == 0)
	{
		test_result = second_walking_zeros_step();
	}

	if (test_result == 0)
	{
		test_result = third_walking_zeros_step();
	}

	if (test_result == 0)
	{
		test_result = fourth_walking_zeros_step();
	}

	printf("PS Pmod Loopback Tests complete...");
	
	if (test_result == 0)
	{
		printf("\033[32mPASSED\033[0m\n");
	}
	else
	{
		printf("\033[5mFAILED\033[0m\n");
	}

    exit(test_result);
}
