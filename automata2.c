#include <stdio.h>

int caracterAEntero(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    
    return -1;
}

int main(void)
{
    // Mezclamos algunos números, letras y símbolos para probar la función
    char pruebas[] = {'3', '0', '7', 'a', 'F', '+', '9', 'x', '2', '$'};
    int total = sizeof(pruebas) / sizeof(pruebas[0]);

    printf(" Test de la funcion caracterAEntero \n");

    for (int i = 0; i < total; i++)
    {
        char c = pruebas[i];
        int res = caracterAEntero(c);

        if (res != -1)
        {
            printf("'%c' -> Es digito, valor: %d\n", c, res);
        }
        else
        {
            printf("'%c' -> No es un digito valido (-1)\n", c);
        }
    }

    return 0;
}