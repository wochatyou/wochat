#include <stdlib.h>
#include "dui_mempool.h"

#define Assert(condition)	((void)true)
#define StaticAssertDecl(condition, errmessage)	((void)true)
#define StaticAssertStmt(condition, errmessage)	((void)true)

#define VALGRIND_CHECK_MEM_IS_DEFINED(addr, size)			do {} while (0)
#define VALGRIND_CREATE_MEMPOOL(context, redzones, zeroed)	do {} while (0)
#define VALGRIND_DESTROY_MEMPOOL(context)					do {} while (0)
#define VALGRIND_MAKE_MEM_DEFINED(addr, size)				do {} while (0)
#define VALGRIND_MAKE_MEM_NOACCESS(addr, size)				do {} while (0)
#define VALGRIND_MAKE_MEM_UNDEFINED(addr, size)				do {} while (0)
#define VALGRIND_MEMPOOL_ALLOC(context, addr, size)			do {} while (0)
#define VALGRIND_MEMPOOL_FREE(context, addr)				do {} while (0)
#define VALGRIND_MEMPOOL_CHANGE(context, optr, nptr, size)	do {} while (0)

/*
 * Max
 *		Return the maximum of two numbers.
 */
#define Max(x, y)		((x) > (y) ? (x) : (y))

 /*
  * Min
  *		Return the minimum of two numbers.
  */
#define Min(x, y)		((x) < (y) ? (x) : (y))

/* Define bytes to use libc memset(). */
#define MEMSET_LOOP_LIMIT 1024

/* Get a bit mask of the bits set in non-long aligned addresses */
#define LONG_ALIGN_MASK (sizeof(long) - 1)
  /*
   * MemSet
   *	Exactly the same as standard library function memset(), but considerably
   *	faster for zeroing small word-aligned structures (such as parsetree nodes).
   *	This has to be a macro because the main point is to avoid function-call
   *	overhead.   However, we have also found that the loop is faster than
   *	native libc memset() on some platforms, even those with assembler
   *	memset() functions.  More research needs to be done, perhaps with
   *	MEMSET_LOOP_LIMIT tests in configure.
   */
#define MemSet(start, val, len) \
	do \
	{ \
		/* must be void* because we don't know if it is integer aligned yet */ \
		void   *_vstart = (void *) (start); \
		int		_val = (val); \
		Size	_len = (len); \
\
		if ((((uintptr_t) _vstart) & LONG_ALIGN_MASK) == 0 && \
			(_len & LONG_ALIGN_MASK) == 0 && \
			_val == 0 && \
			_len <= MEMSET_LOOP_LIMIT && \
			/* \
			 *	If MEMSET_LOOP_LIMIT == 0, optimizer should find \
			 *	the whole "if" false at compile time. \
			 */ \
			MEMSET_LOOP_LIMIT != 0) \
		{ \
			long *_start = (long *) _vstart; \
			long *_stop = (long *) ((char *) _start + _len); \
			while (_start < _stop) \
				*_start++ = 0; \
		} \
		else \
			memset(_vstart, _val, _len); \
	} while (0)

   /*
	* MemSetAligned is the same as MemSet except it omits the test to see if
	* "start" is word-aligned.  This is okay to use if the caller knows a-priori
	* that the pointer is suitably aligned (typically, because he just got it
	* from palloc(), which always delivers a max-aligned pointer).
	*/
#define MemSetAligned(start, val, len) \
	do \
	{ \
		long   *_start = (long *) (start); \
		int		_val = (val); \
		Size	_len = (len); \
\
		if ((_len & LONG_ALIGN_MASK) == 0 && \
			_val == 0 && \
			_len <= MEMSET_LOOP_LIMIT && \
			MEMSET_LOOP_LIMIT != 0) \
		{ \
			long *_stop = (long *) ((char *) _start + _len); \
			while (_start < _stop) \
				*_start++ = 0; \
		} \
		else \
			memset(_start, _val, _len); \
	} while (0)

/*
 * MemoryContextCounters
 *		Summarization state for MemoryContextStats collection.
 *
 * The set of counters in this struct is biased towards AllocSet; if we ever
 * add any context types that are based on fundamentally different approaches,
 * we might need more or different counters here.  A possible API spec then
 * would be to print only nonzero counters, but for now we just summarize in
 * the format historically used by AllocSet.
 */
typedef struct MemoryContextCounters
{
	Size		nblocks;		/* Total number of malloc blocks */
	Size		freechunks;		/* Total number of free chunks */
	Size		totalspace;		/* Total bytes requested from malloc */
	Size		freespace;		/* The unused portion of totalspace */
} MemoryContextCounters;

/*
 * Type MemoryContextData is declared in nodes/memnodes.h.  Most users
 * of memory allocation should just treat it as an abstract type, so we
 * do not provide the struct contents here.
 */
typedef struct MemoryContextData* MemoryContext;

/*
 * MemoryContext
 *		A logical context in which memory allocations occur.
 *
 * MemoryContext itself is an abstract type that can have multiple
 * implementations.
 * The function pointers in MemoryContextMethods define one specific
 * implementation of MemoryContext --- they are a virtual function table
 * in C++ terms.
 *
 * Node types that are actual implementations of memory contexts must
 * begin with the same fields as MemoryContextData.
 *
 * Note: for largely historical reasons, typedef MemoryContext is a pointer
 * to the context struct rather than the struct type itself.
 */

typedef void (*MemoryStatsPrintFunc) (MemoryContext context, void* passthru, const char* stats_string, bool print_to_stderr);

typedef struct MemoryContextMethods
{
	void*	(*alloc) (MemoryContext context, Size size);
	/* call this free_p in case someone #define's free() */
	void	(*free_p) (void* pointer);
	void*	(*realloc) (void* pointer, Size size);
	void	(*reset) (MemoryContext context);
	void	(*delete_context) (MemoryContext context);
	MemoryContext	(*get_chunk_context) (void* pointer);
	Size	(*get_chunk_space) (void* pointer);
	bool	(*is_empty) (MemoryContext context);
	void	(*stats) (MemoryContext context, MemoryStatsPrintFunc printfunc, void* passthru, MemoryContextCounters* totals,	bool print_to_stderr);
#ifdef MEMORY_CONTEXT_CHECKING
	void		(*check) (MemoryContext context);
#endif
} MemoryContextMethods;

/*
 * The first field of every node is NodeTag. Each node created (with makeNode)
 * will have one of the following tags as the value of its first field.
 *
 * Note that inserting or deleting node types changes the numbers of other
 * node types later in the list.  This is no problem during development, since
 * the node numbers are never stored on disk.  But don't do it in a released
 * branch, because that would represent an ABI break for extensions.
 */
typedef enum NodeTag
{
	T_Invalid = 0,
	T_AllocSetContext = 1,
	T_GenerationContext = 2,
	T_SlabContext = 3,
} NodeTag;

/*
 * A memory context can have callback functions registered on it.  Any such
 * function will be called once just before the context is next reset or
 * deleted.  The MemoryContextCallback struct describing such a callback
 * typically would be allocated within the context itself, thereby avoiding
 * any need to manage it explicitly (the reset/delete action will free it).
 */
typedef void (*MemoryContextCallbackFunction) (void* arg);

typedef struct MemoryContextCallback
{
	MemoryContextCallbackFunction func; /* function to call */
	void* arg;			/* argument to pass it */
	struct MemoryContextCallback* next; /* next in list of callbacks */
} MemoryContextCallback;

typedef struct MemoryContextData
{
	NodeTag		type;			/* identifies exact kind of context */
	/* these two fields are placed here to minimize alignment wastage: */
	bool		isReset;		/* T = no space alloced since last reset */
	bool		allowInCritSection; /* allow palloc in critical section */
	Size		mem_allocated;	/* track memory allocated for this context */
	const MemoryContextMethods* methods;	/* virtual function table */
	MemoryContext parent;		/* NULL if no parent (toplevel context) */
	MemoryContext firstchild;	/* head of linked list of children */
	MemoryContext prevchild;	/* previous child of same parent */
	MemoryContext nextchild;	/* next child of same parent */
	const char* name;			/* context name (just for debugging) */
	const char* ident;			/* context ID if any (just for debugging) */
	MemoryContextCallback* reset_cbs;	/* list of reset/delete callbacks */
} MemoryContextData;

/*
 * MemoryContextMethodID
 *		A unique identifier for each MemoryContext implementation which
 *		indicates the index into the mcxt_methods[] array. See mcxt.c.
 *
 * For robust error detection, ensure that MemoryContextMethodID has a value
 * for each possible bit-pattern of MEMORY_CONTEXT_METHODID_MASK, and make
 * dummy entries for unused IDs in the mcxt_methods[] array.  We also try
 * to avoid using bit-patterns as valid IDs if they are likely to occur in
 * garbage data, or if they could falsely match on chunks that are really from
 * malloc not palloc.  (We can't tell that for most malloc implementations,
 * but it happens that glibc stores flag bits in the same place where we put
 * the MemoryContextMethodID, so the possible values are predictable for it.)
 */
typedef enum MemoryContextMethodID
{
	MCTX_UNUSED1_ID,			/* 000 occurs in never-used memory */
	MCTX_UNUSED2_ID,			/* glibc malloc'd chunks usually match 001 */
	MCTX_UNUSED3_ID,			/* glibc malloc'd chunks > 128kB match 010 */
	MCTX_ASET_ID,
	MCTX_GENERATION_ID,
	MCTX_SLAB_ID,
	MCTX_ALIGNED_REDIRECT_ID,
	MCTX_UNUSED4_ID				/* 111 occurs in wipe_mem'd memory */
} MemoryContextMethodID;


/* These functions implement the MemoryContext API for AllocSet context. */
void* AllocSetAlloc(MemoryContext context, Size size);
void AllocSetFree(void* pointer);
void* AllocSetRealloc(void* pointer, Size size);
void AllocSetReset(MemoryContext context);
void AllocSetDelete(MemoryContext context);
MemoryContext AllocSetGetChunkContext(void* pointer);
Size AllocSetGetChunkSpace(void* pointer);
bool AllocSetIsEmpty(MemoryContext context);
void AllocSetStats(MemoryContext context,
	MemoryStatsPrintFunc printfunc, void* passthru,
	MemoryContextCounters* totals,
	bool print_to_stderr);
#ifdef MEMORY_CONTEXT_CHECKING
void AllocSetCheck(MemoryContext context);
#endif

/* These functions implement the MemoryContext API for Generation context. */
void* GenerationAlloc(MemoryContext context, Size size);
void GenerationFree(void* pointer);
void* GenerationRealloc(void* pointer, Size size);
void GenerationReset(MemoryContext context);
void GenerationDelete(MemoryContext context);
MemoryContext GenerationGetChunkContext(void* pointer);
Size GenerationGetChunkSpace(void* pointer);
bool GenerationIsEmpty(MemoryContext context);
void GenerationStats(MemoryContext context,
	MemoryStatsPrintFunc printfunc, void* passthru,
	MemoryContextCounters* totals,
	bool print_to_stderr);
#ifdef MEMORY_CONTEXT_CHECKING
void GenerationCheck(MemoryContext context);
#endif


/* These functions implement the MemoryContext API for Slab context. */
void* SlabAlloc(MemoryContext context, Size size);
void SlabFree(void* pointer);
void* SlabRealloc(void* pointer, Size size);
void SlabReset(MemoryContext context);
void SlabDelete(MemoryContext context);
MemoryContext SlabGetChunkContext(void* pointer);
Size SlabGetChunkSpace(void* pointer);
bool SlabIsEmpty(MemoryContext context);
void SlabStats(MemoryContext context,
	MemoryStatsPrintFunc printfunc, void* passthru,
	MemoryContextCounters* totals,
	bool print_to_stderr);
#ifdef MEMORY_CONTEXT_CHECKING
void SlabCheck(MemoryContext context);
#endif

/*
 * These functions support the implementation of palloc_aligned() and are not
 * part of a fully-fledged MemoryContext type.
 */
void AlignedAllocFree(void* pointer);
void* AlignedAllocRealloc(void* pointer, Size size);
MemoryContext AlignedAllocGetChunkContext(void* pointer);
Size AlignedAllocGetChunkSpace(void* pointer);

static void BogusFree(void* pointer) {}
static void* BogusRealloc(void* pointer, Size size) { return NULL; }
static MemoryContext BogusGetChunkContext(void* pointer) { return NULL; }
static Size BogusGetChunkSpace(void* pointer) { return 0; }

/*****************************************************************************
 *	  GLOBAL MEMORY															 *
 *****************************************************************************/

static const MemoryContextMethods mcxt_methods[] = {
	/* aset.c */
	[MCTX_ASET_ID] .alloc = AllocSetAlloc,
	[MCTX_ASET_ID].free_p = AllocSetFree,
	[MCTX_ASET_ID].realloc = AllocSetRealloc,
	[MCTX_ASET_ID].reset = AllocSetReset,
	[MCTX_ASET_ID].delete_context = AllocSetDelete,
	[MCTX_ASET_ID].get_chunk_context = AllocSetGetChunkContext,
	[MCTX_ASET_ID].get_chunk_space = AllocSetGetChunkSpace,
	[MCTX_ASET_ID].is_empty = AllocSetIsEmpty,
	[MCTX_ASET_ID].stats = AllocSetStats,
#ifdef MEMORY_CONTEXT_CHECKING
	[MCTX_ASET_ID].check = AllocSetCheck,
#endif

	/* generation.c */
	[MCTX_GENERATION_ID].alloc = GenerationAlloc,
	[MCTX_GENERATION_ID].free_p = GenerationFree,
	[MCTX_GENERATION_ID].realloc = GenerationRealloc,
	[MCTX_GENERATION_ID].reset = GenerationReset,
	[MCTX_GENERATION_ID].delete_context = GenerationDelete,
	[MCTX_GENERATION_ID].get_chunk_context = GenerationGetChunkContext,
	[MCTX_GENERATION_ID].get_chunk_space = GenerationGetChunkSpace,
	[MCTX_GENERATION_ID].is_empty = GenerationIsEmpty,
	[MCTX_GENERATION_ID].stats = GenerationStats,
#ifdef MEMORY_CONTEXT_CHECKING
	[MCTX_GENERATION_ID].check = GenerationCheck,
#endif

	/* slab.c */
	[MCTX_SLAB_ID].alloc = SlabAlloc,
	[MCTX_SLAB_ID].free_p = SlabFree,
	[MCTX_SLAB_ID].realloc = SlabRealloc,
	[MCTX_SLAB_ID].reset = SlabReset,
	[MCTX_SLAB_ID].delete_context = SlabDelete,
	[MCTX_SLAB_ID].get_chunk_context = SlabGetChunkContext,
	[MCTX_SLAB_ID].get_chunk_space = SlabGetChunkSpace,
	[MCTX_SLAB_ID].is_empty = SlabIsEmpty,
	[MCTX_SLAB_ID].stats = SlabStats,
#ifdef MEMORY_CONTEXT_CHECKING
	[MCTX_SLAB_ID].check = SlabCheck,
#endif

	/* alignedalloc.c */
	[MCTX_ALIGNED_REDIRECT_ID].alloc = NULL,	/* not required */
	[MCTX_ALIGNED_REDIRECT_ID].free_p = AlignedAllocFree,
	[MCTX_ALIGNED_REDIRECT_ID].realloc = AlignedAllocRealloc,
	[MCTX_ALIGNED_REDIRECT_ID].reset = NULL,	/* not required */
	[MCTX_ALIGNED_REDIRECT_ID].delete_context = NULL,	/* not required */
	[MCTX_ALIGNED_REDIRECT_ID].get_chunk_context = AlignedAllocGetChunkContext,
	[MCTX_ALIGNED_REDIRECT_ID].get_chunk_space = AlignedAllocGetChunkSpace,
	[MCTX_ALIGNED_REDIRECT_ID].is_empty = NULL, /* not required */
	[MCTX_ALIGNED_REDIRECT_ID].stats = NULL,	/* not required */
#ifdef MEMORY_CONTEXT_CHECKING
	[MCTX_ALIGNED_REDIRECT_ID].check = NULL,	/* not required */
#endif

	/*
	 * Unused (as yet) IDs should have dummy entries here.  This allows us to
	 * fail cleanly if a bogus pointer is passed to pfree or the like.  It
	 * seems sufficient to provide routines for the methods that might get
	 * invoked from inspection of a chunk (see MCXT_METHOD calls below).
	 */

	[MCTX_UNUSED1_ID].free_p = BogusFree,
	[MCTX_UNUSED1_ID].realloc = BogusRealloc,
	[MCTX_UNUSED1_ID].get_chunk_context = BogusGetChunkContext,
	[MCTX_UNUSED1_ID].get_chunk_space = BogusGetChunkSpace,

	[MCTX_UNUSED2_ID].free_p = BogusFree,
	[MCTX_UNUSED2_ID].realloc = BogusRealloc,
	[MCTX_UNUSED2_ID].get_chunk_context = BogusGetChunkContext,
	[MCTX_UNUSED2_ID].get_chunk_space = BogusGetChunkSpace,

	[MCTX_UNUSED3_ID].free_p = BogusFree,
	[MCTX_UNUSED3_ID].realloc = BogusRealloc,
	[MCTX_UNUSED3_ID].get_chunk_context = BogusGetChunkContext,
	[MCTX_UNUSED3_ID].get_chunk_space = BogusGetChunkSpace,

	[MCTX_UNUSED4_ID].free_p = BogusFree,
	[MCTX_UNUSED4_ID].realloc = BogusRealloc,
	[MCTX_UNUSED4_ID].get_chunk_context = BogusGetChunkContext,
	[MCTX_UNUSED4_ID].get_chunk_space = BogusGetChunkSpace,
};

/*
 * MemoryContextCreate
 *		Context-type-independent part of context creation.
 *
 * This is only intended to be called by context-type-specific
 * context creation routines, not by the unwashed masses.
 *
 * The memory context creation procedure goes like this:
 *	1.  Context-type-specific routine makes some initial space allocation,
 *		including enough space for the context header.  If it fails,
 *		it can ereport() with no damage done.
 *	2.	Context-type-specific routine sets up all type-specific fields of
 *		the header (those beyond MemoryContextData proper), as well as any
 *		other management fields it needs to have a fully valid context.
 *		Usually, failure in this step is impossible, but if it's possible
 *		the initial space allocation should be freed before ereport'ing.
 *	3.	Context-type-specific routine calls MemoryContextCreate() to fill in
 *		the generic header fields and link the context into the context tree.
 *	4.  We return to the context-type-specific routine, which finishes
 *		up type-specific initialization.  This routine can now do things
 *		that might fail (like allocate more memory), so long as it's
 *		sure the node is left in a state that delete will handle.
 *
 * node: the as-yet-uninitialized common part of the context header node.
 * tag: NodeTag code identifying the memory context type.
 * method_id: MemoryContextMethodID of the context-type being created.
 * parent: parent context, or NULL if this will be a top-level context.
 * name: name of context (must be statically allocated).
 *
 * Context routines generally assume that MemoryContextCreate can't fail,
 * so this can contain Assert but not elog/ereport.
 */
void MemoryContextCreate(MemoryContext node, NodeTag tag, MemoryContextMethodID method_id, MemoryContext parent, const char* name)
{
	/* Creating new memory contexts is not allowed in a critical section */
	Assert(CritSectionCount == 0);

	/* Initialize all standard fields of memory context header */
	node->type = tag;
	node->isReset = true;
	node->methods = &mcxt_methods[method_id];
	node->parent = parent;
	node->firstchild = NULL;
	node->mem_allocated = 0;
	node->prevchild = NULL;
	node->name = name;
	node->ident = NULL;
	node->reset_cbs = NULL;

	/* OK to link node into context tree */
	if (parent)
	{
		node->nextchild = parent->firstchild;
		if (parent->firstchild != NULL)
			parent->firstchild->prevchild = node;
		parent->firstchild = node;
		/* inherit allowInCritSection flag from parent */
		node->allowInCritSection = parent->allowInCritSection;
	}
	else
	{
		node->nextchild = NULL;
		node->allowInCritSection = false;
	}

	VALGRIND_CREATE_MEMPOOL(node, 0, false);
}

/*
 * CurrentMemoryContext
 *		Default memory context for allocations.
 */
MemoryContext CurrentMemoryContext = NULL;

/*
 * Standard top-level contexts. For a description of the purpose of each
 * of these contexts, refer to src/backend/utils/mmgr/README
 */
MemoryContext TopMemoryContext = NULL;
MemoryContext ErrorContext = NULL;


/*
 * Recommended default alloc parameters, suitable for "ordinary" contexts
 * that might hold quite a lot of data.
 */
#define ALLOCSET_DEFAULT_MINSIZE   0
#define ALLOCSET_DEFAULT_INITSIZE  (8 * 1024)
#define ALLOCSET_DEFAULT_MAXSIZE   (8 * 1024 * 1024)
#define ALLOCSET_DEFAULT_SIZES \
	ALLOCSET_DEFAULT_MINSIZE, ALLOCSET_DEFAULT_INITSIZE, ALLOCSET_DEFAULT_MAXSIZE

 /*
  * Recommended alloc parameters for "small" contexts that are never expected
  * to contain much data (for example, a context to contain a query plan).
  */
#define ALLOCSET_SMALL_MINSIZE	 0
#define ALLOCSET_SMALL_INITSIZE  (1 * 1024)
#define ALLOCSET_SMALL_MAXSIZE	 (8 * 1024)
#define ALLOCSET_SMALL_SIZES \
	ALLOCSET_SMALL_MINSIZE, ALLOCSET_SMALL_INITSIZE, ALLOCSET_SMALL_MAXSIZE

/*--------------------
 * Chunk freelist k holds chunks of size 1 << (k + ALLOC_MINBITS),
 * for k = 0 .. ALLOCSET_NUM_FREELISTS-1.
 *
 * Note that all chunks in the freelists have power-of-2 sizes.  This
 * improves recyclability: we may waste some space, but the wasted space
 * should stay pretty constant as requests are made and released.
 *
 * A request too large for the last freelist is handled by allocating a
 * dedicated block from malloc().  The block still has a block header and
 * chunk header, but when the chunk is freed we'll return the whole block
 * to malloc(), not put it on our freelists.
 *
 * CAUTION: ALLOC_MINBITS must be large enough so that
 * 1<<ALLOC_MINBITS is at least MAXALIGN,
 * or we may fail to align the smallest chunks adequately.
 * 8-byte alignment is enough on all currently known machines.  This 8-byte
 * minimum also allows us to store a pointer to the next freelist item within
 * the chunk of memory itself.
 *
 * With the current parameters, request sizes up to 8K are treated as chunks,
 * larger requests go into dedicated blocks.  Change ALLOCSET_NUM_FREELISTS
 * to adjust the boundary point; and adjust ALLOCSET_SEPARATE_THRESHOLD in
 * memutils.h to agree.  (Note: in contexts with small maxBlockSize, we may
 * set the allocChunkLimit to less than 8K, so as to avoid space wastage.)
 *--------------------
 */

#define ALLOC_MINBITS		3	/* smallest chunk size is 8 bytes */
#define ALLOCSET_NUM_FREELISTS	11
#define ALLOC_CHUNK_LIMIT	(1 << (ALLOCSET_NUM_FREELISTS-1+ALLOC_MINBITS))
 /* Size of largest chunk that we use a fixed size for */
#define ALLOC_CHUNK_FRACTION	4
/* We allow chunks to be at most 1/4 of maxBlockSize (less overhead) */

#define ALLOC_BLOCKHDRSZ	MAXALIGN(sizeof(AllocBlockData))
#define ALLOC_CHUNKHDRSZ	sizeof(MemoryChunk)

typedef struct AllocBlockData* AllocBlock;	/* forward reference */

/*
 * AllocPointer
 *		Aligned pointer which may be a member of an allocation set.
 */
typedef void* AllocPointer;

typedef struct MemoryChunk
{
#ifdef MEMORY_CONTEXT_CHECKING
	Size		requested_size;
#endif

	/* bitfield for storing details about the chunk */
	uint64		hdrmask;		/* must be last */
} MemoryChunk;

/*
 * AllocFreeListLink
 *		When pfreeing memory, if we maintain a freelist for the given chunk's
 *		size then we use a AllocFreeListLink to point to the current item in
 *		the AllocSetContext's freelist and then set the given freelist element
 *		to point to the chunk being freed.
 */
typedef struct AllocFreeListLink
{
	MemoryChunk* next;
} AllocFreeListLink;

/*
 * Obtain a AllocFreeListLink for the given chunk.  Allocation sizes are
 * always at least sizeof(AllocFreeListLink), so we reuse the pointer's memory
 * itself to store the freelist link.
 */
#define GetFreeListLink(chkptr) \
	(AllocFreeListLink *) ((char *) (chkptr) + ALLOC_CHUNKHDRSZ)

 /* Validate a freelist index retrieved from a chunk header */
#define FreeListIdxIsValid(fidx) \
	((fidx) >= 0 && (fidx) < ALLOCSET_NUM_FREELISTS)

/* Determine the size of the chunk based on the freelist index */
#define GetChunkSizeFromFreeListIdx(fidx) \
	((((Size) 1) << ALLOC_MINBITS) << (fidx))

/*
 * AllocSetContext is our standard implementation of MemoryContext.
 *
 * Note: header.isReset means there is nothing for AllocSetReset to do.
 * This is different from the aset being physically empty (empty blocks list)
 * because we will still have a keeper block.  It's also different from the set
 * being logically empty, because we don't attempt to detect pfree'ing the
 * last active chunk.
 */
typedef struct AllocSetContext
{
	MemoryContextData header;	/* Standard memory-context fields */
	/* Info about storage allocated in this context: */
	AllocBlock	blocks;			/* head of list of blocks in this set */
	MemoryChunk* freelist[ALLOCSET_NUM_FREELISTS];	/* free chunk lists */
	/* Allocation parameters for this context: */
	Size		initBlockSize;	/* initial block size */
	Size		maxBlockSize;	/* maximum block size */
	Size		nextBlockSize;	/* next block size to allocate */
	Size		allocChunkLimit;	/* effective chunk size limit */
	AllocBlock	keeper;			/* keep this block over resets */
	/* freelist this context could be put in, or -1 if not a candidate: */
	int			freeListIndex;	/* index in context_freelists[], or -1 */
} AllocSetContext;

typedef AllocSetContext* AllocSet;

/*
 * AllocBlock
 *		An AllocBlock is the unit of memory that is obtained by aset.c
 *		from malloc().  It contains one or more MemoryChunks, which are
 *		the units requested by palloc() and freed by pfree(). MemoryChunks
 *		cannot be returned to malloc() individually, instead they are put
 *		on freelists by pfree() and re-used by the next palloc() that has
 *		a matching request size.
 *
 *		AllocBlockData is the header data for a block --- the usable space
 *		within the block begins at the next alignment boundary.
 */
typedef struct AllocBlockData
{
	AllocSet	aset;			/* aset that owns this block */
	AllocBlock	prev;			/* prev block in aset's blocks list, if any */
	AllocBlock	next;			/* next block in aset's blocks list, if any */
	char* freeptr;		/* start of free space in this block */
	char* endptr;			/* end of space in this block */
}	AllocBlockData;

/*
 * PointerIsValid
 *		True iff pointer is valid.
 */
#define PointerIsValid(pointer) ((const void*)(pointer) != NULL)

/*
 * AllocPointerIsValid
 *		True iff pointer is valid allocation pointer.
 */
#define AllocPointerIsValid(pointer) PointerIsValid(pointer)

#define IsA(nodeptr,_type_)		(nodeTag(nodeptr) == T_##_type_)

 /*
  * AllocSetIsValid
  *		True iff set is valid allocation set.
  */
#define AllocSetIsValid(set) \
	(PointerIsValid(set) && IsA(set, AllocSetContext))

  /*
   * AllocBlockIsValid
   *		True iff block is valid block of allocation set.
   */
#define AllocBlockIsValid(block) \
	(PointerIsValid(block) && AllocSetIsValid((block)->aset))


/*
 * Rather than repeatedly creating and deleting memory contexts, we keep some
 * freed contexts in freelists so that we can hand them out again with little
 * work.  Before putting a context in a freelist, we reset it so that it has
 * only its initial malloc chunk and no others.  To be a candidate for a
 * freelist, a context must have the same minContextSize/initBlockSize as
 * other contexts in the list; but its maxBlockSize is irrelevant since that
 * doesn't affect the size of the initial chunk.
 *
 * We currently provide one freelist for ALLOCSET_DEFAULT_SIZES contexts
 * and one for ALLOCSET_SMALL_SIZES contexts; the latter works for
 * ALLOCSET_START_SMALL_SIZES too, since only the maxBlockSize differs.
 *
 * Ordinarily, we re-use freelist contexts in last-in-first-out order, in
 * hopes of improving locality of reference.  But if there get to be too
 * many contexts in the list, we'd prefer to drop the most-recently-created
 * contexts in hopes of keeping the process memory map compact.
 * We approximate that by simply deleting all existing entries when the list
 * overflows, on the assumption that queries that allocate a lot of contexts
 * will probably free them in more or less reverse order of allocation.
 *
 * Contexts in a freelist are chained via their nextchild pointers.
 */
#define MAX_FREE_CONTEXTS 100	/* arbitrary limit on freelist length */

typedef struct AllocSetFreeList
{
	int			num_free;		/* current list length */
	AllocSetContext* first_free;	/* list header */
} AllocSetFreeList;

/* context_freelists[0] is for default params, [1] for small params */
static AllocSetFreeList context_freelists[2] =
{
	{
		0, NULL
	},
	{
		0, NULL
	}
};

/*
 * Array giving the position of the left-most set bit for each possible
 * byte value.  We count the right-most position as the 0th bit, and the
 * left-most the 7th bit.  The 0th entry of the array should not be used.
 *
 * Note: this is not used by the functions in pg_bitutils.h when
 * HAVE__BUILTIN_CLZ is defined, but we provide it anyway, so that
 * extensions possibly compiled with a different compiler can use it.
 */
static const uint8 pg_leftmost_one_pos[256] = {
	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

/* ----------
 * AllocSetFreeIndex -
 *
 *		Depending on the size of an allocation compute which freechunk
 *		list of the alloc set it belongs to.  Caller must have verified
 *		that size <= ALLOC_CHUNK_LIMIT.
 * ----------
 */
static inline int
AllocSetFreeIndex(Size size)
{
	int			idx;

	if (size > (1 << ALLOC_MINBITS))
	{
		/*----------
		 * At this point we must compute ceil(log2(size >> ALLOC_MINBITS)).
		 * This is the same as
		 *		pg_leftmost_one_pos32((size - 1) >> ALLOC_MINBITS) + 1
		 * or equivalently
		 *		pg_leftmost_one_pos32(size - 1) - ALLOC_MINBITS + 1
		 *
		 * However, for platforms without intrinsic support, we duplicate the
		 * logic here, allowing an additional optimization.  It's reasonable
		 * to assume that ALLOC_CHUNK_LIMIT fits in 16 bits, so we can unroll
		 * the byte-at-a-time loop in pg_leftmost_one_pos32 and just handle
		 * the last two bytes.
		 *
		 * Yes, this function is enough of a hot-spot to make it worth this
		 * much trouble.
		 *----------
		 */
#ifdef HAVE_BITSCAN_REVERSE
		idx = pg_leftmost_one_pos32((uint32)size - 1) - ALLOC_MINBITS + 1;
#else
		uint32		t, tsize;

		/* Statically assert that we only have a 16-bit input value. */
		/*
		StaticAssertDecl(ALLOC_CHUNK_LIMIT < (1 << 16),
			"ALLOC_CHUNK_LIMIT must be less than 64kB");
		*/
		tsize = size - 1;
		t = tsize >> 8;
		idx = t ? pg_leftmost_one_pos[t] + 8 : pg_leftmost_one_pos[tsize];
		idx -= ALLOC_MINBITS - 1;
#endif
		Assert(idx < ALLOCSET_NUM_FREELISTS);
	}
	else
		idx = 0;

	return idx;
}



/* These functions implement the MemoryContext API for AllocSet context. */
void* AllocSetAlloc(MemoryContext context, Size size)
{
	return NULL;
}

void AllocSetFree(void* pointer)
{

}

void* AllocSetRealloc(void* pointer, Size size)
{
	return NULL;
}

void AllocSetReset(MemoryContext context)
{

}

void AllocSetDelete(MemoryContext context)
{

}

MemoryContext AllocSetGetChunkContext(void* pointer)
{
	return NULL;
}

Size AllocSetGetChunkSpace(void* pointer)
{
	return 0;
}

bool AllocSetIsEmpty(MemoryContext context)
{
	return false;
}

void AllocSetStats(MemoryContext context,
	MemoryStatsPrintFunc printfunc, void* passthru,
	MemoryContextCounters* totals,
	bool print_to_stderr)
{

}

#ifdef MEMORY_CONTEXT_CHECKING
void AllocSetCheck(MemoryContext context)
{
	
}
#endif

/* These functions implement the MemoryContext API for Generation context. */
void* GenerationAlloc(MemoryContext context, Size size)
{
	return NULL;
}

void GenerationFree(void* pointer)
{
}

void* GenerationRealloc(void* pointer, Size size)
{
	return NULL;
}

void GenerationReset(MemoryContext context)
{
}

void GenerationDelete(MemoryContext context)
{
}

MemoryContext GenerationGetChunkContext(void* pointer)
{
	return NULL;
}

Size GenerationGetChunkSpace(void* pointer)
{
	return 0;
}

bool GenerationIsEmpty(MemoryContext context)
{
	return false;
}

void GenerationStats(MemoryContext context,
	MemoryStatsPrintFunc printfunc, void* passthru,
	MemoryContextCounters* totals,
	bool print_to_stderr)
{
}

#ifdef MEMORY_CONTEXT_CHECKING
void GenerationCheck(MemoryContext context)
{
}

#endif


/* These functions implement the MemoryContext API for Slab context. */
void* SlabAlloc(MemoryContext context, Size size)
{
	return NULL;
}

void SlabFree(void* pointer)
{
}

void* SlabRealloc(void* pointer, Size size)
{
	return NULL;
}

void SlabReset(MemoryContext context)
{
}

void SlabDelete(MemoryContext context)
{
}

MemoryContext SlabGetChunkContext(void* pointer)
{
	return NULL;
}

Size SlabGetChunkSpace(void* pointer)
{
	return 0;
}

bool SlabIsEmpty(MemoryContext context)
{
	return false;
}

void SlabStats(MemoryContext context,
	MemoryStatsPrintFunc printfunc, void* passthru,
	MemoryContextCounters* totals,
	bool print_to_stderr)
{
}

#ifdef MEMORY_CONTEXT_CHECKING
void SlabCheck(MemoryContext context)
{
}

#endif

/*
 * These functions support the implementation of palloc_aligned() and are not
 * part of a fully-fledged MemoryContext type.
 */
void AlignedAllocFree(void* pointer)
{
}

void* AlignedAllocRealloc(void* pointer, Size size)
{
	return NULL;
}

MemoryContext AlignedAllocGetChunkContext(void* pointer)
{
	return NULL;
}

Size AlignedAllocGetChunkSpace(void* pointer)
{
	return 0;
}


/*
 * Public routines
 */

 /*
  * AllocSetContextCreateInternal
  *		Create a new AllocSet context.
  *
  * parent: parent context, or NULL if top-level context
  * name: name of context (must be statically allocated)
  * minContextSize: minimum context size
  * initBlockSize: initial allocation block size
  * maxBlockSize: maximum allocation block size
  *
  * Most callers should abstract the context size parameters using a macro
  * such as ALLOCSET_DEFAULT_SIZES.
  *
  * Note: don't call this directly; go through the wrapper macro
  * AllocSetContextCreate.
  */
MemoryContext AllocSetContextCreateInternal(MemoryContext parent, const char* name, Size minContextSize, Size initBlockSize, Size maxBlockSize)
{
	int			freeListIndex;
	Size		firstBlockSize;
	AllocSet	set;
	AllocBlock	block;

	/* ensure MemoryChunk's size is properly maxaligned */
	StaticAssertDecl(ALLOC_CHUNKHDRSZ == MAXALIGN(ALLOC_CHUNKHDRSZ), "sizeof(MemoryChunk) is not maxaligned");
	/* check we have enough space to store the freelist link */
	StaticAssertDecl(sizeof(AllocFreeListLink) <= (1 << ALLOC_MINBITS),
		"sizeof(AllocFreeListLink) larger than minimum allocation size");

	/*
	 * First, validate allocation parameters.  Once these were regular runtime
	 * tests and elog's, but in practice Asserts seem sufficient because
	 * nobody varies their parameters at runtime.  We somewhat arbitrarily
	 * enforce a minimum 1K block size.  We restrict the maximum block size to
	 * MEMORYCHUNK_MAX_BLOCKOFFSET as MemoryChunks are limited to this in
	 * regards to addressing the offset between the chunk and the block that
	 * the chunk is stored on.  We would be unable to store the offset between
	 * the chunk and block for any chunks that were beyond
	 * MEMORYCHUNK_MAX_BLOCKOFFSET bytes into the block if the block was to be
	 * larger than this.
	 */
	Assert(initBlockSize == MAXALIGN(initBlockSize) && initBlockSize >= 1024);
	Assert(maxBlockSize == MAXALIGN(maxBlockSize) && maxBlockSize >= initBlockSize && AllocHugeSizeIsValid(maxBlockSize)); /* must be safe to double */
	Assert(minContextSize == 0 || (minContextSize == MAXALIGN(minContextSize) && minContextSize >= 1024 && minContextSize <= maxBlockSize));
	Assert(maxBlockSize <= MEMORYCHUNK_MAX_BLOCKOFFSET);

	/*
	 * Check whether the parameters match either available freelist.  We do
	 * not need to demand a match of maxBlockSize.
	 */
	if (minContextSize == ALLOCSET_DEFAULT_MINSIZE && initBlockSize == ALLOCSET_DEFAULT_INITSIZE)
		freeListIndex = 0;
	else if (minContextSize == ALLOCSET_SMALL_MINSIZE && initBlockSize == ALLOCSET_SMALL_INITSIZE)
		freeListIndex = 1;
	else
		freeListIndex = -1;

	/*
	 * If a suitable freelist entry exists, just recycle that context.
	 */
	if (freeListIndex >= 0)
	{
		AllocSetFreeList* freelist = &context_freelists[freeListIndex];

		if (freelist->first_free != NULL)
		{
			/* Remove entry from freelist */
			set = freelist->first_free;
			freelist->first_free = (AllocSet)set->header.nextchild;
			freelist->num_free--;

			/* Update its maxBlockSize; everything else should be OK */
			set->maxBlockSize = maxBlockSize;

			/* Reinitialize its header, installing correct name and parent */
			MemoryContextCreate((MemoryContext)set,
				T_AllocSetContext,
				MCTX_ASET_ID,
				parent,
				name);

			((MemoryContext)set)->mem_allocated = set->keeper->endptr - ((char*)set);

			return (MemoryContext)set;
		}
	}

	/* Determine size of initial block */
	firstBlockSize = MAXALIGN(sizeof(AllocSetContext)) + ALLOC_BLOCKHDRSZ + ALLOC_CHUNKHDRSZ;
	if (minContextSize != 0)
		firstBlockSize = Max(firstBlockSize, minContextSize);
	else
		firstBlockSize = Max(firstBlockSize, initBlockSize);

	/*
	 * Allocate the initial block.  Unlike other aset.c blocks, it starts with
	 * the context header and its block header follows that.
	 */
	set = (AllocSet)malloc(firstBlockSize);
	if (set == NULL)
	{
		if (TopMemoryContext)
			MemoryContextStats(TopMemoryContext);
#if 0
		ereport(ERROR,
			(errcode(ERRCODE_OUT_OF_MEMORY),
				errmsg("out of memory"),
				errdetail("Failed while creating memory context \"%s\".",
					name)));
#endif 
		return NULL;
	}

	/*
	 * Avoid writing code that can fail between here and MemoryContextCreate;
	 * we'd leak the header/initial block if we ereport in this stretch.
	 */

	/* Fill in the initial block's block header */
	block = (AllocBlock)(((char*)set) + MAXALIGN(sizeof(AllocSetContext)));
	block->aset = set;
	block->freeptr = ((char*)block) + ALLOC_BLOCKHDRSZ;
	block->endptr = ((char*)set) + firstBlockSize;
	block->prev = NULL;
	block->next = NULL;

	/* Mark unallocated space NOACCESS; leave the block header alone. */
	VALGRIND_MAKE_MEM_NOACCESS(block->freeptr, block->endptr - block->freeptr);

	/* Remember block as part of block list */
	set->blocks = block;
	/* Mark block as not to be released at reset time */
	set->keeper = block;

	/* Finish filling in aset-specific parts of the context header */
	MemSetAligned(set->freelist, 0, sizeof(set->freelist));

	set->initBlockSize = initBlockSize;
	set->maxBlockSize = maxBlockSize;
	set->nextBlockSize = initBlockSize;
	set->freeListIndex = freeListIndex;

	/*
	 * Compute the allocation chunk size limit for this context.  It can't be
	 * more than ALLOC_CHUNK_LIMIT because of the fixed number of freelists.
	 * If maxBlockSize is small then requests exceeding the maxBlockSize, or
	 * even a significant fraction of it, should be treated as large chunks
	 * too.  For the typical case of maxBlockSize a power of 2, the chunk size
	 * limit will be at most 1/8th maxBlockSize, so that given a stream of
	 * requests that are all the maximum chunk size we will waste at most
	 * 1/8th of the allocated space.
	 *
	 * Also, allocChunkLimit must not exceed ALLOCSET_SEPARATE_THRESHOLD.
	 */
	StaticAssertStmt(ALLOC_CHUNK_LIMIT == ALLOCSET_SEPARATE_THRESHOLD,	"ALLOC_CHUNK_LIMIT != ALLOCSET_SEPARATE_THRESHOLD");

	/*
	 * Determine the maximum size that a chunk can be before we allocate an
	 * entire AllocBlock dedicated for that chunk.  We set the absolute limit
	 * of that size as ALLOC_CHUNK_LIMIT but we reduce it further so that we
	 * can fit about ALLOC_CHUNK_FRACTION chunks this size on a maximally
	 * sized block.  (We opt to keep allocChunkLimit a power-of-2 value
	 * primarily for legacy reasons rather than calculating it so that exactly
	 * ALLOC_CHUNK_FRACTION chunks fit on a maximally sized block.)
	 */
	set->allocChunkLimit = ALLOC_CHUNK_LIMIT;
	while ((Size)(set->allocChunkLimit + ALLOC_CHUNKHDRSZ) >
		(Size)((maxBlockSize - ALLOC_BLOCKHDRSZ) / ALLOC_CHUNK_FRACTION))
		set->allocChunkLimit >>= 1;

	/* Finally, do the type-independent part of context creation */
	MemoryContextCreate((MemoryContext)set,
		T_AllocSetContext,
		MCTX_ASET_ID,
		parent,
		name);

	((MemoryContext)set)->mem_allocated = firstBlockSize;

	return (MemoryContext)set;
}


