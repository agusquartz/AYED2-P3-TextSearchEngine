#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ask_words(char* words);
void show_footer();
void show_title();
void show_occurrences(char** lines);

void ask_words(char* words){
    printf("\n\e[4;33mIngrese la/s palabra/s a buscar (separadas por comas):\033[0m ");
    fgets(words, 100, stdin);
    words[strcspn(words, "\n")] = 0; // Remove newline character

    if (strlen(words) == 0) {
        printf("\n\e[0;31mNo se ingresó ninguna palabra. Reintentando...\033[0m\n");
        exit(1);
    }

    printf("\n\e[0;33mBuscando la/s palabra/s: %s\033[0m\n", words);
}

void show_footer(){
    printf("\n\e[0;33m--------- Siguiente Pagina -> ENTER ----------");
    printf("\n-------------------- Salir -> ESC   ----------\033[0m\n");
}

void show_title(){
    printf("\e[4;31m==================== Word/s Search Engine ======================\033[0m\n");
}

void show_occurrences(char** lines) {
    int i = 0;
    while (lines[i][0] != '\0') {
        printf("%s\n", lines[i]);
        i++;
    }
}
