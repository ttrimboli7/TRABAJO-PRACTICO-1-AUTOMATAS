#include <stdio.h>
#include <ctype.h>
#include <string.h>

int columna(int e, char c)
{
    if (c == '@') {
        if (e == 1 || e == 3 || e == 4 || e == 6) return 8;
    }

    if (e == 0) {
        if (c == '0') return 0;
        if (c >= '1' && c <= '9') return 3;
        if (c == '+' || c == '-') return 2;
    }
    else if (e == 1) {
        if (c == '0' || (c >= '1' && c <= '7')) return 5;
        if (c == 'x' || c == 'X') return 6;
    }
    else if (e == 2) {
        if (c >= '1' && c <= '9') return 3;
    }
    else if (e == 3) {
        if (c >= '0' && c <= '9') return 4;
    }
    else if (e == 4) {
        if (c == '0' || (c >= '1' && c <= '7')) return 5;
    }
    else if (e == 5 || e == 6) {
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) return 7;
    }

    return -1;
}


int esPalabraLeng(const char *cadena)
{
    static const int tt[7][9] = {
       {  1, -1,  2,  3, -1, -1, -1, -1, -1 },
       { -1, -1, -1, -1, -1,  4,  5, -1,  0 },
       { -1, -1, -1,  3, -1, -1, -1, -1, -1 },
       { -1, -1, -1, -1,  3, -1, -1, -1,  0 },
       { -1, -1, -1, -1, -1,  4, -1, -1,  0 },
       { -1, -1, -1, -1, -1, -1, -1,  6, -1 },
       { -1, -1, -1, -1, -1, -1, -1,  6,  0 }
    };

    int e = 0;
    int i = 0;
    char c = cadena[0];

    while (c != '\0' && e != -1) {
        int col = columna(e, c);

        if (col == -1)
            return 0;

        e = tt[e][col];
        i++;
        c = cadena[i];
    }

    
    return (e == 1 || e == 3 || e == 4 || e == 6);
}

int verifica(const char *s)
{
    if (s == NULL || *s == '\0')
        return 0;

    for (unsigned i = 0; s[i] != '\0'; i++) {
        unsigned char c = (unsigned char)s[i];

        if (!(isdigit(c) || c == '+' || c == '-' || c == '@' ||
              c == 'x' || c == 'X' ||
              (c >= 'a' && c <= 'f') ||
              (c >= 'A' && c <= 'F')))
            return 0;
    }

    return 1;
}

/* Determina el tipo de constante individual */
const char *clasificarSegmento(const char *s)
{
    const char *p = s;

    if (*p == '+' || *p == '-')
        p++;

    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
        return "Hexadecimal";

    if (p[0] == '0' && p[1] != '\0')
        return "Octal";

    return "Decimal";
}


void clasificarCadenaCompleta(const char *cadena)
{
    char copia[256];
    strncpy(copia, cadena, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    int hex = 0, dec = 0, oct = 0;
    char *subcadena = strtok(copia, "@");

    printf("\n--- Desglose de Constantes ---\n");
    while (subcadena != NULL) {
        const char *tipo = clasificarSegmento(subcadena);
        printf("  Segmento \"%s\" -> %s\n", subcadena, tipo);

        if (strcmp(tipo, "Hexadecimal") == 0)
            hex++;
        else if (strcmp(tipo, "Octal") == 0)
            oct++;
        else
            dec++;

        subcadena = strtok(NULL, "@");
    }

    printf("\n--- Resumen ---\n");
    printf("Decimales  : %d\n", dec);
    printf("Octales    : %d\n", oct);
    printf("Hexadecimal : %d\n", hex);
}

int main(void)
{
    char palabra[256];

    printf("Introduce la cadena de constantes (separadas por @): ");
    if (scanf("%255s", palabra) != 1)
        return 1;

    if (verifica(palabra) && esPalabraLeng(palabra)) {
        printf("\nCadena VALIDA\n");
        clasificarCadenaCompleta(palabra);
    } else {
        printf("\nCadena INVALIDA segun el automata.\n");
    }

    return 0;
}
