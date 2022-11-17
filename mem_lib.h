#ifndef __MEM_LIB_H__
#define __MEM_LIB_H__

#include <stddef.h>

typedef struct heap {
    struct heap *   next;    // next section in heap
    void *          address; // address this mem is at in heap
    int             size;    // defines size stored!
    int             visited; // boolean check if we've visited
} heap_t;

typedef struct ptr {
    struct ptr* next;
    void**      pointer;
    int         visited;
} ptr_t;

heap_t* heap_head; // Linked list of heap recordings
ptr_t* ptr_head; // Linked list of pointer recordings

void my_gc_init(void);
void my_malloc(void** pointer, size_t size);
void my_free(void* pointer);
void my_deregister(void** pointer);
void my_register(void** pointer);
void my_base_clean(void);
void my_deep_clean(void);
void my_full_clean(void);
void my_print(void);

#endif
