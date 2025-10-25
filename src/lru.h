/**
 * @file lru.h
 * @brief Definiciones de estructuras y funciones para la Cache LRU (Least Recently Used).
 *
 * Contiene la logica para crear, cargar, manipular (agregar/usar), y
 * gestionar la persistencia de la cache en disco.
 */
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

    // Se asume que nodes.h declara List, Position y struct Node

/**
 * @brief Estructura principal de la cache LRU.
 *
 * Contiene la capacidad maxima y los punteros a los extremos de la lista
 * ligada que simula la cache.
 */
typedef struct {
    int max_cache;     /**< Capacidad maxima de la cache. */
    List main;         /**< Puntero al nodo cabecera de la lista ligada. */
    Position primero;  /**< Puntero al elemento mas recientemente usado (MRU). */
    Position ultimo;   /**< Puntero al elemento menos recientemente usado (LRU). */
} LRUcache;

/** @name Declaraciones de Funciones LRU */
///@{
int agregar_dato(LRUcache *cache, char val);
int usar_dato(LRUcache *cache, char val);
void mostrar_cache(LRUcache *cache);
void eliminar_menos_usado(LRUcache *cache);
int guardar_cache(LRUcache *cache);
int Exit(LRUcache *cache);
///@}

/**
 * @brief Crea un archivo vacio.
 * @param path La ruta del archivo a crear.
 * @return int Retorna 0 si es exitoso, 1 en caso de error.
 */
int CreateFile(const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL)
        return 1;
    fclose(file);
    return 0;
}
/**
 * @brief Verifica si un archivo existe.
 * @param path La ruta del archivo
 * @return int Retorna 0 no existe, 1 en caso de que si.
 */
int FileExists(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * @brief Crea un directorio (carpeta).
 * @param folder_name El nombre o ruta de la carpeta a crear.
 * @return int Retorna 0 si es exitoso, 1 en caso de error.
 */
int CreateDir(char *folder_name) {
    if(MKDIR(folder_name) != 0)
    {
        printf("No se pudo crear el directorio '%s'\n", folder_name);
        return 1;
    }
    return 0;
}

/**
 * @brief Inicializa la estructura LRUcache.
 * @param cache Puntero a la estructura LRUcache a inicializar.
 */
void init_cache(LRUcache *cache) {
    cache->max_cache = 0;
    cache->main = CreateList();
    cache->primero = NULL;
    cache->ultimo = NULL;
}
/**
 * @brief Crea la estructura de directorios y archivos de metadatos para la cache.
 * @param capacity Capacidad maxima de la cache como cadena de texto.
 * @param cache Puntero a la estructura LRUcache.
 * @return int Retorna 0 si es exitoso, 1 en caso de error.
 */
int Create_cache(char *capacity, LRUcache *cache) {
    if(capacity == NULL || atoi(capacity) <= 0) {
        printf("Capacidad no valida\n");
        return 1;
    }

    int max = atoi(capacity);
    cache->max_cache = max;

    char path_meta[256], path_data[256];
    snprintf(path_meta, sizeof(path_meta), "lru_cache%smetadata.txt", DIR_SEPARATOR);
    snprintf(path_data, sizeof(path_data), "lru_cache%sdata.txt", DIR_SEPARATOR);

    if(FileExists(path_meta) || FileExists(path_data)) {
        printf("La cache ya existe. Si desea crear una nueva, elimine la carpeta 'lru_cache' primero.\n");
        return 1;
    }
    if(CreateDir("lru_cache"))
        printf("El directorio ya existe, continuando...\n");

    FILE *meta = fopen(path_meta, "w");
    FILE *data = fopen(path_data, "w");
    if(meta == NULL || data == NULL) {
        printf("Hubo un error creando archivos\n");
        return 1;
    }

    fprintf(meta, "max_cache=%d\n", max);
    fclose(meta);
    fclose(data);

    printf("Cache creada con capacidad maxima de %d\n", max);
    return 0;
}


/**
 * @brief Inserta un dato al final de la lista. Usado solo para cargar la cache
 * desde disco y mantener el orden MRU->LRU.
 * @param cache Puntero a la estructura LRUcache.
 * @param val El dato de tipo char a insertar.
 */
void insertar_al_final(LRUcache *cache, char val) {
    Position nuevo = malloc(sizeof(struct Node));
    if (!nuevo) { 
        perror("Error de asignacion de memoria");
        exit(EXIT_FAILURE);
    }
    nuevo->Element = val;
    nuevo->Next = NULL;

    
    if (!cache->main->Next) {
        cache->main->Next = nuevo;
        cache->primero = nuevo;
        cache->ultimo = nuevo;
    } else {
        cache->ultimo->Next = nuevo;
        cache->ultimo = nuevo;
    }
}
/**
 * @brief Carga la cache desde los archivos de persistencia (metadata.txt y data.txt).
 * @param cache Puntero a la estructura LRUcache donde se cargaran los datos.
 * @return int Retorna 0 si es exitoso, 1 si la cache no existe o hay error de I/O.
 */
int LoadCache(LRUcache *cache) {
    char path_meta[256], path_data[256];
    snprintf(path_meta, sizeof(path_meta), "lru_cache%smetadata.txt", DIR_SEPARATOR);
    snprintf(path_data, sizeof(path_data), "lru_cache%sdata.txt", DIR_SEPARATOR);

    FILE *meta = fopen(path_meta, "r");
    if(!meta) {
        printf("Error al abrir '%s'\n", path_meta);
        return 1;
    }
    FILE *data = fopen(path_data, "r");
    if(!data) {
        printf("Error al abrir '%s'\n", path_data);
        fclose(meta);
        return 1;
    }
    if(fscanf(meta, "max_cache=%d\n", &cache->max_cache) != 1 || cache->max_cache <= 0) {
        printf("La memoria cache no ha sido inicializada.\n");
        fclose(meta);
        fclose(data);
        return 1;
    }
    fclose(meta);

    EmptyList(&cache->main);
    cache->main = CreateList();
    cache->primero = NULL;
    cache->ultimo = NULL;

    char line[16];
    while(fgets(line, sizeof(line), data)) {
        char c = line[0];
        if(c == '\n' || c == '\0')
            continue;
        insertar_al_final(cache, c); // Usa insercion al final para mantener el orden de carga
    }
    fclose(data);
    return 0;
}

/**
 * @brief Persiste la cache actual en disco, manteniendo el orden MRU -> LRU.
 * @param cache Puntero a la estructura LRUcache a guardar.
 * @return int Retorna 0 si es exitoso, 1 en caso de error de escritura.
 */
int guardar_cache(LRUcache *cache) {
    char path[256];
    snprintf(path, sizeof(path), "lru_cache%sdata.txt", DIR_SEPARATOR);
    FILE *data = fopen(path, "w");
    if(!data) {
        printf("Error al abrir '%s' para escritura\n", path);
        return 1;
    }

    Position p = cache->main->Next;
    while(p) {
        fputc(p->Element, data);
        fputc('\n', data);
        p = p->Next;
    }
    fclose(data);
    return 0;
}


/**
 * @brief Agrega un dato a la cache persistiendo los cambios.
 * Si el dato ya existe se marca como usado y se actualiza el archivo.
 * Si la cache esta llena, elimina el LRU antes de insertar.
 * @param cache Puntero a la estructura LRUcache.
 * @param val El dato de tipo char a agregar.
 * @return int Retorna 0 si es exitoso, 1 en caso de error.
 */
int agregar_dato(LRUcache *cache, char val) {
    if(!cache || !cache->main)
    return 1;
    
    Position encontrado = FindNode(cache->main, val);
    
    if(encontrado) {
        if(usar_dato(cache, val))
        return 1;
        if(guardar_cache(cache))
        return 1;
        return 0;
    }
    
    int count = 0;
    Position tmp = cache->main->Next;
    while(tmp) {
        count++;
        tmp = tmp->Next;
    }
    
    if(count >= cache->max_cache)
    eliminar_menos_usado(cache);
    
    Position nuevo = malloc(sizeof(struct Node));
    if (!nuevo) {
        perror("Error de asignacion de memoria");
        return 1;
    }
    
    nuevo->Element = val;
    // Insercion al inicio (nuevo MRU)
    nuevo->Next = cache->main->Next;
    cache->main->Next = nuevo;
    cache->primero = nuevo;
    
    // Si la cache estaba vacia, este es el LRU tambien
    if(!cache->ultimo)
    cache->ultimo = nuevo;
    
    if(guardar_cache(cache))
    return 1;
    
    printf("Dato %c agregado al cache\n", val);
    return 0;
}

/**
 * @brief Marca un dato existente como usado, moviendolo al frente (MRU) de la lista.
 * @param cache Puntero a la estructura LRUcache.
 * @param val El dato de tipo char a usar.
 */
int usar_dato(LRUcache *cache, char val) {
    Position prev = cache->main;
    Position actual = cache->main->Next;
    
    // Buscar el nodo y su predecesor
    while(actual && actual->Element != val) {
        prev = actual;
        actual = actual->Next;
    }
    
    if(!actual)
    {
        printf("Dato %c no encontrado en el cache\n", val);
        return 1;
    }
    
    // Si ya es el primero (MRU)
    if(prev == cache->main) {
        printf("Dato %c usado (ya es MRU)\n", val);
        return 0; 
    }
    
    // Si el nodo actual era el LRU, el predecesor es el nuevo LRU
    if (actual == cache->ultimo)
    cache->ultimo = prev;
    
    // Quitar de la posicion actual
    prev->Next = actual->Next;
    
    // Mover al frente (MRU)
    actual->Next = cache->main->Next;
    cache->main->Next = actual;
    cache->primero = actual;
    
    printf("Dato %c used\n", val);
    return 0;
}

/**
 * @brief Muestra el contenido actual de la cache en orden MRU -> LRU.
 * @param cache Puntero a la estructura LRUcache.
 */
void mostrar_cache(LRUcache *cache) {
    if(!cache->main->Next) {
        printf("Contenido del cache: (vacio)\n");
        return;
    }
    printf("Contenido del cache: ");
    Position tmp = cache->main->Next;
    while(tmp) {
        printf("%c", tmp->Element);
        if(tmp->Next) printf(" - ");
        tmp = tmp->Next;
    }
    printf("\n");
}


/**
 * @brief Elimina el elemento Menos Usado Recientemente (LRU) de la cache.
 * @param cache Puntero a la estructura LRUcache.
 */
void eliminar_menos_usado(LRUcache *cache) {
    if(!cache->main->Next)
    return;
    
    Position prev = cache->main;
    Position actual = cache->main->Next;
    
    // Encontrar el LRU (el ultimo) y su predecesor
    while(actual->Next) {
        prev = actual;
        actual = actual->Next;
    }
    printf("Dato %c eliminado\n", actual->Element);
    
    // Desvincular
    prev->Next = NULL;
    
    // Actualizar punteros de la cache
    if (prev == cache->main) {
        // La lista queda vacia
        cache->ultimo = NULL;
        cache->primero = NULL;
    } else {
        // El predecesor es el nuevo LRU
        cache->ultimo = prev;
    }
    
    free(actual);
}
/**
 * @brief Libera los recursos asociados a la cache y limpia los archivos persistidos.
 * @param cache Puntero a la estructura LRUcache que se desea limpiar.
 * @return int Retorna 0 si es exitoso, 1 si ocurre algun error.
 */
int Exit(LRUcache *cache) {
    if(!cache)
        return 1;

    char path_meta[256], path_data[256];
    snprintf(path_meta, sizeof(path_meta), "lru_cache%smetadata.txt", DIR_SEPARATOR);
    snprintf(path_data, sizeof(path_data), "lru_cache%sdata.txt", DIR_SEPARATOR);

    FILE *data = fopen(path_data, "w");
    if(!data) {
        printf("Error al limpiar '%s'\n", path_data);
        return 1;
    }
    fclose(data);

    FILE *meta = fopen(path_meta, "w");
    if(!meta) {
        printf("Error al limpiar '%s'\n", path_meta);
        return 1;
    }
    fprintf(meta, "max_cache=0\n");
    fclose(meta);

    cache->max_cache = 0;
    if(cache->main)
        EmptyList(&cache->main);
    cache->primero = NULL;
    cache->ultimo = NULL;
    return 0;
}
