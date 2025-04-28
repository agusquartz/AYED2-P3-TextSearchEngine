#include <stdlib.h>
#include "HashTable.h"

define INITIAL_CAPACITY 101  // default initial capacity for HashTable

/* Crea un HashTable, devuelve el puntero a la estructura creada*/
HashTable HTCreate() {
    _HashTable* table = malloc(sizeof(_HashTable));
    CONFIRM_RETVAL(table != NULL, NULL);

    table->cap = INITIAL_CAPACITY;
    table->tam = 0;
    table->arr = calloc(INITIAL_CAPACITY, sizeof(Celda*));
    if (table->arr == NULL) {
        free(table);
        return NULL;
    }

    return table;
}

/**
funcion privada de ayuda
Devuelve un numero entero grande (long) correspondiente a la clave pasada
por ejemplo pueden utilizar:
_stringLong("ABC") = 11*27^2 + 12*27^1 + 13*27^0 = 8356
_stringLong("09A") = 1*27^2  + 10*27^1 +  11*27^0 = 1010

o sino algo como
_stringLong("ABC") = 65*10000 + 66*100 + 67 = 656667

*/
long _stringLong(char* clave) {
	/*AYUDA SOBRE STRINGS
	Tabla de conversion de caracteres validos
	caracteres       valor ascii
	A-Z ... a-z   =  65 al 122
	0-9           =  48 al 59

	Ejemplos
	para saber el valor ascii de un caracter
	char* a = "ABC";
	int x = a[0] // a = 65

	para saber el tamanho del string
	int tam = strlen(a); // tam = 3

	AYUDA PARA CALCULOS MATEMATICOS
	int potencia = pow(27,2) // potencia = 27^2

	*/
	return 0l;
}

/*
funcion privada de ayuda
devuelve el hash para cada intento de insercion/busqueda
sea i el numero de intento y clave la clave usada.
aplicar la formula:
hash(x,i) = ((stringInt(x) mod CAP) + i^2) mod CAP
*/

int _hash(char* clave, int i) {
	return 0;
}




/* Agrega el valor con la clave dada en el hash table, en el caso de repetir la clave se sobreescriben los datos
Devuelve TRUE si tuvo exito, sino FALSE*/
BOOLEAN HTPut(HashTable p, char* clave, void* valor) {
	/*AGREGUE SU CODIGO AQUI*/
	return FALSE;
}

/* Obtiene el valor asociado a la clave dentro del HashTable y lo pasa por referencia a retval
Devuelve TRUE si tuvo exito, sino FALSE*/
BOOLEAN HTGet(HashTable p, char* clave, void** retval) {
	/*AGREGUE SU CODIGO AQUI*/
	return FALSE;
}

/* Remueve el valor asociado a la clave pasada
Devuelve TRUE si tuvo exito, sino FALSE*/
BOOLEAN HTRemove(HashTable p, char* clave) {
	/*AGREGUE SU CODIGO AQUI*/
	return FALSE;
}

/* Devuelve TRUE si el HashTable contiene la clave*/
BOOLEAN HTContains(HashTable p, char* clave) {
	/*AGREGUE SU CODIGO AQUI*/
	return FALSE;
}

/* Devuelve la cantidad de elementos (tamanho) cargados en el HashTable*/
BOOLEAN HTSize(HashTable p) {
	/*AGREGUE SU CODIGO AQUI*/
	return FALSE;
}

/* Destruye la estructura*/
BOOLEAN HTDestroy(HashTable p) {
	/*AGREGUE SU CODIGO AQUI*/
	return FALSE;
}