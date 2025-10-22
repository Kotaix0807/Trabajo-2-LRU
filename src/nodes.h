#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
typedef struct Node {
    char data;
    struct Node* prev;
    struct Node* next;
} Node;

//crear nodo
Node* create_node(char data)
{
    Node* nuevo_nodo = malloc(sizeof(Node));
    if (!nuevo_nodo)
    {
        printf("Error, no se pudo asignar memoria para el nuevo nodo\n");
        return NULL;
    }
    
    nuevo_nodo -> data = data;
    nuevo_nodo -> prev = NULL;
    nuevo_nodo -> next = NULL;
    
    return nuevo_nodo;
}*/

typedef char ElementType;
typedef struct Node *PtrToNode;
/*
    Entender esto como un arreglo dinámico de nodos.
*/
typedef PtrToNode List; //Arreglo dinámico de nodos
typedef PtrToNode Position; //Arreglo dinámico de nodos

struct Node {
    ElementType Element; //Contenido del nodo
    Position Next; //Puntero al nodo que le sigue
};

//Declaraciones de funciones.
Position FindNode(List L, ElementType val);
Position PreviusNode(List L, Position p);
void NewNode(List L, ElementType val, Position p);
void DeleteNode(List L, Position p);
int IsLast(Position p);
void EmptyList(List *L);
List CreateList(void);


//Funciones de manipulación de nodos
List CreateList(void) { //Crear lista
    List L = malloc(sizeof(struct Node));
    if (L == NULL) {
        perror("Error creando la lista");
        exit(EXIT_FAILURE);
    }
    L->Next = NULL;
    L->Element = 0;
    return L;
}

void EmptyList(List *L) { //Vaciar la lista
    if (L != NULL && *L != NULL) {
        Position current = (*L)->Next;
        Position temp;
        while (current != NULL) {
            temp = current;
            current = current->Next;
            free(temp);
        }
        free(*L);
        *L = NULL;
    }
}

int IsLast(Position p) { //Verificar si es el último
    return p != NULL && p->Next == NULL;
}

void DeleteNode(List L, Position p) { //Borrar nodo
    if (L == NULL || p == NULL)
        return;
    
    Position prev = PreviusNode(L, p);
    if (prev != NULL) {
        Position temp = p;
        prev->Next = p->Next;
        free(temp);
    }
}

void NewNode(List L, ElementType val, Position p) { //Crear nodo nuevo
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

Position PreviusNode(List L, Position p) { //Encontrar nodo anterior
    if (L == NULL || p == NULL) return NULL;
    
    Position current = L;
    if (current == p) {
        return NULL;
    }
    while (current != NULL && current->Next != p) {
        current = current->Next;
    }
    return current;
}

Position FindNode(List L, ElementType val) { //Encontrar nodo
    if (L == NULL)
        return NULL;
    
    Position p = L->Next;
    while (p != NULL && p->Element != val) {
        p = p->Next;
    }
    return p;
}

