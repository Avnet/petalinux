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
// Create Date:         Nov 17, 2016
// Tool versions:       Vivado 2016.2
// File Name:           gpio.c
// Description:         PL GPIO loopback Linux test routines.
//
//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// System includes.
#include <stdio.h>

// ---------------------------------------------------------------------------
// Local includes.
#include "gpio.h"

int32u gpio3_pl;
int32u gpio4_pl;
int32u gpio5_pl;

int gpio_setup(int32u verbosity)
{
	int final_result = TEST_SUCCESS;
	int unit_result = TEST_SUCCESS;

	/*
	 * Initialize the GPIO driver instances.
	 */
	unit_result = pl_gpio_initialize(&gpio3_pl, GPIO3_DEVICE_ADDRESS);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}
	else
	{
		if (verbosity)
		{
			printf("   Mapped GPIO3 PL successfully to 0x%08X\r\n", gpio3_pl);
		}
	}

	unit_result = pl_gpio_initialize(&gpio4_pl, GPIO4_DEVICE_ADDRESS);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}
	else
	{
		if (verbosity)
		{
			printf("   Mapped GPIO4 PL successfully to 0x%08X\r\n", gpio4_pl);
		}
	}

	unit_result = pl_gpio_initialize(&gpio5_pl, GPIO5_DEVICE_ADDRESS);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}
	else
	{
		if (verbosity)
		{
			printf("   Mapped GPIO5 PL successfully to 0x%08X\r\n", gpio5_pl);
		}
	}

    return final_result;
}

// ---------------------------------------------------------------------------
int test_gpio3(int32u test_pattern, int32u verbosity)
{
	int result = TEST_SUCCESS;
	int32u delay_wait = 1;
    int32u test_pattern_channel1;
    int32u test_pattern_channel2;

    /*
     * Set the direction for both of the channel1 IOs to output.
     */
    if (pl_gpio_set_data_direction(gpio3_pl, PL_GPIO_CHANNEL1, (GPIO3_BIT_MASK & ~GPIO3_BIT_MASK)) != TEST_SUCCESS)
    {
    	printf("   Can't set GPIO3 channel 1 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the direction for both of the channel2 IOs to input.
     */
    if (pl_gpio_set_data_direction(gpio3_pl, PL_GPIO_CHANNEL2, GPIO3_BIT_MASK) != TEST_SUCCESS)
    {
    	printf("   Can't set GPIO3 channel 2 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the values of each of the output channels.
     */
    test_pattern_channel1 = (test_pattern & GPIO3_BIT_MASK);
    if (pl_gpio_data_write(gpio3_pl, PL_GPIO_CHANNEL1, &test_pattern_channel1) != TEST_SUCCESS)
    {
    	printf("   Can't write data to GPIO3\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Wait for some time to get the loopback propagation latched.
     */
    while (delay_wait-- > 0);

    /*
     * Get the values of each of the input channels.
     */
    if (pl_gpio_data_read(gpio3_pl, PL_GPIO_CHANNEL2, &test_pattern_channel2) != TEST_SUCCESS)
    {
    	printf("   Can't read data from GPIO3\r\n");
    	result = TEST_FAILURE;
    }
    test_pattern_channel2 = (test_pattern_channel2 & GPIO3_BIT_MASK);

    /*
     * Compare the values of each of the result registers to determine if the
     * test pattern propagated successfully.
     */
    if (test_pattern_channel1 != test_pattern_channel2)
    {
        printf("   \033[5mFAILURE\033[0m on GPIO3: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);

        result = TEST_FAILURE;
    }
    else if (verbosity)
    {
        printf("   SUCCESS on GPIO3: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);
    }

    return result;
}

// ---------------------------------------------------------------------------
int test_gpio4(int32u test_pattern, int32u verbosity)
{
	int result = TEST_SUCCESS;
	int32u delay_wait = 1;
    int32u test_pattern_channel1;
    int32u test_pattern_channel2;

    /*
     * Set the direction for both of the channel1 IOs to output.
     */
    if (pl_gpio_set_data_direction(gpio4_pl, PL_GPIO_CHANNEL1, (GPIO4_BIT_MASK & ~GPIO4_BIT_MASK)) != TEST_SUCCESS)
    {
    	printf("   Can't set GPIO4 channel 1 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the direction for both of the channel2 IOs to input.
     */
    if (pl_gpio_set_data_direction(gpio4_pl, PL_GPIO_CHANNEL2, GPIO4_BIT_MASK) != TEST_SUCCESS)
    {
    	printf("   Can't set GPIO4 channel 2 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the values of each of the output channels.
     */
    test_pattern_channel1 = (test_pattern & GPIO4_BIT_MASK);
    if (pl_gpio_data_write(gpio4_pl, PL_GPIO_CHANNEL1, &test_pattern_channel1) != TEST_SUCCESS)
    {
    	printf("   Can't write data to GPIO4\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Wait for some time to get the loopback propagation latched.
     */
    while (delay_wait-- > 0);

    /*
     * Get the values of each of the input channels.
     */
    if (pl_gpio_data_read(gpio4_pl, PL_GPIO_CHANNEL2, &test_pattern_channel2) != TEST_SUCCESS)
    {
    	printf("   Can't read data from GPIO4\r\n");
    	result = TEST_FAILURE;
    }
    test_pattern_channel2 = (test_pattern_channel2 & GPIO4_BIT_MASK);

    /*
     * Compare the values of each of the result registers to determine if the
     * test pattern propagated successfully.
     */
    if (test_pattern_channel1 != test_pattern_channel2)
    {
        printf("   \033[5mFAILURE\033[0m on GPIO4: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);

        result = TEST_FAILURE;
    }
    else if (verbosity)
    {
        printf("   SUCCESS on GPIO4: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);
    }

    return result;
}

// ---------------------------------------------------------------------------
int test_gpio5(int32u test_pattern, int32u verbosity)
{
	int result = TEST_SUCCESS;
	int32u delay_wait = 1;
    int32u test_pattern_channel1;
    int32u test_pattern_channel2;

    /*
     * Set the direction for both of the channel1 IOs to output.
     */
    if (pl_gpio_set_data_direction(gpio5_pl, PL_GPIO_CHANNEL1, (GPIO5_BIT_MASK & ~GPIO5_BIT_MASK)) != TEST_SUCCESS)
    {
    	printf("   Can't set GPIO5 channel 1 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the direction for both of the channel2 IOs to input.
     */
    if (pl_gpio_set_data_direction(gpio5_pl, PL_GPIO_CHANNEL2, GPIO5_BIT_MASK) != TEST_SUCCESS)
    {
    	printf("   Can't set GPIO5 channel 2 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the values of each of the output channels.
     */
    test_pattern_channel1 = (test_pattern & GPIO5_BIT_MASK);
    if (pl_gpio_data_write(gpio5_pl, PL_GPIO_CHANNEL1, &test_pattern_channel1) != TEST_SUCCESS)
    {
    	printf("   Can't write data to GPIO5\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Wait for some time to get the loopback propagation latched.
     */
    while (delay_wait-- > 0);

    /*
     * Get the values of each of the input channels.
     */
    if (pl_gpio_data_read(gpio5_pl, PL_GPIO_CHANNEL2, &test_pattern_channel2) != TEST_SUCCESS)
    {
    	printf("   Can't read data from GPIO5\r\n");
    	result = TEST_FAILURE;
    }
    test_pattern_channel2 = (test_pattern_channel2 & GPIO5_BIT_MASK);

    /*
     * Compare the values of each of the result registers to determine if the
     * test pattern propagated successfully.
     */
    if (test_pattern_channel1 != test_pattern_channel2)
    {
        printf("   \033[5mFAILURE\033[0m on GPIO5: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);

        result = TEST_FAILURE;
    }
    else if (verbosity)
    {
        printf("   SUCCESS on GPIO5: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);
    }

    return result;
}

// ---------------------------------------------------------------------------
int gpio_test(int32u test_pattern, int32u verbosity)
{
    int result = TEST_SUCCESS;
    int32u fail = 0;

    /*
     * Call the individual tests for each of the Pmod pairs.
     */
    if (test_gpio3(test_pattern, verbosity) != TEST_SUCCESS)
    {
        fail = 1;
    }

    if (test_gpio4(test_pattern, verbosity) != TEST_SUCCESS)
    {
        fail = 1;
    }

    if (test_gpio5(test_pattern, verbosity) != TEST_SUCCESS)
    {
        fail = 1;
    }

    if (fail == 0)
    {
        return result;
    }

    return TEST_FAILURE;
}


// ---------------------------------------------------------------------------
int gpio_walking_ones(int32u verbosity)
{
    int bit_under_test;
    int result = TEST_SUCCESS;
    int32u fail = 0;
    int32u wones = 0x00000001;
	int32u bit_width = 32;
    
    for (bit_under_test = 0; bit_under_test < bit_width; bit_under_test++)
    {
		// Perform the test result comparison.
        result = gpio_test(wones, verbosity);
        
        if (result == TEST_FAILURE)
        {
            fail = 1;
        }

		// If this is a deep debug mode, prompt the operator before continuing
		// the test sequence.
		if (verbosity > 1)
		{
			printf("Bits currently under test: %d\r\n\r\n", bit_under_test);
		}
		
        // Shift the one in the test pattern to prepare for the next test 
        // iteration.
        wones = wones << 1;
    }
    
    // Show the result of the test as either pass or fail.
    if (verbosity > 0)
	{
		printf("Walking ones Test...");
	}
    
    if (fail == 0)
    {
        if (verbosity > 0)
        {
            printf("\033[32mPASSED\033[0m\r\n");
        }
        
        return result;
    }
    
    if (verbosity > 0)
    {
        printf("\033[5mFAILED\033[0m\r\n");
    }
    
    return TEST_FAILURE;
}


// ---------------------------------------------------------------------------
int gpio_walking_zeros(int32u verbosity)
{
    int bit_under_test;
    int result = TEST_SUCCESS;
    int32u fail = 0;
    int32u wzeros = 0xFFFFFFFE;
	int32u bit_width = 32;
    
    for (bit_under_test = 0; bit_under_test < bit_width; bit_under_test++)
    {   
        // Perform the test result comparison.
        result = gpio_test(wzeros, verbosity);
        
        if (result == TEST_FAILURE)
        {
            fail = 1;
        }

		// If this is a deep debug mode, prompt the operator before continuing
		// the test sequence.
		if (verbosity > 1)
		{
			printf("Bits currently under test: %d\r\n\r\n", bit_under_test);
		}
		
        // Shift the zero in the test pattern to prepare for the next test 
        // iteration.
        wzeros = ~(wzeros);
        wzeros = wzeros << 1;
        wzeros = ~(wzeros);        
    }

    // Show the result of the test as either pass or fail.
    if (verbosity > 0)
	{
		printf("Walking zeros Test...");
	}
    
    if (fail == 0)
    {
        if (verbosity > 0)
        {
            printf("\033[32mPASSED\033[0m\r\n");
        }
        
        return result;
    }

    if (verbosity > 0)
    {
        printf("\033[5mFAILED\033[0m\r\n");
    }
    
    return TEST_FAILURE;
}

// ---------------------------------------------------------------------------
int gpio_all_zeros(int32u verbosity)
{
	int result = TEST_SUCCESS;
    int32u fail = 0;
    int32u zeros = 0x00000000;
    
    // Perform the test result comparison.
    result = gpio_test(zeros, verbosity);
    
    if (result == TEST_FAILURE)
    {
        fail = 1;
    }
    
    // Show the result of the test as either pass or fail.
    if (verbosity > 0)
	{
		printf("All zeros Test...");
	}
    
    if (fail == 0)
    {
        if (verbosity > 0)
        {
            printf("\033[32mPASSED\033[0m\r\n");
        }
        
        return result;
    }

    if (verbosity > 0)
    {
        printf("\033[5mFAILED\033[0m\r\n");
    }
    
    return TEST_FAILURE;
}


// ---------------------------------------------------------------------------
int gpio_all_ones(int32u verbosity)
{
	int result = TEST_SUCCESS;
    int32u fail = 0;
    int32u ones = 0xFFFFFFFF;
    
    // Perform the test result comparison.
    result = gpio_test(ones, verbosity);
    
    if (result == TEST_FAILURE)
    {
        fail = 1;
    }
    
    // Show the result of the test as either pass or fail.
    if (verbosity > 0)
	{
		printf("All ones Test...");
	}
    
    if (fail == 0)
    {
        if (verbosity > 0)
        {
            printf("\033[32mPASSED\033[0m\r\n");
        }
        
        return result;
    }

    if (verbosity > 0)
    {
        printf("\033[5mFAILED\033[0m\r\n");
    }
    
    return TEST_FAILURE;
}

