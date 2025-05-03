#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "confirm.h"
#include "HashTable.h"

FILE* open_file(const char* path);
void print_lines(FILE* file, int start_line, int end_line);
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

/*programa de prueba, implemente su codigo del proyecto 3 aqui*/

int main(int argc, char** argv) {
	if (argc < 2) {
        printf("Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }

	/*ejemplo de uso*/
	HashTable d = HTCreate();
	int v1 = 0;
	/*meter cualquier verdura*/
	HTPut(d, "papa", (void*)3000);
	HTPut(d, "cebolla", (void*)4000);
	HTPut(d, "perejil", (void*)500);
	/*sacar cualquier verdura*/
	HTGet(d, "papa", (void**)&v1);
	printf("papa %d", v1);
	//CONFIRM_RETURN(1 == 0);
	HTDestroy(d);
	system("PAUSE");

	int i;
	for (i = 1; i < argc; i++) {
        const char* filepath = argv[i];
        FILE* file = open_file(filepath);
        if (file) {
            printf("Successfully opened file: %s\n", filepath);
            fclose(file); // Close the file after opening
        } else {
            printf("Failed to open file: %s\n", filepath);
        }
    }



	return (EXIT_SUCCESS);
}

