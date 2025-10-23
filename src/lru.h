#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <direct.h>
    #define DIR_SEPARATOR "\\"
    #define MKDIR(dir) _mkdir(dir)
    #else
    #include <sys/stat.h>
    #define DIR_SEPARATOR "/"
    #define MKDIR(dir) mkdir(dir, 0777)
    #endif
    
    //por agregar: agregar_dato(), usar_dato(), mostrar_cache(), eliminar_menos_usado()
    
typedef struct {
    int max_cache;
    List main;
    Position primero;
    Position ultimo;
} LRUcache;
    
int FileExists(const char *path) { //Chequea si existe un archivo
    FILE *file = fopen(path, "r");
    if (file)
        return 1;
    return 0;
}

int CreateFile(const char *path) { //Crea un archivo
    FILE *file = fopen(path, "w");
    if (file == NULL)
        return 1;
    fclose(file);
    return 0;
}
int CreateDir(char *folder_name) {// Crea una carpeta
    if(MKDIR(folder_name) != 0)
    {
        printf("No se pudo crear el directorio '%s'\n", folder_name);
        return 1;
    }
    return 0;
}
    
void init_cache(LRUcache *cache) { //Inicializa el cache
    cache->max_cache = 0;
    cache->main = CreateList();
    cache->primero = NULL;
    cache->ultimo = NULL;  
}

int Create_cache(char* capacity) { //Crea el cache
    if(capacity == NULL || atoi(capacity) <= 0)
    {
        printf("Capacidad no valida\n");
        return 1;
    }
    int max = atoi(capacity);
    char path_met[256], path_data[256];
    snprintf(path_met, sizeof(path_met), "lru_cache%smetadata.txt", DIR_SEPARATOR);
    snprintf(path_data, sizeof(path_data), "lru_cache%sdata.txt", DIR_SEPARATOR);
    if(FileExists(path_met) || FileExists(path_data))
    {
        printf("Ya existe una memoria cache\n");
        return 1;
    }
    if(CreateDir("lru_cache"))
        return 1;
    FILE *meta = fopen(path_met, "w");
    FILE *data = fopen(path_data, "w");
    if(meta == NULL || data == NULL)
    {
        printf("Hubo un error creando archivos\n");
        return 1;
    }
    fprintf(meta, "max_cache=%d\n", max);
    fprintf(meta, "head=\n");
    fprintf(meta, "last=\n");
    fclose(meta);
    fclose(data);
    printf("Cache creada con capacidad maxima de %d\n", max);
    return 0;
}

void test(LRUcache *cache) {
    printf("Max cache: %d\n", cache->max_cache);
}

int LoadCache(LRUcache *cache) {
    char path[256];
    snprintf(path, sizeof(path), "lru_cache%smetadata.txt", DIR_SEPARATOR);
    FILE *meta = fopen(path, "r");
    if(meta == NULL)
    {
        printf("No se pudo cargar correctamente el cache, o no existe\n");
        return 1;
    }
    fscanf(meta, "max_cache=%d\n", &cache->max_cache);
    fscanf(meta, "head=%c\n", &cache->primero->Element);
    fscanf(meta, "last=%c\n", &cache->ultimo->Element);
    fclose(meta);
    path[0] = '\0';
    snprintf(path, sizeof(path), "lru_cache%sdata.txt", DIR_SEPARATOR);
    FILE *data = fopen(path, "r");
    if(meta == NULL)
    {
        printf("No se pudo cargar correctamente el cache\n");
        return 1;
    }
    while(!feof(data))
    {
        char dato = fgetc(data);
        if(dato != EOF)
            NewNode(cache->main, dato, cache->ultimo);
    }

    fclose(data);
    PrintList(cache->main);
    return 0;
}

int Add_data(char *val, LRUcache *cache) {
    if(LoadCache(cache))
        return 1;
    char path[256];
    snprintf(path, sizeof(path), "lru_cache%sdata.txt", DIR_SEPARATOR);
    FILE *data = fopen(path, "a");
    if(data == NULL)
    {
        printf("No se pudo abrir el archivo de datos\n");
        return 1;
    }
    NewNode(cache->primero, val, cache->ultimo);
    return 0;
}
