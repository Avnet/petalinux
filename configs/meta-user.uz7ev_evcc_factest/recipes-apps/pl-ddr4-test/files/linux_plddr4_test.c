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
// Please direct any questions to the PicoZed community support forum:
//    http://www.ultrazed.org/forum
//
// Product information is available at:
//    http://www.ultrazed.org/product/ultrazed
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2018 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Jan 29, 2018
// Design Name:         UltraZed-EV PL DDR4 Test
// Module Name:         linux-plddr4_test.c
// Project Name:        UltraZed-EV PL DDR4 Test
// Target Devices:      Zynq UltraScale+
// Avnet Boards:        UltraZed-EV SOM
//
// Tool versions:       Vivado 2017.3
//
// Description:         UltraZed-EV PL DDR4 Test
//
// Dependencies:        
//
// Revision:            Jan 29, 2018: 1.00 Initial version
//
//----------------------------------------------------------------------------
//
// Use of this test application is dependent on the ZU+ PL DDR4 memory 
// interface defined in the u-boot and devicetree files:
// meta-user/recipes-bsp/u-boot/files/platform-top.h file:
// #define CONFIG_NR_DRAM_BANKS 3
// AND it also has to be described in the 
// meta-user/recipes-bsp/device-tree/files/system-user.dtsi file:
// /include/ "system-conf.dtsi"
// / {
//    #address-cells = <2>;
//    #size-cells = <2>;
//    memory {
//        device_type = "memory";
//        reg = <0x0 0x0 0x0 0x80000000>, <0x4 0x0 0x0 0x40000000>,<0x8 0x0 0x0 0x80000000>;
//    };
//    reserved-memory {
//        ranges;
//        reserved {
//            reg = <0x4 0x0 0x0 0x40000000>;
//        };
//    };
// };



// the Zynq UltraScale+ PL de
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Make the SDK console work in the debugger
#define printf(...) \
 fprintf(stdout, __VA_ARGS__); \
 fflush(stdout);

typedef long long int u64;

int main()
{
   unsigned int plddr4_size = 0x40000000;
   off_t plddr4_pbase = 0x400000000; // physical base address
   u64 *plddr4_64_vptr;
   int fd;

   // Map the BRAM physical address into user space getting a virtual address for it
   if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {

      plddr4_64_vptr = (u64 *)mmap(NULL, plddr4_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, plddr4_pbase);

      // Write to the memory that was mapped, use devmem from the command line of Linux to verify it worked
      // it could be read back here also

      plddr4_64_vptr[0] = 0xDEADBEEFFACEB00C;
      close(fd);
   }
 }
