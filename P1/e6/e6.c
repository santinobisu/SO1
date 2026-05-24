/*
Ej. 6. ¿Es getchar() una llamada al sistema? ¿Por qu´e s´ı o por qu´e no? ¿C´omo funciona ungetc()?
*/

// No, porque esta funcion corre completamente en el espacio de usuario, pero muchas llamadas a getchar() puede implicar una llamada a read() (syscall).
// ungetc() funciona de la siguiente forma: devuelve un carácter al stream manipulando el buffer interno, sin interactuar con el kernel.