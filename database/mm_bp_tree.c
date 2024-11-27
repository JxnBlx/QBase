#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "bp_tree.h"
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/mman.h>

// #include <climits>

// typedef struct {
//     // filesize 4 bytes
//     size_t filesize;
//     // root node offset 4 bytes
//     size_t root_offset;
//     // node size 
//     uint32_t node_size;
// } Header;

void initialize_file(const char* filename){
    
    FILE *file = fopen(filename, "w+");

    if (file == NULL) {
        printf("ERROR OPENING FILE");
        return;
    }

    
}

BPT* bptInit(int t){
    
}