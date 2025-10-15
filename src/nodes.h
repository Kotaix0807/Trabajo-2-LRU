#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
}

