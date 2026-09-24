/*
    Ejercicio 2 

    Funcion que recibe un caracter numerico y devuelve el entero que
    representa. El caracter se valida con un AFD de dos estados antes de
    convertirlo, y la conversion se hace restando el codigo ASCII del '0'.

        digito -> [0-9]
*/

#include <stdio.h>

/* Unica columna de la tabla: la clase de los digitos decimales. */
#define COL_DIGITO  0

#define ESTADOS   2
#define COLUMNAS  1

/*
    Devuelve la columna de la tabla que le corresponde al caracter c,
    o -1 si c no pertenece al alfabeto del lenguaje.
*/
int columna(char c)
{
    if (c >= '0' && c <= '9')
        return COL_DIGITO;

    return -1;
}

/*
    AFD del ejercicio: desde el estado inicial q0 un digito lleva al estado
    final q1. Cualquier otro simbolo, o un segundo caracter, es un error.
*/
int esPalabraLeng(char c)
{
    static int tt[ESTADOS][COLUMNAS] = {
        /*            0-9  */
        /* q0  */ {    1 },
        /* q1+ */ {   -1 }
    };

    int e = 0;
    int col = columna(c);

    if (col == -1)
        return 0;

    e = tt[e][col];

    return e == 1;
}

/*
    Devuelve el valor entero del caracter numerico recibido, o -1 si el
    caracter no pertenece al lenguaje (error lexico).
*/
int caracterAEntero(char c)
{
    if (esPalabraLeng(c))
        return c - '0';

    return -1;
}

int main(void)
{
    char pruebas[] = { '0', '5', '9', 'a', '+' };
    int totalPruebas = sizeof(pruebas) / sizeof(pruebas[0]);
    int i;

    printf("PRUEBAS EJERCICIO 2\n");

    for (i = 0; i < totalPruebas; i++) {
        char c = pruebas[i];
        int res = caracterAEntero(c);

        if (res != -1)
            printf("Caracter: '%c' -> Entero retornado: %d (Exito)\n", c, res);
        else
            printf("Caracter: '%c' -> Error Lexico (No es digito)\n", c);
    }

    return 0;
}
