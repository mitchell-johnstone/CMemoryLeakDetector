#include "mem_lib.h"
#include <stdlib.h>
#include <stdio.h>

struct ex_t{
    int num;
    struct ex_t* ptr;
};

int main(){
    my_gc_init();

    struct ex_t* p1;
    struct ex_t* p2;
    // register pointers
    my_register((void**)&p1);
    my_register((void**)&p2);
    // Malloc
    my_malloc((void**)&p1, sizeof(struct ex_t));
    my_malloc((void**)&p2, sizeof(struct ex_t));
    // register inside pointers
    my_register((void**)&(p1->ptr));
    my_register((void**)&(p2->ptr));
    // link nested pointers
    p1->ptr = p2;
    p2->ptr = p1;
    // deregister top pointers
    my_deregister((void**)&p1);
    my_deregister((void**)&p2);
    // show the current memory
    my_print();
    // attempt clean
    my_base_clean();
    return 0;
}
