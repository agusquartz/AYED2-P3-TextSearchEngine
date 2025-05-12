#include <stdio.h>
#include <stdlib.h>
#include "InvertedIndex.h"
#include "FileManager.h"

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

int main() {
    // Create inverted index
    printf("Creando índice invertido...\n");
    InvertedIndex* idx = II_Create();
    if (!idx) {
        fprintf(stderr, "II_Create devolvió NULL\n");
        return EXIT_FAILURE;
    }
    if (!idx->table) {
        fprintf(stderr, "idx->table es NULL\n");
        return EXIT_FAILURE;
    }

    // Load DonQuijote.txt (placed under libros/DonQuijote.txt)
    printf("Cargando archivo DonQuijote.txt...\n");
    const char* filename = "DonQuijote.txt";
    int file_id = II_LoadFile(idx, filename);
    if (file_id < 0) {
        fprintf(stderr, "Failed to load file: %s\n", filename);
        II_Destroy(idx);
        return EXIT_FAILURE;
    }
    printf("Loaded '%s' as document ID %d\n", filename, file_id);

    // Define search terms
    // char* terms[] = {"quijote", "donn", "sancho"};
    char* terms[] = {"don", "Quijote"};
    int term_count = sizeof(terms) / sizeof(terms[0]);

    // Search index
    int result_count = 0;
    printData* results = II_Search(idx, terms, term_count, &result_count);
    if (result_count == 0) {
        printf("No se encontraron resultados para los términos especificados.\n");
    } else {
        printf("Resultados encontrados (%d):\n", result_count);
        for (int i = 0; i < result_count; ++i) {
            printf("Documento %d: líneas %d a %d\n",
                   results[i].doc_id,
                   results[i].first_occurrence_line,
                   results[i].last_occurrence_line);
            // Optionally print context
            printf("--- Contenido aproximado: ---\n");
            // Reset file pointer for print_lines
            FILE* f = idx->opened_files[results[i].doc_id];
            fseek(f, 0, SEEK_SET);
            print_lines(f, results[i].first_occurrence_line, results[i].last_occurrence_line);
            printf("------------------------------\n");
        }
    }

    // Clean up
    free(results);
    II_Destroy(idx);
    return EXIT_SUCCESS;
}
