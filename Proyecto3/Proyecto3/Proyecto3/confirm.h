
#ifndef DEFINE_CONFIRM_H
#define DEFINE_CONFIRM_H

/* Estos macros facilitan la deteccion y recuperacion de errores en el programa.
   La idea no es solamente detectar y registrar sino tambien interrumpir la 
   ejecucion para evitar corromper memoria (similar al concepto de excepciones).
*/


/* Si expr no es verdadero retornar */
#define CONFIRM_RETURN(expr) \
if (!(expr)) { GlobalReportarError(__FILE__, __LINE__);  return; }

/* Si expr no es verdadero, retornar val */
#define CONFIRM_RETVAL(expr, val) \
if (!(expr)) { GlobalReportarError(__FILE__, __LINE__);  return val; }

/* Si expr no es verdadero, saltar a la etiqueta dada */
#define CONFIRM_GOTO(expr, label) \
if (!(expr)) { GlobalReportarError(__FILE__, __LINE__);  goto label; }

/* Utilizalo si solamente quieres marcar el error y continuar.. rara vez se utiliza */
#define CONFIRM_NORETURN(expr) \
if (!(expr)) { GlobalReportarError(__FILE__, __LINE__);}

/* Implementado una vez por programa para establecer como manejar errores */
extern void GlobalReportarError(char* pszFile, int  iLine);

#endif
