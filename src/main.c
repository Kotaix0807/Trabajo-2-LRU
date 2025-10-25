/**
 * @file main.c
 * @brief Archivo principal del programa de simulacion de cache LRU.
 *
 * Implementa la interfaz de linea de comandos para interactuar con la
 * estructura de cache Least Recently Used (LRU).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodes.h"
#include "lru.h"

/**
 * @brief Funcion principal del programa.
 *
 * Procesa los argumentos de la linea de comandos para realizar operaciones
 * sobre la cache LRU.
 *
 * @param argc Numero de argumentos de la linea de comandos.
 * @param argv Arreglo de cadenas de caracteres con los argumentos.
 * @return int Retorna 0 si la ejecucion es exitosa, 1 en caso de error.
 */
int main(int argc, char *argv[])
{
    LRUcache main_cache;
    init_cache(&main_cache);
    
    if(argc < 2)
    {
        printf("No arguments provided.\nTry: '[--help] [-h]'\n");
        return 1;
    }

    /// Mostrar ayuda
    if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Comandos disponibles: utilizando ./lru antes del argumento:\n");
        printf("create <max_capacity>   -> Crear memoria cache con tamano maximo.\n");
        printf("add <char>             -> Agregar un dato a la cache.\n");
        printf("all                    -> Mostrar todos los datos en cache.\n");
        printf("get <char>             -> Marcar dato como usado (mover al frente).\n");
        printf("exit                   -> Salir del programa.\n");
        return 0;
    }

    /// Comando create: Inicializa una nueva cache con la capacidad especificada.
    if(strcmp(argv[1], "create") == 0)
    {
        if(argc < 3)
        {
            printf("Debe indicar la capacidad maxima.\n");
            return 1;
        }
        if(Create_cache(argv[2], &main_cache))
            return 1;
        return 0;
    }

    /// Para cualquier otro comando, intenta cargar la cache existente desde disco
    if(LoadCache(&main_cache))
    {
        printf("No se pudo cargar la memoria cache. Asegurese de crearla primero.\n");
        return 1;
    }

    /// Comando add: Agrega un nuevo dato a la cache (o lo usa si ya existe).
    if(strcmp(argv[1], "add") == 0)
    {
        if(argc < 3)
        {
            printf("Debe indicar el dato a agregar.\n");
            return 1;
        }
        if(agregar_dato(&main_cache, argv[2][0]))
            return 1;
        mostrar_cache(&main_cache);
        return 0;
    }
    /// Comando get: Marca un dato existente como usado (lo mueve a la posicion MRU).
    else if(strcmp(argv[1], "get") == 0)
    {
        if(argc < 3)
        {
            printf("Debe indicar el dato a usar.\n");
            return 1;
        }
        if(usar_dato(&main_cache, argv[2][0]))
            return 1;
        if(guardar_cache(&main_cache))
            return 1;
        mostrar_cache(&main_cache);
        return 0;
    }
    /// Comando all: Muestra el contenido actual de la cache.
    else if(strcmp(argv[1], "all") == 0)
    {
        mostrar_cache(&main_cache);
        return 0;
    }
    /// Comando exit: Termina la ejecucion del programa.
    else if(strcmp(argv[1], "exit") == 0)
    {
        printf("Borrando cache y saliendo...\n");
        if(Exit(&main_cache))
            return 1;
        return 0;
    }
    /// Comando desconocido.
    else
    {
        printf("Comando desconocido. Use --help para ver los comandos disponibles.\n");
        return 0;
    }

    return 0;
}
