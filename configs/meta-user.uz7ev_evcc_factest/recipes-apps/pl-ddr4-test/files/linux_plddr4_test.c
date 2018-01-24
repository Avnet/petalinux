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
