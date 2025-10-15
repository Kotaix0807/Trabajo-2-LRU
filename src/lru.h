#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//por agregar: agregar_dato(), usar_dato(), mostrar_cache(), eliminar_menos_usado().
//Compatibilidad con windows (crear carpeta y archivo)

typedef struct {
    int max_cache;
    int elementos_actuales;
    struct Node *primero;
    struct Node *ultimo;

} LRUcache;

int binarySearch(int arr[], int n, int x)
{
    int low = 0;
    int high = n - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        // Check if x is present at mid
        if (arr[mid] == x)
            return mid;

        // If x greater, ignore left half
        if (arr[mid] < x)
            low = mid + 1;

            // If x is smaller, ignore right half
            else
            high = mid - 1;
        }
        // If we reach here, then element was not present
        return -1;
    }
    
    int CreateDir(char *folder_name) // Crea una carpeta
    {
    size_t cmd_size = strlen(folder_name) + 20;
    char *cmd = malloc(sizeof(char) * cmd_size);
    snprintf(cmd, cmd_size, "mkdir \"%s\"", folder_name);
    if(system(cmd) != 0)
    {
        printf("No se pudo crear el directorio '%s'\n", folder_name);
        free(cmd);
        return 1;
    }
    free(cmd);
    return 0;
}

int CreateFile(char *file_name) // Crea un archivo
{
    FILE *file = fopen(file_name, "w");
    if (file == NULL)
    {
        printf("No se pudo crear el archivo '%s'\n", file_name);
        return 1;
    }
    fclose(file);
    return 0;
}

void init_cache(LRUcache *cache) // Inicializa la cache
{
    cache->max_cache = 0;
    cache->elementos_actuales = 0;
    cache->primero = NULL;
    cache->ultimo = NULL;  
}

int create_cache(char* capacity, LRUcache* cache) // Crea la cache
{
    if(capacity == NULL || atoi(capacity) <= 5)
    {
        printf("Capacidad no valida\n");
        return 1;
    }
    cache->max_cache = atoi(capacity);
    printf("Cache creada con capacidad maxima de %d\n", cache->max_cache);
    if(CreateDir(".lru_cache"))
    return 1;
    FILE *meta = fopen(".lru_cache/metadata.txt", "w");
    if(meta == NULL)
    {
        printf("No se pudo abrir el archivo de metadata\n");
        return 1;
    }
    fprintf(meta, "max_cache = %d\n", cache->max_cache);
    fclose(meta);
    return 0;
}
int add_data(char data, LRUcache *cache)
{
    if(cache == NULL)
    {
        printf("ERROR: cache no creado\n");
        return 1;
    }
    return 0;
}