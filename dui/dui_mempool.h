#ifndef __WT_DUI_MEMPOOL_H__
#define __WT_DUI_MEMPOOL_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void* MemoryPoolContext;

MemoryPoolContext mempool_create(const char* mempool_name, unsigned int minContextSize, unsigned int initBlockSize, unsigned int maxBlockSize);

void mempool_destroy(MemoryPoolContext cxt);

void mempool_reset(MemoryPoolContext cxt);

void* palloc(MemoryPoolContext cxt, size_t size);

void* palloc0(MemoryPoolContext cxt, size_t size);

void pfree(void* pointer);

#ifdef _DEBUG
// size_t mempool_size(MemoryPoolContext cxt);
#endif

#ifdef __cplusplus
}
#endif

#endif // __WT_DUI_MEMPOOL_H__