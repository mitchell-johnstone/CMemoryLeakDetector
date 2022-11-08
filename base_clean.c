#include "mem_lib.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    my_gc_init();

    // register pointer
    int* p;
    my_register((void**)&p);

    // malloc
    my_malloc((void**)&p,sizeof(int));
    my_base_clean(); // should be fine, nothing to clean

    // double malloc!
    my_malloc((void**)&p,sizeof(int)); // losing previous pointer in heap!
    my_print();
    my_base_clean();

    // deregister
    my_deregister((void**)&p);
    // free
    my_free((void*)p);
    // print
    my_print();

    return 0;
}
