/*
Ej. 14. La siguiente funci´on recorre una cadena s de longitud len y guarda en el array r cuales caracteres
aparecieron en la cadena (asuma, por un momento, que podemos usar esta construcci´on de parallel for).
¿Hay condici´on de carrera?

void charsof(char *s, int len, bool r[256])
{
    int i;
    for (i = 0; i < 256; i++)
        r[i] = false;
    parallel for (i = 0; i < len; i++)
        r[s[i]] = true;
}

Sí, hay un race condition pero no necesariamente causa un mal funcionamiento en el programa.
En resumen el race condition esta al acceder a la cadena s y el arreglo r.
*/