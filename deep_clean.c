#include "mem_lib.h"
#include <stdlib.h>
#include <stdio.h>

struct ex_t{
    int num;
    struct ex_t* ptr;
};

int main(){
    my_gc_init();

    // main pointers
    struct ex_t* p1;
    struct ex_t* p2;
    // register them
    my_register((void**)&p1);
    my_register((void**)&p2);
    // malloc
    my_malloc((void**)&p1, sizeof(struct ex_t));
    my_malloc((void**)&p2, sizeof(struct ex_t));
    // register nested pointers
    my_register((void**)&(p1->ptr));
    my_register((void**)&(p2->ptr));
    // assign nested pointers
    p1->ptr = p2;
    p2->ptr = p1;
    // deregister top pointers
    my_deregister((void**)&p1);
    my_deregister((void**)&p2);
    my_print();
    // clean
    my_deep_clean();
    return 0;
}

