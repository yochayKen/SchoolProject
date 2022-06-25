#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../list.h"

typedef struct test{
    char *name;
    int age;
}Test;

void print_test(void *test)
{
    Test *t = (Test *)test;
    printf("My name is %s and im %d years old\n", t->name, t->age);
}

int check_test(void *data, void *test)
{
    Test *t = (Test *)test;
    Test *d = (Test *)data;
    if ((strcmp(t->name, d->name) == 0) && (t->age == d->age))
        return 0;
    return 1;
}

int main()
{
    List* test_list = NULL;

    Test *t1;
    Test *t2;
    Test *t3;
    Test *t4;

    test_list = create_list();

    t1 = (Test *)malloc(sizeof(Test));
    t2 = (Test *)malloc(sizeof(Test));
    t3 = (Test *)malloc(sizeof(Test));
    t4 = (Test *)malloc(sizeof(Test));

    t1->name = "Bob";
    t1->age = 30;

    t2->name = "Alice";
    t2->age = 20;

    t3->name = "Michael";
    t3->age = 25;

    t4->name = "Michelle";
    t4->age = 40;

    printf("===== LINKED LIST TESTS ======\n\n");

    printf("===== APPEND TEST ======\n");
    append_to_list(test_list, t1);
    append_to_list(test_list, t2);
    append_to_list(test_list, t3);
    append_to_list(test_list, t4);
    print_list(test_list, print_test);

    printf("===== REMOVE TEST ======\n");
    remove_from_list(test_list, t1 ,check_test);
    print_list(test_list, print_test);

    printf("===== REMOVE TEST ======\n");
    remove_from_list(test_list, t3 ,check_test);
    print_list(test_list, print_test);


    delete_list(test_list);
    return 0;
}