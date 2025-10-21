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

typedef int ElementType; //Sirve para cambiar fácilmente el tipo de dato, ayuda a entender mejor su propósito
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

struct Node {
    //(int)
    ElementType Element; //Elemento del nodo de tipo int
    Position Next;
};

void EmptyList(List *L) {
    if(L != NULL) {
        free(*L);
        *L = NULL;
    }
}

Position FindNode(List L, ElementType val) {
    Position p = L; //Se iguala al primer elemento de la lista (El de mas a la izquierda)
    while(p != NULL && p->Element != val) {
        p = p->Next;
    }
    return p;
}

void NewNode(List L, ElementType val, Position p) { //Si posición == NULL, entonces 
    //Insertar un nodo en la Lista L con valor val y posición p
    //Pasos a seguir: Encontrar la posición en la lista
    //Si no existe la posición, dar error ---> resolver la forma de encontrar esa posición
    //Crear un nodo
    Position temp = malloc(sizeof(struct Node)); //Se iguala al primer elemento de la lista (El de mas a la izquierda)
    if(temp == NULL) {
        perror("Malloc");
        exit(EXIT_FAILURE);
    }
    temp->Element = val; //Se crea el nodo con este valor

    temp->Next = p->Next; //El nodo nuevo apunta al que apuntaba el de la posición p
    p->Next = temp; //El de la posición p apunta al nodo nuevo
    
}

void DeleteNode(List L, Position p) { //Si posición == NULL, entonces 
    //Insertar un nodo en la Lista L con valor val y posición p
    //Pasos a seguir: Encontrar la posición en la lista
    //Si no existe la posición, dar error ---> resolver la forma de encontrar esa posición
    //Crear un nodo
    Position temp = malloc(sizeof(struct Node)); //Se iguala al primer elemento de la lista (El de mas a la izquierda)
    if(temp == NULL) {
        perror("Malloc");
        exit(EXIT_FAILURE);
    }

    temp->Next = p->Next; //El nodo nuevo apunta al que apuntaba el de la posición p
    p->Next = temp; //El de la posición p apunta al nodo nuevo
    
}

