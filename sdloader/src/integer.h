/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef _INTEGER
#define _INTEGER

#ifdef _WIN32			/* FatFs development platform */

#include <windows.h>
#include <tchar.h>

#else				/* Embedded platform */

#include <sys/types.h>
#include <inttypes.h>

/* These types must be 16-bit, 32-bit or larger integer */
typedef int INT;
typedef unsigned int UINT;

/* These types must be 8-bit integer */
typedef char CHAR;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;

/* These types must be 16-bit integer */
typedef int16_t SHORT;
typedef uint16_t USHORT;
typedef uint16_t WORD;
typedef uint16_t WCHAR;

/* These types must be 32-bit integer */
typedef int32_t LONG;
typedef uint32_t ULONG;
typedef uint32_t DWORD;

#endif				/*  */

#endif				/*  */
