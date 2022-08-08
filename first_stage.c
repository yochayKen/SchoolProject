#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "first_stage.h"
#include "handle_string.h"
#include "tables.h"
#include "list.h"
#include "error.h"
#include "utils.h"

#define LINE_BUFFER_SIZE 80
#define STORAGE_SPACE 1024
#define SYMBOL_DECLARATION_POS 1

#define SYMBOL_TERMINATE_CHARACTER ':'
#define SEMI_COLON ';'

static unsigned int data_counter = 0;
static unsigned int instruction_counter = 0;

static unsigned int data_storage[STORAGE_SPACE];
static unsigned int instruction_storage[STORAGE_SPACE];

void set_data_counter(unsigned int count)
{
    data_counter = count;
}

unsigned int get_data_counter()
{
    return data_counter;
}

void set_instruction_counter(unsigned int count)
{
    instruction_counter = count;
}

unsigned int get_instruction_counter()
{
    return instruction_counter;
}

int is_a_struct(char *str)
{
    return get_struct_value(str);
}

int is_an_instruction(char *str)
{
    return get_instruction_value(str);
}

void update_symbol_address(Symbol *s, unsigned int address)
{
    s->declared_address = address;
}

void handle_struct_type(FieldType type, char *content, int pos)
{
    return;
}

int check_symbol_name(void *current_symbol, void *orig_symbol)
{
    Symbol *s1 = (Symbol *)current_symbol;
    Symbol *s2 = (Symbol *)orig_symbol;

    if (strcmp(s1->symbol_name, s2->symbol_name) == 0)
        return 0;
    return 1;
}

Symbol *create_symbol_type(char *symbol_name, unsigned int address, FieldType type)
{
    Symbol *s = (Symbol *)malloc(sizeof(Symbol));
    s->symbol_name = symbol_name;
    s->declared_address = address;
    s->type = type;
    return s;
}

void remove_symbol_from_table(Symbol *symbol, List *symbol_table)
{
    remove_from_list(symbol_table, (void *)symbol, check_symbol_name);
}

void add_symbol_to_table(Symbol *symbol, List *symbol_table)
{
    Symbol *tmp;
    Symbol *current_symbol = (Symbol *)malloc(sizeof(Symbol));

    if (is_list_empty(symbol_table) == TRUE)
    {
        append_to_list(symbol_table, (void *)symbol);
        return;
    }
    
    tmp = (Symbol *)get_head_element(symbol_table);
    while (tmp != NULL)
    {
        if(strcmp(tmp->symbol_name, symbol->symbol_name) == 0)
        {
            set_error_type(SYMBOL_ALREADY_DECLARED);
            return;
        }
        tmp = (Symbol *)get_next_element(symbol_table);
    }
    memcpy(current_symbol, symbol, sizeof(Symbol));
    append_to_list(symbol_table, (void *)current_symbol);
}


Bool validate_symbol(char *symbol_name)
{
    char c = get_first_char(symbol_name);
    if (isalpha(c) != 0 || c == '_')
        return TRUE;

    set_error_type(INVALID_SYMBOL_DECLERATION);
    return FALSE;
}

void handle_symbol(Symbol *symbol, List *symbol_table)
{
    if (validate_symbol(symbol->symbol_name) == FALSE)
        return;

    remove_last_char(symbol->symbol_name);
    add_symbol_to_table(symbol, symbol_table);
}

Bool is_a_symbol(char *str, int pos)
{
    if (is_end_with(str, SYMBOL_TERMINATE_CHARACTER) == TRUE && pos == SYMBOL_DECLARATION_POS)
        return TRUE;
    return FALSE;
}

void read_line(char *content, List *symbol_table)
{
    int struct_type, instruction_type, str_pos = 1;
    Symbol *symbol;
    Bool is_symbol = FALSE;
    char *str;

    while ((str = get_nth_substring(content, str_pos)) != NULL)
    {
        if (is_start_with(content, SEMI_COLON) == TRUE)
            return;
        
        if (is_a_symbol(str, str_pos) == TRUE)
        {
            is_symbol = TRUE;
            symbol = create_symbol_type(str, 0, UNKNOWN);
            handle_symbol(symbol, symbol_table);
            if (get_error() == TRUE)
                return;
            str = get_nth_substring(content, ++str_pos); 
        }

        if ((struct_type = is_a_struct(str)) != -1)
        {
            if (is_symbol == TRUE)
            {
                if (struct_type == ENTRY || struct_type == EXTERN)
                {
                    printf("Warning! do not declare %s after symbol declaration\n", str);
                    remove_symbol_from_table(symbol, symbol_table);
                    return;
                }
                else
                    update_symbol_address(symbol, get_data_counter());
            }
            
            str_pos++;
            handle_struct_type(struct_type, content, str_pos);
        }
        else if ((instruction_type = is_an_instruction(str)) != -1)
        {
            printf("instruction: %s\n", str);
        }
        
        str_pos++;
    }
}

/*For test*/
void print(void *s)
{
    Symbol *sm = (Symbol *)s;
    printf("%s\n", sm->symbol_name);
}

Bool start_first_stage(File *file)
{
    unsigned int line_number = 1;
    char *line_content = (char *)malloc(LINE_BUFFER_SIZE);
    List *symbol_table = create_list();
    FILE *fp = fopen(file->file_name, "r");

    set_data_counter(0);
    set_instruction_counter(0);

    while(fgets(line_content, LINE_BUFFER_SIZE, fp) != NULL)
    {
        read_line(line_content, symbol_table);
        if (get_error() == TRUE)
        {
            declare_an_error(get_error_type(), line_number);
            return FALSE;
        }
        line_number++;
    }
    print_list(symbol_table, print);
    fclose(fp);
    return TRUE;
}
