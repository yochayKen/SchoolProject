#include <stdio.h>
#include "list.h"
#include "utils.h"

typedef struct macro
{
    char *macro_name;
    unsigned int declaration_line;
    List *commands;
}Macro;


void start_preprocess_stage(char *orig_content)
{
    return;
}