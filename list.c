#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*Create a generic linked list. Return a List object.*/
List *create_list()
{
    List *list;
    list = (List *)malloc(sizeof(List));
    list->head = list->tail = NULL;
    list->length = 0;
    return list;
}

/*Append object to the end of the list*/
void append_node(List *list, void *data)
{
    Node *node;
    node = (Node *)malloc(sizeof(Node));

    node->data = data;
    node->next = NULL;

    if (list->head == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail = node;
    }
    list->length++;
}

/*Remove object from the list. Note - comparing addresses of the objects*/
void remove_node(List *list, void *data)
{
    Node *nptr = list->head;
    Node *node;
    while (nptr != NULL)
    {
        if (nptr->next->data == data)
        {
            node = nptr->next;
            nptr->next = node->next;
            free(node);
            list->length--;
            break;
        }
    }
}

/*Remove the hole list*/
void delete_list(List *list)
{
    Node *current = list->head;
    Node *next;
    while(current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}
