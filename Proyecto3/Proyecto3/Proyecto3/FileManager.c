#include <stdio.h>
#include <stdlib.h>

FILE* open_file(const char* path) {
    FILE* file = fopen(path, "r");

    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    
    return file;
}
