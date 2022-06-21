#ifndef _LIST_H_
#define _LIST_H_

typedef struct list{
    void *head;
    void *tail;
    unsigned int length;
}List;

#endif