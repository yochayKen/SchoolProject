#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "utils.h"

/*Create a generic linked list. Return a List reference.*/
List *create_list()
{
    List *list;
    list = (List *)malloc(sizeof(List));
    list->head = list->tail = list->current_element_addr = NULL;
    list->length = 0;
    return list;
}

/*Append 'data' type to the end of the list*/
void append_to_list(List *list, void *data)
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
        list->tail->next = node;
        list->tail = node;
    }
    list->length++;
}

/*
Remove 'data' type from the list. If 'data' type does not exists,
return 1. The function argument, MUST return int
(0 or 1) and will except 2 arguments of (void *) type.
*/
int remove_from_list(List *list, void *data, int(* func_ptr)(void *, void *))
{
    Node *nptr = list->head;
    Node *priv_node;
    while (nptr != NULL)
    {
        if ((*func_ptr)(data, nptr->data) == 0)
        {
            if (nptr == list->head)
            {
                list->head = list->head->next;
            }
            else
            {
                priv_node->next = nptr->next;
            }
            free(nptr);
            list->length--;
            return 0;
        }
        else
        {
            priv_node = nptr;
            nptr = nptr->next;
        }
    }
    return 1;
}

/*
Search 'data' type in a given list. If 'data' type does not exists,
return NULL. The function argument, MUST return int
(0 or 1) and will except 2 arguments of (void *) type.
*/
void *search_in_list(List *list, void *data, int(* func_ptr)(void *, void *))
{
    Node *nptr = list->head;
    void *result = NULL;
    while (nptr != NULL)
    {
        if ((*func_ptr)(nptr->data, data) == 0)
        {
            result = nptr->data;
            break;
        }
        else
        {
            nptr = nptr->next;
        }
    }
    return result;
}

void *get_head_element(List *list)
{
    list->current_element_addr = list->head;
    return list->current_element_addr->data;
}

void *get_next_element(List *list)
{
    list->current_element_addr = list->current_element_addr->next;
    if (list->current_element_addr == NULL)
    {
        list->current_element_addr = list->head;
        return NULL;
    }
    return list->current_element_addr->data;
}

Node *get_current_element_addr(List *list)
{
    return list->current_element_addr;
}

void *check_next_element_in_list(List *list)
{
    Node *nptr = list->current_element_addr;
    if (nptr->next == NULL)
        return NULL;
    return nptr->next->data;
}

void insert_sublist_in_list(Node *current_node, List *sub_list)
{
    Node *nptr = current_node;
    Node *tmp = current_node->next->next;
    nptr->next = sub_list->head;
    sub_list->tail->next = tmp;
}

int count_list(List *list)
{
    Node *nptr = list->head;
    int num_of_lines = 0;

    while (nptr != NULL)
    {
        num_of_lines++;
        nptr = nptr->next;
    }
    return num_of_lines;
}

Bool is_list_empty(List *list)
{
    if (list->head == NULL)
        return TRUE;
    return FALSE;
}

/*
Printing list elements. The function argument,
MUST return void and will except 1 (void *) argument.
 */
void print_list(List *list, void(* print_func_ptr)(void *))
{
    Node *nptr = list->head;
    while(nptr != NULL)
    {
        (*print_func_ptr)(nptr->data);
        nptr = nptr->next;
    }
}

/*Removing hole list*/
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
