#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "HashTable.h"
#include "boolean.h"

#define NUM_TESTS 10000
#define KEY_LEN    8

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

/* Generate a random string key of length KEY_LEN */
static void gen_random_key(char *s) {
    static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < KEY_LEN; i++) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[KEY_LEN] = '\0';
}

int main(void) {
    srand(42);  // reproducible
    HashTable ht = HTCreate();
    assert(ht != NULL);

    char (*keys)[KEY_LEN+1] = malloc(NUM_TESTS * (KEY_LEN+1));
    int *values = malloc(NUM_TESTS * sizeof(int));
    assert(keys && values);

    // Insert random keys with random values
    for (int i = 0; i < NUM_TESTS; i++) {
        gen_random_key(keys[i]);
        values[i] = rand();
        BOOLEAN ok = HTPut(ht, keys[i], (void*)(intptr_t)values[i]);
        assert(ok == TRUE);
    }
    // Table size should be NUM_TESTS
    assert(HTSize(ht) == NUM_TESTS);

    // Test contains and get
    for (int i = 0; i < NUM_TESTS; i++) {
        assert(HTContains(ht, keys[i]) == TRUE);
        void *vp = NULL;
        BOOLEAN found = HTGet(ht, keys[i], &vp);
        assert(found == TRUE);
        int retrieved = (int)(intptr_t)vp;
        assert(retrieved == values[i]);
    }

    // Test updating existing keys
    for (int i = 0; i < NUM_TESTS; i++) {
        int newval = values[i] + 1;
        BOOLEAN ok = HTPut(ht, keys[i], (void*)(intptr_t)newval);
        assert(ok == TRUE);
        void *vp = NULL;
        assert(HTGet(ht, keys[i], &vp) == TRUE);
        assert((int)(intptr_t)vp == newval);
    }

    // Remove half of keys and test
    for (int i = 0; i < NUM_TESTS; i += 2) {
        BOOLEAN ok = HTRemove(ht, keys[i]);
        assert(ok == TRUE);
        assert(HTContains(ht, keys[i]) == FALSE);
    }
    // New size should be NUM_TESTS/2
    assert(HTSize(ht) == NUM_TESTS - NUM_TESTS/2);

    // Ensure remaining keys still retrievable
    for (int i = 1; i < NUM_TESTS; i += 2) {
        assert(HTContains(ht, keys[i]) == TRUE);
        void *vp = NULL;
        assert(HTGet(ht, keys[i], &vp) == TRUE);
    }

    // Clear table
    BOOLEAN destroyed = HTDestroy(ht);
    assert(destroyed == TRUE);

    free(keys);
    free(values);

    printf("All %d tests passed successfully.\n", NUM_TESTS);
    return EXIT_SUCCESS;
}
