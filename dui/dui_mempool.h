#ifndef __WT_DUI_MEMPOOL_H__
#define __WT_DUI_MEMPOOL_H__

#include <stdbool.h>

#include "dui.h"

typedef unsigned char uint8;	/* == 8 bits */
typedef unsigned short uint16;	/* == 16 bits */
typedef unsigned int uint32;	/* == 32 bits */

typedef signed char int8;		/* == 8 bits */
typedef signed short int16;		/* == 16 bits */
typedef signed int int32;		/* == 32 bits */

/*
 * bitsN
 *		Unit of bitwise operation, AT LEAST N BITS IN SIZE.
 */
typedef uint8 bits8;			/* >= 8 bits */
typedef uint16 bits16;			/* >= 16 bits */
typedef uint32 bits32;			/* >= 32 bits */

#ifndef HAVE_INT64
typedef long int int64;
#endif
#ifndef HAVE_UINT64
typedef unsigned long int uint64;
#endif

/* ----------------
 * Alignment macros: align a length or address appropriately for a given type.
 * The fooALIGN() macros round up to a multiple of the required alignment,
 * while the fooALIGN_DOWN() macros round down.  The latter are more useful
 * for problems like "how many X-sized structures will fit in a page?".
 *
 * NOTE: TYPEALIGN[_DOWN] will not work if ALIGNVAL is not a power of 2.
 * That case seems extremely unlikely to be needed in practice, however.
 *
 * NOTE: MAXIMUM_ALIGNOF, and hence MAXALIGN(), intentionally exclude any
 * larger-than-8-byte types the compiler might have.
 * ----------------
 */

#define TYPEALIGN(ALIGNVAL,LEN)  \
	(((uintptr_t) (LEN) + ((ALIGNVAL) - 1)) & ~((uintptr_t) ((ALIGNVAL) - 1)))

 /* Define as the maximum alignment requirement of any C data type. */
#define MAXIMUM_ALIGNOF 8

#define MAXALIGN(LEN)			TYPEALIGN(MAXIMUM_ALIGNOF, (LEN))

/*
 * Size
 *		Size of any memory resident object, as returned by sizeof.
 */
typedef size_t Size;

#define AllocSetContextCreate AllocSetContextCreateInternal

#endif // __WT_DUI_MEMPOOL_H__