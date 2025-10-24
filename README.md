# Trabajo-2-LRU
Trabajo 2 Estructura de datos
Para compilar el programa:
gcc main.c -o lru -Wall o tambien se puede usar makefile (make)

Para ejecutar el programa:
./lru con sus respectivos argumentos

./lru create <max_capacity>   Crear memoria cache con tamaño máximo.
./lru add <char>              Agregar un dato a la cache.
./lru all                     Mostrar todos los datos en cache.
./lru get <char>              Marcar dato como usado (mover al frente).
./lru exit                    Salir del programa.

En caso de posible error eliminar la carpeta lru_cache que se crea en el mismo directorio del proyecto, esto se puede hacer 
con el comando: rm -rf lru_cache

Toda la documentacion fue hecha en formato Doxygen, para compilarla se debe ejecutar el comando:  doxygen Doxyfile o acceder a la carpeta html y abrir el archivo index.html