#include "mem_lib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/** HELPER FUNCTIONS **/

/**
 * Determines if the given pointer in in the given heap segment
 * @param pointer: the pointer to the address we need to check
 * @param heap: the block of heap to check against
 * @return true if the pointer in the heap block, false otherwise
 */
bool in_heap_block(void* pointer, heap_t* heap){
    return pointer >= heap->address && pointer < heap->address + heap->size;
}

/**
 * Determines if the given pointer is in our heap or not.
 * @param pointer: the pointer to the address we need to check
 * @return true if the pointer points to heap memory, false otherwise.
 */
bool in_heap(void* pointer){
    // loop through our heap
    for(heap_t* heap = heap_head; heap != NULL; heap = heap->next){
        // if it's in the block, it's in the heap
        if(in_heap_block(pointer, heap)){
            return true;
        }
    }
    // not in our heap
    return false;
}

/**
 * Method to determine which heap block this pointer is in;
 */
heap_t* which_heap(void* pointer){
    // loop through our heap
    for(heap_t* heap = heap_head; heap != NULL; heap = heap->next){
        // if it's in the block, it's in the heap
        if(in_heap_block(pointer, heap)){
            return heap;
        }
    }
    // not in our heap
    return NULL;

}

/**
 * Adds the size to the heap, keeping a record of the memory location it was
 * added to.
 * @param size: the size of memory to allocate on the heap, in bytes
 * @return the address of the memory
 */
void* add_to_heap(size_t size){
    // if empty, add to the head!
    if(heap_head == NULL){
        // create header block
        heap_head = (heap_t*)malloc(sizeof(heap_t));
        heap_head->next = NULL;
        heap_head->size = size;
        return heap_head->address = malloc(size);
    }
    // get to empty space
    heap_t* heap = heap_head;
    while(heap->next != NULL) heap = heap->next;
    // allocate a new block and move to the new block
    heap = heap->next = (heap_t*)malloc(sizeof(heap_t));
    heap->next = NULL;
    heap->size = size;
    return heap->address = malloc(size);
}

/**
 * Removes the heap block that the pointer points to
 * @param pointer: the pointer of the heap block to remove
 * @return true if the heap block was valid to remove and removed successfully,
 *         false otherwise.
 */
bool remove_from_heap(void* pointer){
    // if empty, nothing to remove
    if(heap_head == NULL) return false;
    // if head, special case
    if(in_heap_block(pointer,heap_head)){
        // remove the head
        heap_t* prev_head = heap_head;
        heap_head = heap_head->next;
        // free previous head
        free(prev_head->address);
        free(prev_head);
        return true;
    }
    // loop through until we get the block containing our pointer
    heap_t* heap = heap_head;
    while(heap->next != NULL){
        if(in_heap_block(pointer,heap->next)){
            // found the heap to remove! Remove it
            heap_t* prev_heap = heap->next;
            heap->next = heap->next->next;
            // free heap block
            free(prev_heap->address);
            free(prev_heap);
            return true;
        }
        heap = heap->next;
    }
    // not in our heap, so return false
    return false;
}

/**
 * Method to add the given pointer to our ptr linked list
 * @param pointer: the pointer to register in our linked list
 * @return true if the register was successful.
 */
bool add_to_ptr(void** pointer){
    if(ptr_head == NULL){
        // head empty, special case
        ptr_head = (ptr_t*)malloc(sizeof(ptr_t));
        ptr_head->next = NULL;
        ptr_head->pointer = pointer;
        return true;
    }
    ptr_t* ptr = ptr_head;
    if(ptr->pointer == pointer) return false; // no duplicates
    while(ptr->next != NULL) {
        ptr = ptr->next;
        if(ptr->pointer == pointer) return false; // no duplicates
    }
    ptr->next = (ptr_t*)malloc(sizeof(ptr_t));
    ptr->next->next = NULL;
    ptr->next->pointer = pointer;
    return true;
}

// remove the pointer from the ptr linked list
bool remove_from_ptr(void** pointer){
    if(ptr_head == NULL) return false; // empty list, no removing
    ptr_t* ptr = ptr_head;
    if(ptr_head->pointer == pointer){ // head of list, special case
        ptr_head = ptr_head->next;
        free(ptr);
        return true;
    }
    while(ptr->next != NULL){
        // if we've found the pointer to remove
        if(ptr->next->pointer == pointer){
            ptr_t* temp = ptr->next;
            ptr->next = ptr->next->next;
            free(temp);
            return true;
        }
    }
    return false;
}

/** GLOBAL FUNCTIONS **/
// initialize
void my_gc_init(void){
    heap_head = NULL;
    ptr_head = NULL;
}

// mallocs the space into the pointer
void my_malloc(void** pointer, size_t size){
    *pointer = add_to_heap(size);
    printf("Malloc'd heap %p successfully!\n", *pointer);
}

// frees the block of memory pointed to by pointer
void my_free(void* pointer){
    if(remove_from_heap(pointer)){
        printf("Removed heap %p successfully!\n", pointer);
    } else {
        printf("Could not free heap %p,\n\tnot pointing to heap!\n", pointer);
    }
}

// remove pointer from linked list;
void my_deregister(void** pointer){
    if(remove_from_ptr(pointer)){
        printf("Removed pointer %p successfully!\n",pointer);
    } else {
        printf("Could not remove pointer %p, \n\tnot in ptr\n",pointer);
    }
}

// registers a ptr in our ptr linked list
void my_register(void** pointer){
    if(add_to_ptr(pointer)){
        printf("Added pointer %p successfully!\n",pointer);
    } else {
        printf("Could not add pointer %p, \n\talready in ptr\n",pointer);
    }
}

// prints out the heap and ptr linked lists
void my_print(void){
    printf("\nHEAP:\n");
    printf("+----------------------+------------------+\n");
    heap_t* heap = heap_head;
    while(heap!=NULL){
        printf("| addr: %14p | size: %10d |\n", heap->address, heap->size);
        printf("+----------------------+------------------+\n");
        heap = heap->next;
    }
    printf("\n\nPTR:\n");
    printf("+----------------------+--------------------+\n");
    ptr_t* ptr = ptr_head;
    while(ptr != NULL){
        printf("| addr: %14p | to: %14p |\n", ptr->pointer, *(ptr->pointer));
        printf("+----------------------+--------------------+\n");
        ptr = ptr->next;
    }
    printf("\n");
}

// checks if heap blocks pointed to by a pointer
void my_base_clean(void){
    printf("Attempting base clean:\n");
    bool had_to_clean = false;
    // loop through all heap blocks
    for(heap_t* heap = heap_head; heap != NULL; ){
        bool pointed_to = false;
        // loop through all pointers
        for(ptr_t* ptr = ptr_head; ptr != NULL && !pointed_to; ptr = ptr->next){
            if(in_heap_block(*(ptr->pointer), heap)){
                pointed_to = true;
            }
        }
        heap_t* next = heap->next;
        // if we haven't been pointed to by any pointer
        if(!pointed_to){
            printf("heap %p is not being pointed to! CLEANING!\n", heap->address);
            remove_from_heap(heap->address);
            had_to_clean = true;
        }
        heap = next;
    }
    // print out message clean
    if(!had_to_clean){
        printf("Nothing to clean!\n");
    }
}

// recursive deep clean, finding all reachable memory
void my_deep_clean_rec(ptr_t* cur){
    if(cur->visited) return; // don't revisit
    cur->visited = true;

    printf("\tvisiting %p", cur);
    heap_t* heap = which_heap(cur->pointer);

    if(heap == NULL) return;
    heap->visited = true;

    for(ptr_t* ptr = ptr_head; ptr != NULL; ptr = ptr->next){
        if(ptr->visited) continue;
        if(in_heap_block(ptr->pointer,heap)){
            my_deep_clean_rec(ptr->next);
        }
    }
}
// Resets ptr visited
void ptr_reset(){
    for(ptr_t* ptr = ptr_head; ptr != NULL; ptr = ptr->next){
        ptr->visited = false;
    }
}
// Resets the heap visited
void heap_reset(){
    for(heap_t* heap = heap_head; heap != NULL; heap = heap->next){
        heap->visited = false;
    }
}
void my_deep_clean(void){
    ptr_reset();
    heap_reset();

    printf("Attempting deep clean:\n");
    bool had_to_clean = false;

    // iterate through all stack pointers
    for(ptr_t* ptr = ptr_head; ptr != NULL; ptr = ptr->next){
        if(ptr->visited) continue;
        if(!in_heap(ptr->pointer)){ // found a stack pointer!
            my_deep_clean_rec(ptr);
        }
    }

    // remove all unused items;
    for(ptr_t* ptr = ptr_head; ptr != NULL;){
        ptr_t* next = ptr->next;
        if(!ptr->visited){
            printf("ptr %p is not being pointed to from stack! CLEANING!\n", ptr->pointer);
            remove_from_ptr(ptr->pointer);
            had_to_clean = true;
        }
        ptr = next;
    }
    // remove all untouched heap
    for(heap_t* heap = heap_head; heap != NULL;){
        heap_t* next = heap->next;
        if(!heap->visited){
            printf("heap %p is not being pointed to from stack! CLEANING!\n", heap->address);
            remove_from_heap(heap->address);
            had_to_clean = true;
        }
        heap = next;
    }

    // print out message clean
    if(!had_to_clean){
        printf("Nothing to clean!\n");
    }
}

void my_full_clean(void){
    for(ptr_t* ptr = ptr_head; ptr != NULL;){
        ptr_t* next = ptr->next;
        remove_from_ptr(ptr->pointer);
        ptr = next;
    }
    for(heap_t* heap = heap_head; heap != NULL;){
        heap_t* next = heap->next;
        remove_from_heap(heap->address);
        heap = next;
    }
}
