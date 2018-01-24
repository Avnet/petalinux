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
// Create Date:         Jun 24, 2013
// Tool versions:       SDK 14.5
// File Name:           pl_gpio_udriver.c
// Description:         PL AXI GPIO mmap driver implementation.
//
//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Local includes.
#include "pl_gpio_udriver.h"

int pl_gpio_initialize(int32u* pl_gpio_pointer, int32u pl_gpio_instance_physical_address)
{
    int fd;
    unsigned char* virtual_addr_base = 0;

    /*
     * Open a file descriptor to the Zynq memory map device.
     */
    fd = open("/dev/mem", O_RDWR);

    /*
     * Map the GPIO controller block memory space to a virtual address
     * which can be manipulated.
     */
    virtual_addr_base = (unsigned char*)mmap(NULL, PL_GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)pl_gpio_instance_physical_address);

    /*
     * Check to see if there was a failure mapping the GPIO controller block
     * memory space.
     */
    if (virtual_addr_base == (unsigned char*) -1)
	{
    	close(fd);
		return TEST_FAILURE;
	}

    /*
     * All done with the device in the mapped memory space, un-map the
     * memory space.
     */
    munmap((void *)virtual_addr_base, PL_GPIO_SIZE);

    /*
     * The mapped memory space is now unmapped, close out the file descriptor.
     */
    close(fd);

    /*
     * Everything went well, assign the GPIO pointer value with the physical
     * address that was specified.
     */
    *pl_gpio_pointer = pl_gpio_instance_physical_address;

    return TEST_SUCCESS;
}

int pl_gpio_data_read(int32u pl_gpio_instance_physical_address, int channel, int32u* data)
{
    int fd;
    unsigned char* virtual_addr_base = 0;

    /*
     * Open a file descriptor to the Zynq memory map device.
     */
    fd = open("/dev/mem", O_RDWR);

    /*
     * Map the GPIO controller block memory space to a virtual address
     * which can be manipulated.
     */
    virtual_addr_base = (unsigned char*)mmap(NULL, PL_GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)pl_gpio_instance_physical_address);

    /*
	 * Check to see if there was a failure mapping the GPIO controller block
	 * memory space.
	 */
	if (virtual_addr_base == (unsigned char*) -1)
	{
		close(fd);
		return TEST_FAILURE;
	}

    if (channel == PL_GPIO_CHANNEL2)
    {
    	/*
    	 * Read the specified data from Channel 2 of the
    	 * GPIO controller block.
    	 */
    	data[0] = REG_READ(virtual_addr_base, PL_GPIO2_DATA);
    }
    else
    {
    	/*
		 * Read the specified data from Channel 1 of the
		 * GPIO controller block.
		 */
    	data[0] = REG_READ(virtual_addr_base, PL_GPIO_DATA);
    }

    /*
     * All done with the device in the mapped memory space, un-map the
     * memory space.
     */
    munmap((void *)virtual_addr_base, PL_GPIO_SIZE);

    /*
     * The mapped memory space is now unmapped, close out the file descriptor.
     */
    close(fd);

    return TEST_SUCCESS;
}

int pl_gpio_data_write(int32u pl_gpio_instance_physical_address, int channel, int32u* data)
{
    int fd;
    unsigned char* virtual_addr_base = 0;

    /*
     * Open a file descriptor to the Zynq memory map device.
     */
    fd = open("/dev/mem", O_RDWR);

    /*
     * Map the GPIO controller block memory space to a virtual address
     * which can be manipulated.
     */
    virtual_addr_base = (unsigned char*)mmap(NULL, PL_GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)pl_gpio_instance_physical_address);

    /*
	 * Check to see if there was a failure mapping the GPIO controller block
	 * memory space.
	 */
	if (virtual_addr_base == (unsigned char*) -1)
	{
		close(fd);
		return TEST_FAILURE;
	}

    if (channel == PL_GPIO_CHANNEL2)
    {
    	/*
    	 * Assign the specified data to Channel 2 of the
    	 * GPIO controller block.
    	 */
    	REG_WRITE(virtual_addr_base, PL_GPIO2_DATA, data[0]);
    }
    else
    {
    	/*
		 * Assign the specified data to Channel 1 of the
		 * GPIO controller block.
		 */
    	REG_WRITE(virtual_addr_base, PL_GPIO_DATA, data[0]);
    }

    /*
     * All done with the device in the mapped memory space, un-map the
     * memory space.
     */
    munmap((void *)virtual_addr_base, PL_GPIO_SIZE);

    /*
     * The mapped memory space is now unmapped, close out the file descriptor.
     */
    close(fd);

    return TEST_SUCCESS;
}

int pl_gpio_set_data_direction(int32u pl_gpio_instance_physical_address, int channel, int32u direction_mask)
{
    int fd;
    unsigned char* virtual_addr_base = 0;

    /*
     * Open a file descriptor to the Zynq memory map device.
     */
    fd = open("/dev/mem", O_RDWR);

    /*
     * Map the GPIO controller block memory space to a virtual address
     * which can be manipulated.
     */
    virtual_addr_base = (unsigned char*)mmap(NULL, PL_GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)pl_gpio_instance_physical_address);

    /*
     * Check to see if there was a failure mapping the GPIO controller block
     * memory space.
     */
    if (virtual_addr_base == (unsigned char*) -1)
    {
        close(fd);
        return TEST_FAILURE;
    }

    if (channel == PL_GPIO_CHANNEL2)
    {
    	/*
    	 * Assign the specified data direction mask to Channel 2 of the
    	 * GPIO controller block.
    	 */
    	REG_WRITE(virtual_addr_base, PL_GPIO2_TRI, direction_mask);
    }
    else
    {
    	/*
		 * Assign the specified data direction mask to Channel 1 of the
		 * GPIO controller block.
		 */
    	REG_WRITE(virtual_addr_base, PL_GPIO_TRI, direction_mask);
    }

    /*
     * All done with the device in the mapped memory space, un-map the
     * memory space.
     */
    munmap((void *)virtual_addr_base, PL_GPIO_SIZE);

    /*
     * The mapped memory space is now unmapped, close out the file descriptor.
     */
    close(fd);

    return TEST_SUCCESS;
}
