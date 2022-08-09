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
#define BUFFER_SIZE 32
#define STORAGE_SPACE 1024
#define SYMBOL_DECLARATION_POS 1
#define INSTRUCTION_SHIFTS 6
#define SRC_ARG_SHIFT 4
#define DST_ARG_SHIFT 2

#define SYMBOL_TERMINATE_CHARACTER ':'
#define SEMI_COLON ';'
#define COMMA ','
#define QUOTATION_MARK '"'
#define HASH '#'
#define DOT '.'

static MemoryCell data_counter = 0;
static MemoryCell instruction_counter = 0;

static MemoryCell data_storage[STORAGE_SPACE];
static MemoryCell instruction_storage[STORAGE_SPACE];

typedef enum{
    IMMEDIATE_ADDR = 0x1,
    DIRECT_ADDR = 0x2,
    RECORD_ADDR = 0x4,
    REGISTER_ADDR = 0x8
}AddrMethod;

typedef enum{
    IMMEDIATE,
    DIRECT,
    RECORD,
    REGISTER
}Method;

typedef enum{
    SRC,
    DST
}ArgPos;

int is_a_struct(char *str)
{
    return get_struct_value(str);
}

int is_an_instruction(char *str)
{
    return get_instruction_value(str);
}

void add_number_to_memory(int number)
{
    data_storage[data_counter++] = number;
    data_storage[data_counter] = '\0';
}

void add_char_to_memory(unsigned char character)
{
    data_storage[data_counter++] = character;
    data_storage[data_counter] = '\0';
}

void add_instruction_to_memory(unsigned int data)
{
    instruction_storage[instruction_counter++] = data;
    instruction_storage[instruction_counter] = '\0';
}

void update_instruction_code(unsigned int *instruction_code, unsigned int instruction_value, int offset)
{
    instruction_value <<= offset;
    *instruction_code |= instruction_value;
}

Bool validate_address_method(unsigned int address_method_options, AddrMethod method)
{
    if (address_method_options & method == 1)
        return TRUE;
    return FALSE;
}

Bool check_for_comma(char *str, char *content, int pos)
{
    return TRUE;
}

void handle_instruction_type(char *content, int pos)
{
    char *str;
    char *instruction;

    unsigned int num_of_args = 0;
    unsigned int instruction_code = 0;
    InstructionType *it = (InstructionType *)malloc(sizeof(InstructionType));

    instruction = get_nth_substring(content, pos);
    if (get_instruction(instruction, it) == FALSE)
    {
        set_error_type(COULD_NOT_FIND_INSTRUCTION);
        return;
    }
    update_instruction_code(&instruction_code, it->value, INSTRUCTION_SHIFTS);

    if (it->max_args == 0)
        return;
    
    str = get_nth_substring(content, ++pos);
}

void handle_string_structure_type(FieldType type, char *content, int pos)
{
    char *str;

    while ((str = get_nth_substring(content, ++pos)) != NULL)
    {
        if (is_start_with(str, QUOTATION_MARK) == FALSE || is_end_with(str, QUOTATION_MARK) == FALSE)
        {
            set_error_type(ERROR_IN_STRING_DECLARATION);
            return;
        }
        *str++;
        while(*str != '\0')
        {
            if (*str == QUOTATION_MARK)
                break;
            add_char_to_memory(*str);
            *str++;
        }
        if (type == STRUCT)
        {
            if (get_error() == TRUE)
                set_error_type(ERROR_IN_STRUCT_DECLARATION);
            return;
        }
    }
}

void handle_data_structure_type(FieldType type, char *content, int pos)
{
    char *str;
    int num;

    while ((str = get_nth_substring(content, ++pos)) != NULL)
    {
        if ((num = atoi(str)) == 0)
        {
            set_error_type(DATA_STRUCT_DECLARED_WITH_NO_NUMBER);
            return;
        }
        add_number_to_memory(num);
        if (check_for_comma(str, content, pos) == FALSE)
        {
            set_error_type(NO_COMMA_FOUND);
            return;
        }
        if (type == STRUCT)
        {
            if (get_error() == TRUE)
                set_error_type(ERROR_IN_STRUCT_DECLARATION);
            return;
        }
    }
}

void handle_struct_structure_type(char *content, int pos)
{
    char *str;
    int tmp_pos = pos;
    
    while ((str = get_nth_substring(content, ++tmp_pos)) != NULL)
    {
        if (tmp_pos > pos)
            pos = tmp_pos - 1;
        
        if (atoi(str) != 0)
            handle_data_structure_type(STRUCT, content, pos);
        else if (is_start_with(str, QUOTATION_MARK) == TRUE)
            handle_string_structure_type(STRUCT, content, pos);
        else
            set_error_type(ERROR_IN_STRUCT_DECLARATION);
    }
}

void handle_struct_type(FieldType type, char *content, int pos)
{
    switch (type)
    {
    case DATA:
        handle_data_structure_type(type, content, pos);
        break;
    case STRING:
        handle_string_structure_type(type, content, pos);
        break;
    case STRUCT:
        handle_struct_structure_type(content, pos);
        break;
    case EXTERN:
        /* handle_extern_structure_type(content, pos); */
        break;
    case ENTRY:
        /* handle_entry_structure_type(content, pos); */
        break;
    default:
        set_error_type(UNKNOWN_DATA_STRUCT_TYPE);
    }
}

int check_symbol_name(void *current_symbol, void *orig_symbol)
{
    Symbol *s1 = (Symbol *)current_symbol;
    Symbol *s2 = (Symbol *)orig_symbol;

    if (strcmp(s1->symbol_name, s2->symbol_name) == 0)
        return 0;
    return 1;
}

void update_symbol_address(Symbol *symbol, int type, List *symbol_table)
{
    Symbol *s = (Symbol *)get_head_element(symbol_table);
    while (s != NULL)
    {
        if (strcmp(s->symbol_name, symbol->symbol_name) == 0)
        {
            s->type = type;
            if (type == INSTRUCTION)
                s->declared_address = instruction_counter;
            else
                s->declared_address = data_counter;
            return;
        }
        s = (Symbol *)get_next_element(symbol_table);
    }
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

    str = get_nth_substring(content, str_pos);
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
            }
            else
                update_symbol_address(symbol, struct_type, symbol_table);
        }
        handle_struct_type(struct_type, content, str_pos);
    }
    else if ((instruction_type = is_an_instruction(str)) != -1)
    {
        if (is_symbol == TRUE)
            update_symbol_address(symbol, INSTRUCTION, symbol_table);
        handle_instruction_type(content, str_pos);
    }
    else
        set_error_type(UNKNOWN_DECLARED_STR);
    return;
}

/*For test*/
void print(void *s)
{
    Symbol *sm = (Symbol *)s;
    printf("Label: %s, Address: %d, Type: %d\n", sm->symbol_name, sm->declared_address, sm->type);
}

void print_memory()
{
    int i;
    for (i = 0; data_storage[i + 1] != '\0'; i++)
        printf("%d, ", data_storage[i]);
    printf("%d\n", data_storage[i]);
}

Bool start_first_stage(File *file)
{
    unsigned int line_number = 1;
    char *line_content = (char *)malloc(LINE_BUFFER_SIZE);
    List *symbol_table = create_list();
    FILE *fp = fopen(file->file_name, "r");

    data_counter = 0;
    instruction_counter = 0;

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
