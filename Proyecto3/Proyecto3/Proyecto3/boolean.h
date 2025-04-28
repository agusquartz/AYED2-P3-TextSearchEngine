

/* Las siguientes dos lineas y el #endif al final protegen al archivo .h 
   para que no sea inlcuido mas de una vez al compilarse asi evitando 
   redefiniciones de funciones y constantes
*/
#ifndef DEFINE_BOOLEAN_H
#define DEFINE_BOOLEAN_H


/* API: verdadero */
#define TRUE 1

/* API: falso */
#define FALSE 0

/* API: Definimos boolean: Utilizar constantes TRUE y FALSE */
typedef int BOOLEAN;

#define ERROR FALSE
#define OK TRUE

#endif
