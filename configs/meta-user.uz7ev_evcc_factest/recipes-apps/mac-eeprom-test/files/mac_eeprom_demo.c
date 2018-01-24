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
//                     Copyright(c) 2015 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Nov 24, 2015
// Design Name:         PicoZed FMC2 IIC MAC ID EEPROM Demonstration
// Module Name:         mac_eeprom_demo.c
// Project Name:        PicoZed FMC2 IIC MAC ID EEPROM Demonstration
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         I2C MAC ID Demonstration for PicoZed FMC2 Carrier
//
// Dependencies:
//
// Revision:            Nov 24, 2015: 1.00 Initial version
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>

#include "mac_eeprom_demo.h"

#define MAC_EEPROM_SLAVE_ADDRESS                    0x51

int zed_mac_eeprom_demo_init(iic_eeprom_demo_t *pDemo)
{
   int ret;

   /* IIC initialization for communicating with the EEPROM slave hardware.
    */
   printf("MAC EEPROM Initialization ...\n\r");
   ret = zed_iic_axi_init(&(pDemo->eeprom_iic),"RTC I2C", pDemo->uBaseAddr_IIC_RTC);
   if (!ret)
   {
      printf("ERROR : Failed to open AXI IIC device driver\n\r");
      return -1;
   }

   return 0;
}

/*****************************************************************************
*
* Read the contents of the EEPROM memory and dump it to the terminal.
*
* @param	None.
*
* @return	0 if successful, else -1.
*
* @note		None.
*
*****************************************************************************/
int dump_mac_eeprom_memory(iic_eeprom_demo_t *pDemo)
{
	int8u ChipAddress = MAC_EEPROM_SLAVE_ADDRESS;
	int8u RegAddress  = 0x00;
	int8u RegData[10];
	int8u ByteCount   = 0;
	int8u page = 0;
	int ret;

    // Begin printing the content table to the terminal.
    printf("+----------------------------------------------------------------------------+\r\n");
    printf("|                   Dumping IIC MAC EEPROM Contents                          |\r\n");
    printf("|----------------------------------------------------------------------------|\r\n");
    printf("|Offset:         0       1       2       3       4       5       6       7   |\r\n");
    printf("|----------------------------------------------------------------------------|\r\n");

    ByteCount = 8;

    // Display all of the data from the EEPROM memory to the terminal.
	while (page < 32)
	{
		RegAddress = page*8;

		ret = pDemo->eeprom_iic.fpIicRead(&(pDemo->eeprom_iic), ChipAddress, RegAddress, RegData, ByteCount+1);
		if (!ret)
		{
			printf("ERROR : Failed to Read from MAC EEPROM\n\r");
			return -1;
		}

		printf("|    0x%02X    |  0x%02X |  0x%02X |  0x%02X |  0x%02X |  0x%02X |  0x%02X |  0x%02X |  0x%02X |\r\n",
		page,
		RegData[0],
		RegData[1],
		RegData[2],
		RegData[3],
		RegData[4],
		RegData[5],
		RegData[6],
		RegData[7]);

		page++;
    }

    // Finish printing the content table to the terminal.
    printf("+----------------------------------------------------------------------------+\r\n");
    printf("\r\n");
    printf("\r\n");

    printf("Stored MAC Address is: %02X:%02X:%02X:%02X:%02X:%02X",
    		RegData[2],
        	RegData[3],
        	RegData[4],
        	RegData[5],
        	RegData[6],
        	RegData[7]);
    printf("\r\n");
    printf("\r\n");


    return 0;
}


/*****************************************************************************
*
* Read the contents of the EEPROM memory and dump it to the terminal.
*
* @param	None.
*
* @return	0 if successful, else -1.
*
* @note		None.
*
*****************************************************************************/
int validate_mac_eeprom_memory(iic_eeprom_demo_t *pDemo)
{
	int8u ChipAddress = MAC_EEPROM_SLAVE_ADDRESS;
	int8u RegAddress  = 0x00;
	int8u WriteData[8], ReadData[8];
	int8u ByteCount   = 0;
	int8u page = 0;
	int ret, index;

    // Begin printing the content table to the terminal.
    printf("+----------------------------------------------------------------------------+\r\n");
    printf("|                      Validating IIC MAC EEPROM Contents                    |\r\n");
    printf("|----------------------------------------------------------------------------|\r\n");

    ByteCount   = 8;

    // Display all of the data from the EEPROM memory to the terminal.
	while (page < 16)
	{
		RegAddress = page*8;

		for (index = 0; index < ByteCount; index++)
		{
			WriteData[index] = (RegAddress + index);
		}

		/* Note that a write should return ByteCount + 1 bytes if successful
		 * because the address byte is prepended to the beginning of the
		 * write buffer.
		 */
		ret = pDemo->eeprom_iic.fpIicWrite(&(pDemo->eeprom_iic), ChipAddress, RegAddress, WriteData, ByteCount);
		if (ret != (ByteCount + 1))
		{
			printf("ERROR : Failed to Write %d bytes to EEPROM page %d (%d)\n\r", (ByteCount + 1), page, ret);
			return -1;
		}

		/* Wait while write completes. */
		usleep(50000);

		ret = pDemo->eeprom_iic.fpIicRead(&(pDemo->eeprom_iic), ChipAddress, RegAddress, ReadData, ByteCount);
		if (ret != ByteCount)
		{
			printf("ERROR : Failed to Read %d bytes from EEPROM page %d (%d)\n\r", ByteCount, page, ret);
			return -1;
		}

		for (index = 0; index < ByteCount; index++)
		{
			if (WriteData[index] != ReadData[index])
			{
				printf("ERROR : Validation failed at Address: 0x%02X\n\r", (RegAddress + index));
				return -1;
			}
		}

		printf("Data write and read verified for address range 0x%02X - 0x%02X\n\r", RegAddress, (RegAddress + index));

		page++;
    }

    // Finish printing the content table to the terminal.
    printf("+----------------------------------------------------------------------------+\r\n");
    printf("\r\n");

    return 0;
}

/*****************************************************************************
*
* Read the contents of the EEPROM memory and dump it to the terminal.
*
* @param	None.
*
* @return	0 if successful, else -1.
*
* @note		None.
*
*****************************************************************************/
int erase_mac_eeprom_memory(iic_eeprom_demo_t *pDemo)
{
	int8u ChipAddress = MAC_EEPROM_SLAVE_ADDRESS;
	int8u RegAddress  = 0x00;
	int8u WriteData[8];
	int8u ByteCount   = 0;
	int8u page = 0;
	int ret, index;

	printf("Erasing MAC EEPROM...\n\r");

    ByteCount = 8;

    // Erase all of the writable memory space of the MAC EEPROM.
	while (page < 8)
	{
		RegAddress = page*8;

		for (index = 0; index < ByteCount; index++)
		{
			WriteData[index] = 0xFF;
		}

		ret = pDemo->eeprom_iic.fpIicWrite(&(pDemo->eeprom_iic), ChipAddress, RegAddress, WriteData, ByteCount);
		if (ret != (ByteCount + 1))
		{
			printf("ERROR : Failed to Write to EEPROM\n\r");
			return -1;
		}
		/* Wait while write completes. */
		usleep(50000);

		page++;
    }

    // Finish printing the content table to the terminal.
    printf("+----------------------------------------------------------------------------+\r\n");
    printf("\r\n");

    return 0;
}
