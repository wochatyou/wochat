#ifndef __WT_DUI_H__
#define __WT_DUI_H__

#include <stdint.h>
#include <assert.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif 

#define DUI_DEBUG	1
#define DUI_OK      0

#define S8      int8_t
#define S16     int16_t
#define S32     int32_t
#define S64     int64_t

#define U8      uint8_t
#define U16     uint16_t
#define U32     uint32_t
#define U64     uint64_t

#define MESSAGEMAP_TABLE_SIZE       (1<<16)

/* DUI_ALIGN() is only to be used to align on a power of 2 boundary */
#define DUI_ALIGN(size, boundary) (((size) + ((boundary) -1)) & ~((boundary) - 1))
#define DUI_ALIGN_DEFAULT32(size)   DUI_ALIGN(size, 4)
#define DUI_ALIGN_DEFAULT64(size)   DUI_ALIGN(size, 8)      /** Default alignment */
#define DUI_ALIGN_PAGE(size)        DUI_ALIGN(size, (1<<16))
#define DUI_ALIGN_TRUETYPE(size)    DUI_ALIGN(size, 64)    

/*
 * Fast MOD arithmetic, assuming that y is a power of 2 !
 */
#define DUI_MOD(x,y)			 ((x) & ((y)-1))

#ifdef _MSC_VER
#define DUI_NO_VTABLE __declspec(novtable)
#else
#define DUI_NO_VTABLE
#endif


#endif // __WT_DUI_H__