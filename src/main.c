#include "../inc/alloc.h"
#include <stdio.h>

#define HEAP_SIZE 0x10000
char heap[HEAP_SIZE];

int main() {
    if( init_jcalloc( &heap, HEAP_SIZE ) != 0 ) {
        return -1;
    }
    
    printf("Allocing...\n");
    unsigned char *myarray = jcalloc( 0x40 );

    size_t i = 0;
    printf("Allocated, reading heap\n");
    while( i < 0x40 ) {
        printf("%02x", (unsigned)myarray[i++] );
    }

    printf("Freeing...\n");
    jcfree( myarray );
}

