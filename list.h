#ifndef _LIST_H_
#define _LIST_H_

typedef struct node{
    void *data;
    struct node *next;
    struct node *priv;
}Node;
typedef struct list{
    Node *head;
    Node *tail;
    unsigned int length;
}List;

/* API */
List *create_list(void);
void append_node(List *, void *);
void remove_node(List *, void *);
void delete_list(List *);

#endif