#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <stdio.h>

/**
 * Print lines [start_line…end_line] de un FILE* abierto.
 */
void print_lines(FILE* file, int start_line, int end_line);

/**
 * Devuelve el número de línea (1-based) dado un byte offset.
 */
int find_line_by_position(FILE* file, long position);

FILE* open_file(const char* path);

/* obtiene las lineas completas (string) desde start_line hasta end_line, ambos incluidos
 * y devuelve un puntero a un array de punteros a char (array de strings)
 */
char** get_lines(FILE* file, int start_line, int end_line);

#endif