#include <stdio.h>
#include "nodes.h"
#include "lru.h"
int main(int argc, char *argv[])
{
    LRUcache main_cache;
    List main_list = CreateList();
    init_cache(&main_cache);
    if(argc < 2)
    {
        printf("No arguments in\nTry: '[--help] [-h]'\n");
        return 1;
    }
    else if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Comandos disponibles:\n");
        printf("<--help> <-h> ----> Muestra esta ayuda.\n");
        printf("<create> <max_capacity> ----> Crear memoria cache con su tamaño maximo.\n");
    }
    else if(strcmp(argv[1], "create") == 0)
    {
        if(Create_cache(argv[2]))
            return 1;
    }
    else if(strcmp(argv[1], "create") == 0)
    {
        if(Add_data(argv[2], &main_cache))
            return 1;
    }    
    else if(strcmp(argv[1], "test") == 0)
    {
        test(&main_cache);
    }
    return 0;
}