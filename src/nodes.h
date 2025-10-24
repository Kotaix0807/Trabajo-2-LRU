/**
 * @file nodes.h
 * @brief Implementacion de las estructuras basicas y funciones para listas ligadas simples.
 *
 * Este archivo define los tipos de datos y las operaciones fundamentales
 * para manipular los nodos que componen la estructura de la cache LRU.
 * Se utiliza un nodo cabecera (header node) para la lista.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Bloque de comentarios de nodo doblemente ligado */

/** @brief Tipo de dato para el contenido del nodo. */
typedef char ElementType;
/** @brief Puntero a la estructura Node. */
typedef struct Node *PtrToNode;

/** @brief Tipo de dato para la lista (puntero al nodo cabecera). */
typedef PtrToNode List;
/** @brief Tipo de dato para una posicion dentro de la lista (puntero a un nodo). */
typedef PtrToNode Position;

/**
 * @brief Estructura de un nodo de la lista ligada simple.
 */
struct Node {
    ElementType Element; /**< Contenido del nodo (el dato de la cache). */
    Position Next;       /**< Puntero al nodo siguiente. */
};

/** @name Declaraciones de Funciones de Lista */
///@{
Position FindNode(List L, ElementType val);
Position PreviusNode(List L, Position p);
void NewNode(List L, ElementType val, Position p);
void DeleteNode(List L, Position p);
int IsLast(Position p);
void EmptyList(List *L);
List CreateList(void);
///@}


/** @name Funciones de Manipulacion de Nodos */
///@{

/**
 * @brief Crea e inicializa una lista ligada (con nodo cabecera).
 * @return List Retorna un puntero al nodo cabecera de la nueva lista.
 */
List CreateList(void) {
    List L = malloc(sizeof(struct Node));
    if (L == NULL) {
        perror("Error creando la lista");
        exit(EXIT_FAILURE);
    }
    L->Next = NULL;
    L->Element = 0; // El nodo cabecera no almacena un elemento util.
    return L;
}

/**
 * @brief Vacia completamente la lista ligada, liberando la memoria de todos los nodos.
 * @param L Puntero a la lista a vaciar (puntero doble para modificar la List misma).
 */
void EmptyList(List *L) {
    if (L != NULL && *L != NULL) {
        Position current = (*L)->Next;
        Position temp;
        while (current != NULL) {
            temp = current;
            current = current->Next;
            free(temp);
        }
        free(*L); // Liberar tambien el nodo cabecera
        *L = NULL;
    }
}

/**
 * @brief Verifica si una posicion dada es el ultimo nodo de la lista (no nulo).
 * @param p Posicion (nodo) a verificar.
 * @return int Retorna 1 si es el ultimo nodo, 0 en caso contrario.
 */
int IsLast(Position p) {
    return p != NULL && p->Next == NULL;
}

/**
 * @brief Elimina un nodo especifico de la lista.
 * @note Asume que el nodo a borrar no es el nodo cabecera.
 * @param L Puntero al nodo cabecera de la lista.
 * @param p Posicion (nodo) a borrar.
 */
void DeleteNode(List L, Position p) {
    if (L == NULL || p == NULL)
        return;

    Position prev = PreviusNode(L, p);
    if (prev != NULL) {
        Position temp = p;
        prev->Next = p->Next;
        free(temp);
    }
}

/**
 * @brief Inserta un nuevo nodo despues de una posicion especifica.
 * @note La insercion en la cabeza se logra pasando el nodo cabecera como 'p'.
 * @param L Puntero al nodo cabecera de la lista.
 * @param val El valor del elemento a insertar.
 * @param p Posicion despues de la cual se insertara el nuevo nodo.
 */
void NewNode(List L, ElementType val, Position p) {
    if (L == NULL || p == NULL)
        return;
    Position temp = malloc(sizeof(struct Node));
    if (temp == NULL) {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    temp->Element = val;
    temp->Next = p->Next;
    p->Next = temp;
}

/**
 * @brief Encuentra la posicion del nodo predecesor (anterior) a un nodo dado.
 * @param L Puntero al nodo cabecera de la lista.
 * @param p Posicion (nodo) cuyo predecesor se busca.
 * @return Position Retorna el puntero al nodo predecesor, o NULL si 'p' es la cabecera o no esta en la lista.
 */
Position PreviusNode(List L, Position p) {
    if (L == NULL || p == NULL) return NULL;

    Position current = L;
    if (current == p) {
        return NULL; // 'p' es el nodo cabecera
    }
    while (current != NULL && current->Next != p) {
        current = current->Next;
    }
    return current;
}

/**
 * @brief Busca un nodo que contenga un valor especifico.
 * @param L Puntero al nodo cabecera de la lista.
 * @param val El valor del elemento a buscar.
 * @return Position Retorna un puntero al primer nodo que contenga el valor, o NULL si no se encuentra.
 */
Position FindNode(List L, ElementType val) {
    if (L == NULL)
        return NULL;

    Position p = L->Next; // Empezar despues del nodo cabecera
    while (p != NULL && p->Element != val) {
        p = p->Next;
    }
    return p;
}

/**
 * @brief Imprime el contenido de la lista (util para debug o el comando 'all').
 * @param L Puntero al nodo cabecera de la lista.
 */
void PrintList(List L) {
    if (L == NULL || L->Next == NULL) {
        printf("(vacia)\n");
        return;
    }
    Position p = L->Next; // Saltamos nodo cabecera
    printf("Cache (mas reciente -> menos reciente): ");
    while (p != NULL) {
        printf("%c ", p->Element);
        p = p->Next;
    }
    printf("\n");
}
///@}

