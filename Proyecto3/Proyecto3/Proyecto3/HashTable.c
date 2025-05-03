#include <stdlib.h>
#include <string.h>
#include "HashTable.h"
#include "boolean.h"
#include "confirm.h"

#define INITIAL_CAPACITY 257  // default initial capacity for HashTable (Prime Number)
#define REHASH_THRESHOLD 0.7  // load factor threshold to trigger resize
#define TOMBSTONE ((Celda*)-1)

/* Crea un HashTable, devuelve el puntero a la estructura creada*/
HashTable HTCreate() {
    _HashTable* table = malloc(sizeof(_HashTable));
    CONFIRM_RETVAL(table != NULL, NULL);

    table->cap = INITIAL_CAPACITY;
    table->tam = 0;
    table->arr = calloc(table->cap, sizeof(Celda*));
    CONFIRM_RETVAL(table->arr != NULL, NULL);

    return table;
}

/**
 * Resize the table when load factor exceeds threshold.
 */
static BOOLEAN _resize(HashTable p) {
    int oldCap = p->cap;
    int newCap = oldCap * 2 + 1;
    Celda** oldArr = p->arr;
    int oldSize = p->tam;

    Celda** newArr = calloc(newCap, sizeof(Celda*));
    CONFIRM_RETVAL(newArr != NULL, FALSE);

    p->arr = newArr;
    p->cap = newCap;
    p->tam = 0;
    
    // reinsert existing entries
    int idx;
    for (idx = 0; idx < oldCap; idx++) {
        Celda* cell = oldArr[idx];
        if (cell != NULL && cell != TOMBSTONE) {
            // Insert into new table with same key and value
            BOOLEAN result = HTPut(p, cell->clave, cell->valor);
            
            // Clean up old cell
            free(cell->clave);
            free(cell);
            
            if (!result) {
                // Restore old state on failure
                free(p->arr);
                p->arr = oldArr;
                p->cap = oldCap;
                p->tam = oldSize;
                return FALSE;
            }
        } else if (cell == TOMBSTONE) {
            // we'll just skip tombstones
        }
    }
    free(oldArr);
    
    return TRUE;
}

/**
funcion privada de ayuda
Devuelve un numero entero grande (long) correspondiente a la clave pasada
*/
long _stringLong(char* clave) {
    unsigned long value = 0;
    size_t len = strlen(clave);
    if (len == 0) return 0;  // empty string

    // compute initial power = 27^(len-1)
    unsigned long power = 1;
    size_t i;
    for (i = 1; i < len; i++) {
        power *= 27;
    }
    for (i = 0; i < len; i++) {
        unsigned char c = clave[i];
        unsigned long val;
        if (c >= '0' && c <= '9') val = (c - '0') + 1;
        else if (c >= 'A' && c <= 'Z') val = (c - 'A') + 11;
        else if (c >= 'a' && c <= 'z') val = (c - 'a') + 11;
        else val = 0;
        value += val * power;
        power /= 27;
    }
    return (long)value;
}

/*
funcion privada de ayuda
devuelve el hash para cada intento de insercion/busqueda
*/
int _hash(HashTable hash_table, char* clave, int i) {
    // to avoid negatives, we use unsigned long for the hash generation
    unsigned long key = (unsigned long)_stringLong(clave);
    int current_capacity = hash_table->cap;

    unsigned long initial_position = key % current_capacity;
    unsigned long offset = (unsigned long)i * i;
    return (int)((initial_position + offset) % current_capacity);
}

/* Agrega el valor con la clave dada en el hash table */
BOOLEAN HTPut(HashTable p, char* clave, void* valor) {
    CONFIRM_RETVAL(p != NULL && clave != NULL, FALSE);

    // resize if load factor exceeded
    double load = (double)(p->tam + 1) / p->cap;
    if (load > REHASH_THRESHOLD) {
        CONFIRM_RETVAL(_resize(p), FALSE);
    }

    // probe to find slot or existing key
    int firstTombstone = -1;
    int i;
    for (i = 0; i < p->cap; i++) {
        int idx = _hash(p, clave, i);
        Celda* cell = p->arr[idx];

        if (cell == TOMBSTONE) {
            if (firstTombstone < 0) {
                firstTombstone = idx;  // remember first tombstone
            }
        } else if (cell == NULL) {
            int insertIdx = (firstTombstone >= 0 ? firstTombstone : idx);

            // insert new cell
            Celda* newCell = malloc(sizeof(Celda));
            CONFIRM_RETVAL(newCell != NULL, FALSE);

            newCell->clave = strdup(clave);
            CONFIRM_RETVAL(newCell->clave != NULL, FALSE);

            newCell->valor = valor;
            p->arr[insertIdx] = newCell;
            p->tam++;

            return TRUE;
        } else if (strcmp(cell->clave, clave) == 0) {
            /* update existing */
            cell->valor = valor;
            return TRUE;
        }
    }
    return FALSE;  // table full (shouldn't happen after resize)
}

/* Obtiene el valor asociado a la clave dentro del HashTable */
BOOLEAN HTGet(HashTable p, char* clave, void** retval) {
    CONFIRM_RETVAL(p != NULL && clave != NULL && retval != NULL, FALSE);

    int i;
    for (i = 0; i < p->cap; i++) {
        int idx = _hash(p, clave, i);
        Celda* cell = p->arr[idx];

        if (cell == NULL) return FALSE;

        if (cell != TOMBSTONE && strcmp(cell->clave, clave) == 0) {
            *retval = cell->valor;
            return TRUE;
        }
    }
    return FALSE;
}

/* Remueve el valor asociado a la clave pasada */
BOOLEAN HTRemove(HashTable p, char* clave) {
    CONFIRM_RETVAL(p != NULL && clave != NULL, FALSE);
    
    int i;
    for (i = 0; i < p->cap; i++) {
        int idx = _hash(p, clave, i);

        Celda* cell = p->arr[idx];
        if (cell == NULL) break;

        if (cell != TOMBSTONE && strcmp(cell->clave, clave) == 0) {
            // Free the cell and mark as tombstone
            free(cell->clave);
            free(cell);
            p->arr[idx] = TOMBSTONE;
            p->tam--;
            return TRUE;
        }
    }
    return FALSE;
}

/* Devuelve TRUE si el HashTable contiene la clave*/
BOOLEAN HTContains(HashTable p, char* clave) {
    void* tmp;
    return HTGet(p, clave, &tmp);
}

// Devuelve la cantidad de elementos (tamanho) cargados en el HashTable
BOOLEAN HTSize(HashTable p) {
    CONFIRM_RETVAL(p != NULL, 0);
    return p->tam;
}

/* Destruye la estructura*/
BOOLEAN HTDestroy(HashTable p) {
    CONFIRM_RETVAL(p != NULL, FALSE);

    int i;
    for (i = 0; i < p->cap; i++) {
        Celda* cell = p->arr[i];
        if (cell != NULL && cell != TOMBSTONE) {
            free(cell->clave);
            free(cell);
        } else if (cell == TOMBSTONE) {
            // Don't need to free anything for tombstones, so just skip them
        }
    }

    free(p->arr);
    free(p);
    return TRUE;
}