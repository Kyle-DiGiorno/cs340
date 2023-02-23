/**
 * Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *heap_ptr = NULL;
void *end_heap_ptr = NULL;

typedef struct _metadata_t
{
  unsigned int size;    // The size of the memory block.
  unsigned char isUsed; // 0 if the block is free; 1 if the block is used.
  struct _metadata_t *next_free;
  struct _metadata_t *prev_free;
} metadata_t;

metadata_t *free_head = NULL;
metadata_t *free_tail = NULL;

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 * 
 */
void* linked_list_update(metadata_t* prev_meta){
  if (prev_meta->next_free->prev_free)
  {
    prev_meta->next_free->prev_free = prev_meta->prev_free;
  }
  else
  {
    free_tail = prev_meta->prev_free;
  }
  if (prev_meta->prev_free->next_free)
  {
    prev_meta->prev_free->next_free = prev_meta->next_free;
  }
  else
  {
    free_head = prev_meta->next_free;
  }
  return prev_meta + sizeof(metadata_t);
}

void* cal_logic(metadata_t* prev_meta,metadata_t* curMeta,size_t size){
  
    if (prev_meta->size >= size)
    {
      // printf("SIRED\n");
      if (prev_meta->size <= size + sizeof(metadata_t))
      {
        prev_meta->isUsed = 1;
       // printf("^0^\n");
        //linked_list_update(prev_meta);
        return prev_meta + sizeof(metadata_t);
      }  
    metadata_t *split = (void *)prev_meta + size + sizeof(metadata_t);
      split->size = prev_meta->size - (size + sizeof(metadata_t));
      prev_meta->size = size;
      split->isUsed = 0;
      if (prev_meta->prev_free)
      {
        prev_meta->prev_free->next_free = split;
        split->prev_free = prev_meta->prev_free;
      }
      else
      {
        free_head = split;
      }
      if (prev_meta->next_free)
      {
        split->next_free = prev_meta->next_free;
      }
      else
      {
        free_tail = split;
        split->next_free = NULL;
      }
    if(prev_meta->size == 0){
      linked_list_update(prev_meta);
    }
    prev_meta->isUsed = 1;
    //printf("*2*\n");
    return prev_meta + sizeof(metadata_t);
  }
  return NULL;
}

void *c_malloc(size_t size)
{
  //printf("HHHHHHHHHH");
  metadata_t *curMeta = free_head;
  if (heap_ptr == NULL)
  {
    end_heap_ptr = sbrk(0);
    heap_ptr = sbrk(0);
  }

  while ((void *)curMeta != NULL)
  {
    metadata_t *prev_meta = curMeta;

    curMeta = curMeta->next_free;
    void* k = cal_logic(prev_meta, curMeta, size);
    if(k){
      //linked_list_update((metadata_t*)(k));
      //printf("((3))");
      //printf("%d\n",k);
      return k;
    }
    
  }
  metadata_t *meta = sbrk(sizeof(metadata_t));
  meta->size = size;
  meta->isUsed = 1;
  //printf("IOI%d:",size);
  void *data = sbrk(size);
  //printf("&1&");
  return data;
}
void *calloc(size_t num, size_t size)
{
  // if(num*size == 0){
  //   //printf("GTHJKIUYGFVBNHJUYHGFBNJ\n");
  // }
  // printf("\nHARVEY PETET\n--------------------\npppppp\n");
  // implement calloc:
  // void *get_mal = malloc(size * num);
  // if (get_mal == NULL)
  // {
  //   return NULL;
  // }
  // printf(get_mal);
  // //printf("HJHTYU");
  // //memset(get_mal, 0, size * num);
  // return get_mal;
  // void *p;

  //   p = malloc (size * num);
  //   if (p == 0)
  //       return (p);

  //   bzero (p, num * size);
  //   return (p);
  // void* p;
  // size_t sizeInBytes;

  // /* calculate actual memory size in bytes */
  // sizeInBytes = num * size;
  // /* allocate memory block */
  // p = mem1_malloc(sizeInBytes);
  // /* initialize memory block to zero */
  // if (p != NULL_PTR)
  // {
  //   mem1_memset(p, 0, sizeInBytes);
  // }
  // return p;
  // printf("%d\n",size);
  // printf("%d\n",num);
  char *p;

	// If either is zero just return NULL.
	if (num == 0 || size == 0)
	{
    //printf("@#$^&*I(O)P\n");
		return NULL;
	}
	else
	{
    //printf("@5@\n");
		p = c_malloc(num * size);
    // printf("@6@\n");
    // printf("-------?:\n");
    // printf("%d\n",((metadata_t*)(p-sizeof(metadata_t)))->size);
    // printf("-------?:\n");
		bzero(p, num * size);
    // printf("@7@\n");
    
		return p;
	}

}
void* mal_logic(metadata_t* prev_meta,metadata_t* curMeta,size_t size){
  
    if (prev_meta->size >= size)
    {
      // printf("SIRED\n");
      if (prev_meta->size <= size + sizeof(metadata_t))
      {
        prev_meta->isUsed = 1;
       // printf("^0^\n");
        //linked_list_update(prev_meta);
        return prev_meta + sizeof(metadata_t);
      }  
    metadata_t *split = (void *)prev_meta + size + sizeof(metadata_t);
      split->size = prev_meta->size - (size + sizeof(metadata_t));
      prev_meta->size = size;
      split->isUsed = 0;
      if (prev_meta->prev_free)
      {
        prev_meta->prev_free->next_free = split;
        split->prev_free = prev_meta->prev_free;
      }
      else
      {
        free_head = split;
      }
      if (prev_meta->next_free)
      {
        split->next_free = prev_meta->next_free;
      }
      else
      {
        free_tail = split;
        split->next_free = NULL;
      }
    if(prev_meta->size == 0){
      linked_list_update(prev_meta);
    }
    prev_meta->isUsed = 1;
    //printf("*2*\n");
    return prev_meta + sizeof(metadata_t);
  }
  //return NULL;
}
/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */

void *malloc(size_t size)
{
  //printf("HHHHHHHHHH");
  metadata_t *curMeta = free_head;
  if (heap_ptr == NULL)
  {
    end_heap_ptr = sbrk(0);
    heap_ptr = sbrk(0);
  }

  while ((void *)curMeta != NULL)
  {
    metadata_t *prev_meta = curMeta;

    curMeta = curMeta->next_free;
    void* k = mal_logic(prev_meta, curMeta, size);
    if(k){
      //linked_list_update((metadata_t*)(k));
      //printf("((3))");
      //printf("%d\n",k);
      return k;
    }
    
  }
  metadata_t *meta = sbrk(sizeof(metadata_t));
  meta->size = size;
  meta->isUsed = 1;
  //printf("IOI%d:",size);
  void *data = sbrk(size);
  //printf("&1&");
  return data;
}



/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available agaAin for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr)
{
  metadata_t *meta = ptr - sizeof(metadata_t);
  meta->isUsed = 0;
  if (free_head == NULL)
  {
    free_head = meta;
  }
  if (free_tail)
  {
    meta->prev_free = free_tail;
    free_tail->next_free = meta;
  }
  free_tail = meta;
  free_tail->next_free = NULL;
}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size)
{
  // implement realloc:
  if (size == 0)
  {
    free(ptr);
    return NULL;
  }
  metadata_t *ptr_meta = ptr - sizeof(metadata_t);
  void *new_mem;

  if (size <= ptr_meta->size)
  {
    return ptr;
  }
  new_mem = malloc(size);
  memcpy(new_mem, ptr, ptr_meta->size);
  free(ptr);
  return new_mem;
}
