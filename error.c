#include <stdio.h>
#include "utils.h"
#include "error.h"

static char *current_file_name;
static Bool error_flag = FALSE;
static ErrorValue error_value = NONE;

typedef struct error{
    ErrorValue ev;
    char *error_description;
}Error;

static const Error error_table[] = {
    {INVALID_MACRO_DECLERATION, "Macro should be defined before use" },
    {INVALID_SYMBOL_DECLERATION, "Invalid symbol decleration" },
    {SYMBOL_ALREADY_DECLARED, "Symbol has been already declared"},
    {UNKNOWN_FILE_EXTENSION, "Unknown file extension. Should end with '.as'"},
    {UNKNOWN_DECLARED_STR, "Unknown word has been deteced"},
    {DATA_STRUCT_DECLARED_WITH_NO_NUMBER, "Data structure \".data\" has been daclared with invalid argument"},
    {UNKNOWN_DATA_STRUCT_TYPE, "Could not identify the data struct"},
    {ERROR_IN_STRING_DECLARATION, "String declaration missing quotation mark"},
    {ERROR_IN_STRUCT_DECLARATION, "Struct declaration is invalid"},
    {COULD_NOT_FIND_INSTRUCTION, "Could not find instruction name"},
    {FOUND_ARGS_FOR_NONE_INST_ARG, "The instruction should not get arguments"},
    {UNSUPPORTED_ADDR_METHOD, "Unsupported addressing method"},
    {NO_COMMA_FOUND, "No comma found in data structure"}
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

Bool get_error()
{
    return error_flag;
}

void set_error_type(ErrorValue ev)
{
    error_flag = TRUE;
    error_value = ev;
}

ErrorValue get_error_type()
{
    return error_value;
}

void update_current_file_name(char *file_name)
{
    current_file_name = file_name;
}