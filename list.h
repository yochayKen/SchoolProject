#ifndef _LIST_H_
#define _LIST_H_

/* Types*/
typedef struct node{
    void *data;
    struct node *next;
}Node;
typedef struct list{
    Node *head;
    Node *tail;
    unsigned int length;
}List;

/* API */
List *create_list(void);
void append_to_list(List *, void *);
void remove_from_list(List *, void *, int (*func_ptr)(void *, void *));
void print_list(List *, void (*func_ptr)(void *));
void delete_list(List *);

#endif