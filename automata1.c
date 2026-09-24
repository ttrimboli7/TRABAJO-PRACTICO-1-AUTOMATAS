/*
    Ejercicio 1 

    Reconoce una cadena formada por constantes enteras decimales, octales y
    hexadecimales separadas por el caracter '@'. Informa si hay error lexico
    (indicando la posicion) y, si la cadena es valida, cuenta cuantas
    constantes hay de cada grupo.

        decimal      -> [+|-]? ( 0 | [1-9][0-9]* )
        octal        -> 0 [0-7]+
        hexadecimal  -> 0 (x|X) [0-9|a-f|A-F]+
        cadena       -> constante ( @ constante )*
*/

#include <stdio.h>
#include <string.h>

/* Columnas de la tabla de transiciones: particion del alfabeto en clases. */
#define COL_CERO    0   /* 0                 */
#define COL_1A7     1   /* 1 2 3 4 5 6 7     */
#define COL_8A9     2   /* 8 9               */
#define COL_SIGNO   3   /* + -               */
#define COL_EQUIS   4   /* x X               */
#define COL_AF      5   /* a..f  A..F        */
#define COL_ARROBA  6   /* @                 */

#define ESTADOS   8
#define COLUMNAS  7

/* Los tres grupos de constantes enteras a reconocer. */
#define DECIMAL      0
#define OCTAL        1
#define HEXADECIMAL  2

/*
    Devuelve la columna de la tabla que le corresponde al caracter c,
    o -1 si c no pertenece al alfabeto del lenguaje.
*/
int columna(char c)
{
    if (c == '0')
        return COL_CERO;

    if (c >= '1' && c <= '7')
        return COL_1A7;

    if (c == '8' || c == '9')
        return COL_8A9;

    if (c == '+' || c == '-')
        return COL_SIGNO;

    if (c == 'x' || c == 'X')
        return COL_EQUIS;

    if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
        return COL_AF;

    if (c == '@')
        return COL_ARROBA;

    return -1;
}

/*
    Cada estado final identifica por si mismo al grupo de la constante que
    se acaba de leer, asi que no hace falta volver a recorrerla.

        q3 -> el cero sin signo      (decimal)
        q4 -> decimal con digitos    (decimal)
        q6 -> el cero con signo      (decimal)
        q5 -> octal
        q7 -> hexadecimal

    Devuelve -1 si el estado no es final.
*/
int grupo(int e)
{
    if (e == 3 || e == 4 || e == 6)
        return DECIMAL;

    if (e == 5)
        return OCTAL;

    if (e == 7)
        return HEXADECIMAL;

    return -1;
}

const char *nombreGrupo(int g)
{
    if (g == DECIMAL)
        return "DECIMAL";

    if (g == OCTAL)
        return "OCTAL";

    return "HEXADECIMAL";
}

/*
    Recorre la cadena con el AFD. Muestra cada constante reconocida con su
    grupo, informa el primer error lexico si lo hay y, cuando la cadena es
    valida, imprime la cantidad de constantes de cada grupo.

    Devuelve 1 si la cadena pertenece al lenguaje y 0 si no.
*/
int analizarCadena(const char *cadena)
{
    static int tt[ESTADOS][COLUMNAS] = {
        /*            0    1-7   8-9   + -   x X   a-f    @    */
        /* q0  */ {   3,    4,    4,    1,   -1,   -1,   -1 },
        /* q1  */ {   6,    4,    4,   -1,   -1,   -1,   -1 },
        /* q2  */ {   7,    7,    7,   -1,   -1,    7,   -1 },
        /* q3+ */ {   5,    5,   -1,   -1,    2,   -1,    0 },
        /* q4+ */ {   4,    4,    4,   -1,   -1,   -1,    0 },
        /* q5+ */ {   5,    5,   -1,   -1,   -1,   -1,    0 },
        /* q6+ */ {  -1,   -1,   -1,   -1,   -1,   -1,    0 },
        /* q7+ */ {   7,    7,    7,   -1,   -1,    7,    0 }
    };

    int cuenta[3] = { 0, 0, 0 };
    int e = 0;
    int i = 0;
    int inicio = 0;   /* posicion donde arranca la constante en curso */
    int listado = 0;  /* 1 cuando ya se imprimio el encabezado del listado */
    int g;

    while (cadena[i] != '\0') {
        int col = columna(cadena[i]);

        if (col == -1) {
            printf("\nERROR LEXICO en la posicion %d: el simbolo '%c' no pertenece al alfabeto.\n",
                   i + 1, cadena[i]);
            return 0;
        }

        if (tt[e][col] == -1) {
            printf("\nERROR LEXICO en la posicion %d: el simbolo '%c' no puede aparecer en ese lugar.\n",
                   i + 1, cadena[i]);
            return 0;
        }

        /* Un '@' solo se acepta desde un estado final: ahi cierra una constante. */
        if (col == COL_ARROBA) {
            g = grupo(e);
            cuenta[g]++;

            if (!listado) {
                printf("\nConstantes reconocidas:\n");
                listado = 1;
            }

            printf("  %-14.*s -> %s\n", i - inicio, cadena + inicio, nombreGrupo(g));
            inicio = i + 1;
        }

        e = tt[e][col];
        i++;
    }

    g = grupo(e);

    if (g == -1) {
        printf("\nERROR LEXICO: la cadena termina de forma incompleta en la posicion %d.\n", i);
        return 0;
    }

    cuenta[g]++;

    if (!listado)
        printf("\nConstantes reconocidas:\n");

    printf("  %-14.*s -> %s\n", i - inicio, cadena + inicio, nombreGrupo(g));

    printf("\nCadena valida: no se detectaron errores lexicos.\n");
    printf("Cantidad de constantes por grupo:\n");
    printf("  Decimales     : %d\n", cuenta[DECIMAL]);
    printf("  Octales       : %d\n", cuenta[OCTAL]);
    printf("  Hexadecimales : %d\n", cuenta[HEXADECIMAL]);
    printf("  Total         : %d\n", cuenta[DECIMAL] + cuenta[OCTAL] + cuenta[HEXADECIMAL]);

    return 1;
}

int main(void)
{
    char cadena[256];

    printf("Introduce una cadena (constantes separadas por '@'): ");

    if (fgets(cadena, sizeof(cadena), stdin) == NULL)
        return 1;

    cadena[strcspn(cadena, "\r\n")] = '\0';

    if (cadena[0] == '\0') {
        printf("\nERROR LEXICO: la cadena esta vacia, no pertenece al lenguaje.\n");
        return 0;
    }

    analizarCadena(cadena);

    return 0;
}
