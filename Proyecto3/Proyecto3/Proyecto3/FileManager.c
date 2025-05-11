#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileManager.h"

#define BOOKS_PATH "libros/"

FILE* open_file(const char* path) {
    // Buffer para la ruta completa: libros/ + path + '\0'
    size_t len_prefix = strlen(BOOKS_PATH);
    size_t len_path   = strlen(path);
    char* fullpath    = malloc(len_prefix + len_path + 1);
    if (!fullpath) {
        perror("Error allocating memory for full path");
        return NULL;
    }

    // Construir la ruta completa
    memcpy(fullpath, BOOKS_PATH, len_prefix);
    memcpy(fullpath + len_prefix, path,      len_path);
    fullpath[len_prefix + len_path] = '\0';

    // Intentar abrir
    FILE* file = fopen(fullpath, "r");
    if (!file) {
        perror(fullpath);
        free(fullpath);
        return NULL;
    }

    free(fullpath);
    return file;
}

void print_lines(FILE* file, int start_line, int end_line) {
    if (start_line > end_line || start_line < 0 || end_line < 0) {
        printf("Invalid range: start_line (%d) should be <= end_line (%d) and >= 1.\n", start_line, end_line);
        return;
    }

    char buffer[1024]; // Buffer to hold each line
    int current_line = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        current_line++;
        if (current_line >= start_line && current_line <= end_line) {
            printf("%s", buffer);
        } else if (current_line > end_line) {
            break;
        }
    }

    if (current_line < start_line) {
        printf("File has fewer than %d lines.\n", start_line);
    }
}

int find_line_by_position(FILE* file, long position) {
    if (position < 0) {
        printf("Invalid position: %ld\n", position);
        return -1;
    }

    int current_line = 1;
    long current_pos = 0;
    int ch;

    ch = fgetc(file);
    while (ch != EOF) {
        if (current_pos == position) {
            return current_line;
        }
        if (ch == '\n') {
            current_line++;
        }
        current_pos++;
        ch = fgetc(file);
    }

    printf("Position %ld exceeds the file's size.\n", position);
    return -1;
}


