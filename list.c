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

void append_node(List *list, void *data)
{
    Node *node, *node_ptr;
    node = (Node *)malloc(sizeof(Node));
    node->data = data;
    if (list->head == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        node_ptr = list->tail;
        node_ptr->priv = list->tail->priv;
        list->tail = node;
        node_ptr->next = list->tail;
        list->tail = NULL;
    }
    list->length++;
}

void remove_node(List *list, void *data)
{
    return;
}

void delete_list(List *list)
{
    return;
}

