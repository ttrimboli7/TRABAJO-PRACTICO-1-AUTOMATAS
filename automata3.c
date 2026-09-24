/*
    Ejercicio 3 

    Valida con un AFD que la cadena ingresada sea una operacion entera con
    los operadores +, - y *, y recien despues la resuelve respetando la
    precedencia del producto sobre la suma y la resta.

        numero    -> 0 | [1-9][0-9]*
        operacion -> numero ( (+|-|*) numero )*
*/

#include <stdio.h>
#include <string.h>

/* Columnas de la tabla de transiciones: particion del alfabeto en clases. */
#define COL_CERO      0   /* 0        */
#define COL_1A9       1   /* 1 .. 9   */
#define COL_OPERADOR  2   /* + - *    */

#define ESTADOS   4
#define COLUMNAS  3

/*
     convierte un caracter numerico en su valor
    entero, o -1 si no es un digito. (aca reutilice del 2)
*/
int caracterAEntero(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    return -1;
}

/*
    Devuelve la columna de la tabla que le corresponde al caracter c,
    o -1 si c no pertenece al alfabeto del lenguaje.
*/
int columna(char c)
{
    if (c == '0')
        return COL_CERO;

    if (c >= '1' && c <= '9')
        return COL_1A9;

    if (c == '+' || c == '-' || c == '*')
        return COL_OPERADOR;

    return -1;
}

/*
    Recorre la cadena con el AFD e informa el primer error lexico con su
    posicion. Devuelve 1 si la cadena pertenece al lenguaje y 0 si no.
*/
int esPalabraLeng(const char *cadena)
{
    static int tt[ESTADOS][COLUMNAS] = {
        /*             0    1-9   + - *  */
        /* q0  */ {    1,    2,    -1 },
        /* q1+ */ {   -1,   -1,     3 },
        /* q2+ */ {    2,    2,     3 },
        /* q3  */ {    1,    2,    -1 }
    };

    int e = 0;
    int i = 0;

    while (cadena[i] != '\0') {
        int col = columna(cadena[i]);

        if (col == -1) {
            printf("ERROR LEXICO en la posicion %d: el simbolo '%c' no pertenece al alfabeto.\n",
                   i + 1, cadena[i]);
            return 0;
        }

        if (tt[e][col] == -1) {
            printf("ERROR LEXICO en la posicion %d: el simbolo '%c' no puede aparecer en ese lugar.\n",
                   i + 1, cadena[i]);
            return 0;
        }

        e = tt[e][col];
        i++;
    }

    if (e != 1 && e != 2) {
        printf("ERROR LEXICO: la operacion termina de forma incompleta en la posicion %d.\n", i);
        return 0;
    }

    return 1;
}

/*
    Resuelve la operacion en una sola pasada.

    Se acumula el resultado en 'resultado' y se recuerda en 'ultimoTermino'
    cuanto aporto el ultimo numero procesado. Con + y - el numero se suma o
    se resta directamente. Con * hay que corregir: se le quita al resultado
    el aporte del ultimo termino, se lo multiplica por el numero nuevo y se
    vuelve a sumar el termino ya corregido. De esa forma el producto se
    resuelve antes que la suma y la resta sin necesidad de una segunda
    pasada ni de una pila.

    Ejemplo con 3+4*7:  resultado=3, ultimo=3 -> resultado=7, ultimo=4
                        -> resultado=7-4=3, ultimo=4*7=28, resultado=31.
*/
int evaluarExpresion(const char *cadena)
{
    int resultado = 0;
    int numero = 0;
    int ultimoTermino = 0;
    char operador = '+';
    int i = 0;

    while (cadena[i] != '\0') {
        numero = 0;

        while (caracterAEntero(cadena[i]) != -1) {
            numero = numero * 10 + caracterAEntero(cadena[i]);
            i++;
        }

        if (operador == '+') {
            resultado += numero;
            ultimoTermino = numero;
        }
        else if (operador == '-') {
            resultado -= numero;
            ultimoTermino = -numero;
        }
        else if (operador == '*') {
            resultado -= ultimoTermino;
            ultimoTermino *= numero;
            resultado += ultimoTermino;
        }

        if (cadena[i] != '\0') {
            operador = cadena[i];
            i++;
        }
    }

    return resultado;
}

int main(void)
{
    char cadena[256];

    printf("Introduce una operacion simple: ");

    if (fgets(cadena, sizeof(cadena), stdin) == NULL)
        return 1;

    cadena[strcspn(cadena, "\r\n")] = '\0';

    if (cadena[0] == '\0') {
        printf("ERROR LEXICO: la cadena esta vacia, no pertenece al lenguaje.\n");
        return 0;
    }

    if (esPalabraLeng(cadena)) {
        printf("Cadena valida: pertenece al lenguaje.\n");
        printf("%s = %d\n", cadena, evaluarExpresion(cadena));
    }

    return 0;
}
