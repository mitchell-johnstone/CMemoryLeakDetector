#ifndef __MEM_LIB_H__
#define __MEM_LIB_H__

#include <stddef.h>

typedef struct heap {
    struct heap *   next;    // next section in heap
    void *          address; // address this mem is at in heap
    int             size;    // defines size stored!
} heap_t;

typedef struct ptr {
    struct ptr * next;
    void * address;
} ptr_t;

heap_t* heap_head = NULL;
ptr_t* ptr_head = NULL;

void my_malloc(void* pointer, size_t size);
void my_free(void* pointer);
void my_register(void* pointer);
void my_clean(void);

#endif
