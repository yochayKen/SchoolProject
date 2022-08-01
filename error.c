#include <stdio.h>
#include "error.h"

char *current_file_name;

typedef struct error{
    ErrorValue ev;
    unsigned int line_number;
    char *error_description;
}Error;

static const Error error_table[] = {
    { INVALID_MACRO_DECLERATION, 0, "Macro should be defined before use" },
};

void declare_an_error(ErrorValue error_value, unsigned int line_number)
{
    int i;
    for (i = 0; i < sizeof(error_table)/sizeof(Error); i++)
    {
        if (error_table[i].ev == error_value)
        {
            printf("File name: %s => Error in line [%u]: %s\n", current_file_name,  line_number, error_table[i].error_description);
        }
    }
}

void update_current_file_name(char *file_name)
{
    current_file_name = file_name;
}