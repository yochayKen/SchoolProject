#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "first_stage.h"
#include "handle_string.h"
#include "list.h"
#include "error.h"
#include "utils.h"

#define LINE_BUFFER_SIZE 80

#define SYMBOL_POSITION 1
#define STRUCT_POS_NO_SYMBOL 1
#define STRUCT_POS_AFTER_SYMBOL 2
#define SYMBOL_TERMINATE_CHARACTER ':'

#define DATA_TYPE ".data"
#define STRING_TYPE ".string"
#define STRUCT_TYPE ".struct"
#define INTERNAL_TYPE ".intern"
#define EXTERNAL_TYPE ".extern"

Bool validate_symbol(char *symbol_name)
{
    char c = get_first_char(symbol_name);
    if (isalpha(c) != 0 || c == '_')
        return TRUE;
    return FALSE;
}

Symbol *create_symbol_instance(char *symbol_name, DataStructureType type, unsigned int decleration_line)
{
    Symbol *s = (Symbol *)malloc(sizeof(Symbol));
    s->symbol_name = symbol_name;
    s->data_type = type;
    s->decleration_line = decleration_line;
    return s;
}

Bool append_symbol_to_table(List *symbol_list, Symbol *symbol)
{
    void *s = get_head_element(symbol_list);
    if (s == NULL)
    {
        append_to_list(symbol_list, (void *)symbol);
        return TRUE;
    }
    while (s != NULL)
    {
        if (strcmp(symbol->symbol_name, s->symbol_name) == 0)
            return FALSE;
        s = (Symbol *)get_next_element(symbol_list);
    }
    append_to_list(symbol_list, (void *)symbol);
    return TRUE;
}

DataStructureType check_for_data_structure(char *content, unsigned int pos)
{
    char *word = (char *)malloc(LINE_BUFFER_SIZE);
    word = get_nth_substring(content, pos);
    if (strcmp(word, DATA_TYPE) == 0)
        return DATA;
    else if (strcmp(word, STRING_TYPE) == 0)
        return STRING;
    else if (strcmp(word, STRUCT_TYPE) == 0)
        return STRUCT;
    else if (strcmp(word, INTERNAL_TYPE) == 0)
        return INTERNAL;
    else if (strcmp(word, EXTERNAL_TYPE) == 0)
        return EXTERNAL;
    else
        return NONE;
}

void get_symbol_name(char *content, char *symbol_name)
{
    strcpy(symbol_name ,get_nth_substring(content, SYMBOL_POSITION));
    remove_last_char(symbol_name);
}

Bool check_for_symbol(char *content)
{
    char *word;
    word = get_nth_substring(content, SYMBOL_POSITION);
    if (is_end_with(word, SYMBOL_TERMINATE_CHARACTER) == TRUE)
        return TRUE;
    return FALSE;
}

Bool start_first_stage(File *file)
{
    int instruction_counter = 0, data_counter = 0;
    int word_counter = 0;
    unsigned int line_number = 1;

    FILE *fp = fopen(file->file_name, "r");
    
    List *symbol_table = create_list();

    char *line_content = (char *)malloc(LINE_BUFFER_SIZE);
    int type;

    while(line_number <= file->num_of_lines)
    {
        fgets(line_content, LINE_BUFFER_SIZE, fp);
        if (check_for_symbol(line_content) == TRUE)
        {
            char *symbol_name = (char *)malloc(LINE_BUFFER_SIZE);
            get_symbol_name(line_content, symbol_name);
            if (validate_symbol(symbol_name) == FALSE)
            {
                declare_an_error(INVALID_SYMBOL_DECLERATION, line_number);
                return FALSE;
            }
            type = check_for_data_structure(line_content, STRUCT_POS_AFTER_SYMBOL);
            Symbol *symbol_data = create_symbol_instance(symbol_name, type, line_number);
            if (append_symbol_to_table(symbol_table, symbol_data) == FALSE)
            {
                declare_an_error(SYMBOL_ALREADY_DECLARED, line_number);
                return FALSE;
            }
        }
        line_number++;
    }
    fclose(fp);
    return TRUE;
}
