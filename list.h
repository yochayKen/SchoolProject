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
    Node *current_element_addr;
    unsigned int length;
}List;

/* API */
List *create_list(void);
void append_to_list(List *, void *);
int remove_from_list(List *, void *, int (*func_ptr)(void *, void *));
void *search_in_list(List *, void *, int(*func_ptr)(void *, void *));
void *get_next_element(List *);
void *get_head_element(List *);
void print_list(List *, void (*func_ptr)(void *));
void delete_list(List *);

#endif