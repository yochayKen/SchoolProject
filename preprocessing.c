#include <stdio.h>
#include "list.h"
#include "utils.h"

typedef struct macro
{
    char *macro_name;
    unsigned int declaration_line;
    List *commands;
}Macro;


void preprocess_asm_code(char *orig_assmebler_code)
{
    List *macro_list = create_list();
    List *code_list = convert_string_to_list(orig_assmebler_code, '\n');


    return;
}