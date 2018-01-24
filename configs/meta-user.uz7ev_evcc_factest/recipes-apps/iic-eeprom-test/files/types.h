#ifndef TYPES_H
#define TYPES_H

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TEST_FAILURE
#define TEST_FAILURE (-1)
#endif

#ifndef TEST_SUCCESS
#define TEST_SUCCESS (0)
#endif

// This file may be included for embedded programs as well as for
// programs running on the host PC.  For the latter case, BOOL is
// already otherwise defined.
#ifndef HOST_COMPILE
typedef unsigned char   BOOL;
#endif // HOST_COMPILE

typedef unsigned char   int8u;
typedef unsigned char   uint8_t;

typedef char            int8s;
typedef char            int8_t;

typedef unsigned short  int16u;
typedef unsigned short  uint16_t;

typedef short           int16s;
typedef short           int16_t;

//typedef unsigned long   int32u;
//typedef long            int32s;

// On a Zynq Linux system it is okay to declare 32-bit unsigned int
// as "unsigned int" type?
typedef unsigned int    int32u;
typedef unsigned int    uint32_t;

typedef int             int32s;
typedef int             int32_t;

typedef struct _int64u {
	int32u lo_addr;
	int32u hi_addr;
} int64u;

typedef struct uint64_t {
	int32u lo_addr;
	int32u hi_addr;
} uint64_t;

#endif // TYPES_H
