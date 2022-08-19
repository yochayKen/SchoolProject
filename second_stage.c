#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "error.h"
#include "utils.h"
#include "tables.h"
#include "handle_string.h"
#include "first_stage.h"

typedef enum{
    EXTERNAL = 1,
    INTERNAL = 2
}ARE_VALUE;

#define MEMORY_ADDRESS_OFFSET 100
#define MEMORY_SIZE 156 /* Because we start from 100th memory cell and there are 256 cells*/
#define SHIFT_FIRST_REGISTER 6
#define SHIFT_SECOND_REGISTER 2
#define SHIFT_NUMBER 2
#define SRC_OPERAND_VAL 0x30
#define DST_OPERAND_VAL 0xc

#define ENTRY_SYMBOL ".entry"

static MemoryCell instruction_counter = 0;
static MemoryCell data_counter = 0;

static MemoryCell memory[MEMORY_SIZE];
static MemoryCell *instruction_storage;
static MemoryCell *data_storage;

static List *extern_table;
static List *entry_table;


Bool is_instruction(char *str)
{
    if (get_instruction_value(str) != -1)
        return TRUE;
    return FALSE;
}

int is_registet(char *str)
{
    return get_register_value(str);
}

void shift_left(MemoryCell *memory_value, int value, unsigned int offset)
{
    value <<= offset;
    *memory_value |= value;
}

Symbol *get_symbol_from_table(char *symbol, List *symbol_table)
{
    Symbol *s = (Symbol *)get_head_element(symbol_table);

    while (s != NULL)
    {
        if (strcmp(symbol, s->symbol_name) == 0)
            return s;
        s = (Symbol *)get_next_element(symbol_table);
    }
    return NULL;
}

int get_memory_address(char *symbol, List *symbol_table)
{
    Symbol *s = get_symbol_from_table(symbol, symbol_table);
    if (s == NULL)
        return -1;
    return s->declared_address;
}

char *get_symbol_name(char *str)
{
    int str_len = strlen(str) + 1;
    char *tmp = (char *)malloc(str_len);
    int i = 0, j = 1;

    while (i < str_len)
    {
        if (str[i] == DOT)
            return tmp;
        tmp[j - 1] = str[i];
        tmp[j] = '\0';
        i++;
        j++;
    }
    return NULL;
}

void handle_all_operands()
{
    return;
}

void handle_src_operand(char *src_operand, List *symbol_table)
{
    MemoryCell memory_value = 0;
    Symbol *s;
    int tmp_value;
    char *tmp_str;

    printf("%s\n", src_operand);
    if ((tmp_value = get_register_value) != -1)
    {
        shift_left(&memory_value, tmp_value, SHIFT_FIRST_REGISTER);
        memory[instruction_counter++] = memory_value;
    }
    else if ((tmp_value = get_memory_address(src_operand, symbol_table)) != -1)
    {
        s = get_symbol_from_table(src_operand, symbol_table);
        if (s->type != EXTERN)
        {
            shift_left(&memory_value, tmp_value, SHIFT_NUMBER);
            memory_value = memory_value | INTERNAL;
        }
        else
            memory_value = memory_value | EXTERNAL;
        memory[instruction_counter++] = memory_value;
    }
    else if (is_start_with(src_operand, HASH) == TRUE)
    {
        remove_first_char(src_operand);
        tmp_value = atoi(src_operand);
        shift_left(&memory_value, tmp_value, SHIFT_NUMBER);
        memory[instruction_counter++] = memory_value;
    }
    else
    {
        tmp_str = get_symbol_name(src_operand);
        tmp_value = get_memory_address(tmp_str, symbol_table);
        shift_left(&memory_value, tmp_value, SHIFT_NUMBER);
        memory_value = memory_value | INTERNAL;
        memory[instruction_counter++] = memory_value;

        memory_value = 0;
        tmp_str = get_last_char(src_operand);
        tmp_value = atoi(tmp_str);
        shift_left(&memory_value, tmp_value, SHIFT_NUMBER);
        memory[instruction_counter++] = memory_value;
    }
}

void handle_arguments(List *symbol_table)
{
    char *src_operand = strtok(NULL, TOKENS);
    char *dst_operand = strtok(NULL, TOKENS);

    if (src_operand == NULL && dst_operand == NULL)
        return;
    else if (src_operand != NULL && dst_operand == NULL)
        handle_src_operand(src_operand, symbol_table);
    else
        handle_all_operands();
}

int get_num_of_cells(MemoryCell *data_storage)
{
    int i;
    for (i = 0; data_storage[i] != '\0'; i++);
    return i;
}

int get_data_offset(Symbol *s, List *symbol_table)
{
    Symbol *symbol = (Symbol *)get_head_element(symbol_table);
    int current_index = s->start_memory_index, last_index = 0;
    int data_storage_size = get_data_counter();

    while (symbol != NULL)
    {
        if (strcmp(s->symbol_name, symbol->symbol_name) == 0)
        {
            while (symbol != NULL)
            {
                symbol = (Symbol *)get_next_element(symbol_table);
                if (symbol == NULL)
                    return (data_storage_size - data_counter);
                if (symbol->type == DATA || symbol->type == STRING || symbol->type == STRUCT)
                {
                    last_index = symbol->start_memory_index;
                    break;
                }
            }
            return (last_index - current_index);
        }
        symbol = (Symbol *)get_next_element(symbol_table);
    }
    return 0;
}

Bool handle_symbol_definition(char *str, List *symbol_table)
{
    Symbol *s;
    int data_offset, i = 0;

    remove_last_char(str);
    s = get_symbol_from_table(str, symbol_table);
    if (s->type == DATA || s->type == STRUCT || s->type == STRING)
    {
        data_offset = get_data_offset(s, symbol_table);
        while (i < data_offset)
        {
            memory[instruction_counter++] = data_storage[data_counter++];
            i++;
        }
        return TRUE;
    }
    return FALSE;
}

void handle_entry_symbol(char *str, List *symbol_table)
{
    Symbol *s;

    s = create_symbol_type(str, 0, ENTRY);
    handle_symbol(s, symbol_table);
}

void handle_line(char *line_content, List *symbol_table)
{
    char *instruction;
    char *tmp = (char *)malloc(strlen(line_content) + 1);

    strcpy(tmp, line_content);
    instruction = strtok(tmp, TOKENS);

    if (is_start_with(instruction, SEMI_COLON) == TRUE)
        return;

    if (is_end_with(instruction, COLON) == TRUE)
    {
        if (handle_symbol_definition(instruction, symbol_table) == FALSE)
            instruction = strtok(NULL, TOKENS);
        else
            return;
    }

    if (is_start_with(instruction, DOT) == TRUE)
    {
        if (strcmp(instruction, ENTRY_SYMBOL) == 0)
        {
            instruction = strtok(NULL, TOKENS);
            handle_entry_symbol(instruction, symbol_table);
        }
        return;
    }
        
    else if (is_instruction(instruction) == TRUE)
    {
        memory[instruction_counter] = instruction_storage[instruction_counter];
        instruction_counter++;
    }
    handle_arguments(symbol_table);
}

void create_entry_file(List *symbol_table)
{
    return;
}

void create_extern_file(List *symbol_table)
{
    return;
}

void print_table(void *s)
{
    Symbol *sm = (Symbol *)s;
    printf("symbol named: %s is at %d address, type: %d\n", sm->symbol_name, sm->declared_address, sm->type);
}

void print_memory_cells()
{
    int i;
    for (i = 0; i < MEMORY_SIZE; i++)
    {
        printf("%x, ", memory[i]);
    }
    printf("\n");
}

void start_second_stage(File *file, List *symbol_table)
{
    char *line_content = (char *)malloc(LINE_BUFFER_SIZE);
    FILE *fp = fopen(file->file_name, "r");

    instruction_storage = get_instruction_storage();
    data_storage = get_data_storage();

    extern_table = create_list();
    entry_table = create_list();

    while (fgets(line_content, LINE_BUFFER_SIZE, fp) != NULL)
    {
        handle_line(line_content, symbol_table);
    }
    print_list(symbol_table, print_table);
    print_memory_cells();
}