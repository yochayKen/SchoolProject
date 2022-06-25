#include <stdio.h>
#include "list.h"

typedef struct macro
{
    char *macro_name;
    unsigned int declaration_line;
    List *commands;
}Macro;


void preprocess_asm_content(char *content)
{
    return;
}