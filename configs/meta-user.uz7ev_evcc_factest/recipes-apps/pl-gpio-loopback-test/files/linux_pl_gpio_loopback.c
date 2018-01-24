//----------------------------------------------------------------------------
//      _____
//     *     *
//    *____   *____
//   * *===*   *==*
//  *___*===*___**  AVNET Design Resource Center
//       *======*         www.em.avnet.com/drc
//        *====*
//----------------------------------------------------------------------------
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
//						Petalinux 2016.2
// File Name:           linux_pl_gpio_loopback.c
// Description:         PL GPIO loopback test Linux application.
//
//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// System includes.
#include <stdio.h>

// ---------------------------------------------------------------------------
// Local includes.
#include "gpio.h"
#include "platform.h"
#include "types.h"

int main()
{
	int32u verbosity = 1;
	int final_result = TEST_SUCCESS;
	int unit_result = TEST_SUCCESS;

	final_result = TEST_SUCCESS;
	unit_result = TEST_SUCCESS;

	/*
	 * Setup for the test by initializing the corresponding GPIO
	 * peripherals.
	 */
	unit_result = gpio_setup(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Perform the all zeros test, this rules out any stuck at 0 pins.
	 */
	unit_result = gpio_all_zeros(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Perform the all ones test, this rules out any stuck at 1 pins.
	 */
	unit_result = gpio_all_ones(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Perform the walking zeros test, this rules out any unexpected
	 * shorted together nets.
	 */
	unit_result = gpio_walking_zeros(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Perform the walking ones test, this rules out any unexpected
	 * shorted together nets.
	 */
	unit_result = gpio_walking_ones(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Determine overall test result and print results to test operator.
	 */
	printf("PL GPIO Loopback Test: ");

	if (final_result == TEST_SUCCESS)
	{
		printf("\033[32mPASSED\033[0m\r\n");
	}
	else
	{
		printf("\033[5mFAILED\033[0m\r\n");
	}

    return final_result;
}
