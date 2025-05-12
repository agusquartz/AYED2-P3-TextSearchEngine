#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "confirm.h"
#include "HashTable.h"
#include "InvertedIndex.h"
#include "FileManager.h"
#include "tui.c"

/* Implementado una vez por programa para establecer como manejar errores */
extern void GlobalReportarError(char* pszFile, int  iLine) {

	/* Siempre imprime el error */
	fprintf(
		stderr,
		"\nERROR NO ESPERADO: en el archivo %s linea %u",
		pszFile,
		iLine
	);

}

int main(int argc, char** argv) {
	if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

	InvertedIndex* idx = II_Create();
    if (!idx) {
        fprintf(stderr, "II_Create devolvió NULL\n");
        return EXIT_FAILURE;
    }
    if (!idx->table) {
        fprintf(stderr, "idx->table es NULL\n");
        return EXIT_FAILURE;
    }

	int file_id = II_LoadFile(idx, argv[1]);
    if (file_id < 0) {
        fprintf(stderr, "Error cargando fichero '%s'\n", argv[1]);
        II_Destroy(idx);
        return EXIT_FAILURE;
    }

	char buffer[100];
    char* terms[20];
    int term_count;
    printData *results;
    int result_count;
	while (1){
		show_title();
		printf("exit() para salir\n");
		char buffer[100];
		ask_words(buffer);

		if (strcmp(buffer, "exit()") == 0) {
            printf("\nSaliendo...\n");
            break;
        }

		char *terms[20];
		term_count = 0;
		char *tok = strtok(buffer, ",");
		while (tok && term_count < 20) {
			// recortamos espacios iniciales/finales
			while (*tok == ' ') tok++;
			char *end = tok + strlen(tok) - 1;
			while (end > tok && *end == ' ') *end-- = '\0';

			// duplicamos en un buffer modifiable que incluye '\0'
			terms[term_count] = malloc(strlen(tok) + 1);
			strcpy(terms[term_count], tok);
			term_count++;

			tok = strtok(NULL, ",");
		}

		result_count = 0;
		results = II_Search(idx, terms, term_count, &result_count);

		if (result_count == 0) {
			printf("\nNo se encontraron resultados para los términos especificados.\n");
		} else {
			printf("\nResultados encontrados (%d):\n", result_count);
			for (int i = 0; i < result_count; ++i) {
				printf("Documento %d: líneas %d a %d\n",
					results[i].doc_id,
					results[i].first_occurrence_line,
					results[i].last_occurrence_line);
				printf("--- Contenido aproximado: ---\n");
				FILE *f = idx->opened_files[results[i].doc_id];
				fseek(f, 0, SEEK_SET);
				print_lines(f,
							results[i].first_occurrence_line,
							results[i].last_occurrence_line);
				printf("------------------------------\n");
			}
		}

        for (int i = 0; i < term_count; ++i) {
            free(terms[i]);
        }

        free(results);

        printf("\n\n");
	}
	

	for (int i = 0; i < term_count; ++i) free(terms[i]);
    II_Destroy(idx);

    return EXIT_SUCCESS;
}

