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

typedef struct special_symbol{
    char *symbol_name;
    MemoryCell address;
}SpecialSymbol;

#define MEMORY_ADDRESS_OFFSET 100
#define MEMORY_SIZE 156 /* Because we start from 100th memory cell and there are only 256 cells*/
#define SHIFT_FIRST_REGISTER 6
#define SHIFT_SECOND_REGISTER 2
#define SHIFT_NUMBER 2
#define BASE_32 32
#define NUM_OF_BYTES_TO_MACHINE_CODE 3
#define SRC_OPERAND_VAL 0x30
#define DST_OPERAND_VAL 0xc

#define ENTRY_SYMBOL ".entry"
#define ENTRY_EXTENSION "ent"
#define EXTERN_EXTENSION "ext"
#define OBJECT_EXTENSION "obj"

static MemoryCell instruction_counter = 0;
static MemoryCell data_counter = 0;

static MemoryCell memory[MEMORY_SIZE];
static MemoryCell *instruction_storage;
static MemoryCell *data_storage;

static List *extern_table;
static List *entry_table;

/*
Checking if the string is assembly instruction. 
Return TRUE if it is, FALSE otherwise
*/
Bool is_instruction(char *str)
{
    if (get_instruction_value(str) != -1)
        return TRUE;
    return FALSE;
}

/*
Checks if the given string is a register. If not, return -1
otherwise, the register value is returned.
*/
int is_registet(char *str)
{
    return get_register_value(str);
}

/*Converting int char to int*/
int convert_char_to_int(char c)
{
    return (c - '0');
}

/*Convert memory value into base 32 value*/
char *convert_memory_value_to_machine_code(int value)
{
    char *machine_code = (char *)malloc(NUM_OF_BYTES_TO_MACHINE_CODE);
    int high_value = value / BASE_32;
    int low_value = value % BASE_32;

    if (value < 0)
    {
        high_value = (value & 0x3e0) / BASE_32; /*0x3e0 => doing masking on high 5 bits*/
        low_value = (value & 0x1f) % BASE_32; /*0x1f => doing masking on low 5 bits*/
    }

    machine_code[0] = get_encode_base_32(high_value);
    machine_code[1] = get_encode_base_32(low_value);
    machine_code[2] = '\0';
    return machine_code;
}

/*Creating sepical symbol for entry and extern table*/
SpecialSymbol *create_special_symbol(Symbol *s)
{
    SpecialSymbol *ss = (SpecialSymbol *)malloc(sizeof(SpecialSymbol));
    ss->symbol_name = s->symbol_name;
    ss->address = instruction_counter + MEMORY_ADDRESS_OFFSET;
    return ss;
}

/*Shifter to align the value position into a memory cell*/
void shift_left(MemoryCell *memory_value, int value, unsigned int offset)
{
    value <<= offset;
    *memory_value |= value;
}

/*Search symbol from the table*/
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

/*Return the memory address of the symbol*/
int get_memory_address(char *symbol, List *symbol_table)
{
    Symbol *s = get_symbol_from_table(symbol, symbol_table);
    if (s == NULL)
        return -1;
    return s->declared_address;
}

/*Extract symbol name from string. The definition should be like XXXX.X*/
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

/*Add extern or entry symbol into a different table*/
void add_symbol_to_special_table(Symbol *s)
{
    SpecialSymbol *ss = create_special_symbol(s);
    if (s->type == EXTERN)
        append_to_list(extern_table, (void *)ss);
    else
        append_to_list(entry_table, (void *)ss);
}

/*Add symbol address into a memory. Also checks the ARE bits*/
void add_symbol_to_memory(MemoryCell memory_value, int tmp_value, char *src_operand, List *symbol_table)
{
    Symbol *s;

    s = get_symbol_from_table(src_operand, symbol_table);
    if (s->type != EXTERN)
    {
        shift_left(&memory_value, tmp_value, SHIFT_NUMBER);
        memory_value = memory_value | INTERNAL;
    }
    else
        memory_value = memory_value | EXTERNAL;
    if (s->type == EXTERN || s->type == ENTRY)
        add_symbol_to_special_table(s);
    memory[instruction_counter++] = memory_value;
}

void add_constant_to_memory(MemoryCell memory_value, char *operand)
{
    int tmp_val;
    /*Adding the number into memory*/
    remove_first_char(operand);
    tmp_val = atoi(operand);
    shift_left(&memory_value, tmp_val, SHIFT_NUMBER);
    memory[instruction_counter++] = memory_value;
}

/**/
void add_referenced_symbol_value(MemoryCell memory_value, char *operand, List *symbol_table)
{
    int tmp_val;
    char *tmp_str;
    char tmp_char;

    /*Add the address of the symbol*/
    tmp_str = get_symbol_name(operand);
    tmp_val = get_memory_address(tmp_str, symbol_table);
    shift_left(&memory_value, tmp_val, SHIFT_NUMBER);
    memory_value = memory_value | INTERNAL;
    memory[instruction_counter++] = memory_value;

    /*Add the number after dot*/
    memory_value = 0;   
    tmp_char = get_last_char(operand);
    tmp_val = convert_char_to_int(tmp_char);
    shift_left(&memory_value, tmp_val, SHIFT_NUMBER);
    memory[instruction_counter++] = memory_value;
}

/*Handle the source and the destination operands*/
void handle_all_operands(char *src_opernad, char *dst_operand, List *symbol_table)
{
    MemoryCell memory_value = 0;
    int src_value;
    int dst_value;

    if ((src_value = get_register_value(src_opernad)) != -1)
    {
        shift_left(&memory_value, src_value, SHIFT_FIRST_REGISTER);
        if ((dst_value = get_register_value(dst_operand)) != -1)
        {
            shift_left(&memory_value, dst_value, SHIFT_SECOND_REGISTER);
            memory[instruction_counter++] = memory_value;
            return;
        }
        memory[instruction_counter++] = memory_value;

        memory_value = 0;
        if ((dst_value = get_memory_address(dst_operand, symbol_table)) != -1)
            add_symbol_to_memory(memory_value, dst_value, dst_operand, symbol_table);
        else if (is_start_with(dst_operand, HASH) == TRUE)
            add_constant_to_memory(memory_value, dst_operand);
        else
            add_referenced_symbol_value(memory_value, dst_operand, symbol_table);
    }
    else if ((src_value = get_memory_address(src_opernad, symbol_table)) != -1)
    {
        add_symbol_to_memory(memory_value, src_value, src_opernad, symbol_table);
        memory_value = 0;
        if ((dst_value = get_register_value(dst_operand)) != -1)
        {
            shift_left(&memory_value, dst_value, SHIFT_SECOND_REGISTER);
            memory[instruction_counter++] = memory_value;
        }
        else if (is_start_with(dst_operand, HASH) == TRUE)
            add_constant_to_memory(memory_value, dst_operand);
        else
            add_referenced_symbol_value(memory_value, dst_operand, symbol_table);
    }
    else if (is_start_with(src_opernad, HASH) == TRUE)
    {
        add_constant_to_memory(memory_value, src_opernad);
        memory_value = 0;
        if ((dst_value = get_register_value(dst_operand)) != -1)
        {
            shift_left(&memory_value, dst_value, SHIFT_SECOND_REGISTER);
            memory[instruction_counter++] = memory_value;
        }
        else if ((dst_value = get_memory_address(dst_operand, symbol_table)) != -1)
            add_symbol_to_memory(memory_value, dst_value, dst_operand, symbol_table);
        else
            add_referenced_symbol_value(memory_value, dst_operand, symbol_table);
    }
    else
    {
        add_referenced_symbol_value(memory_value, src_opernad, symbol_table);
        memory_value = 0;
        if ((dst_value = get_register_value(dst_operand)) != -1)
        {
            shift_left(&memory_value, dst_value, SHIFT_SECOND_REGISTER);
            memory[instruction_counter++] = memory_value;
        }
        else if ((dst_value = get_memory_address(dst_operand, symbol_table)) != -1)
            add_symbol_to_memory(memory_value, dst_value, dst_operand, symbol_table);
        else
            add_constant_to_memory(memory_value, dst_operand);
    }
}

/*
Handling only if source operand is exists.
 Checking the type of the operand and insert it into the memory
*/
void handle_src_operand(char *src_operand, List *symbol_table)
{
    MemoryCell memory_value = 0;
    int tmp_value;

    if ((tmp_value = get_register_value(src_operand)) != -1)
    {
        /*Adding register value into memory*/
        shift_left(&memory_value, tmp_value, SHIFT_FIRST_REGISTER);
        memory[instruction_counter++] = memory_value;
    }
    else if ((tmp_value = get_memory_address(src_operand, symbol_table)) != -1)
        add_symbol_to_memory(memory_value, tmp_value, src_operand, symbol_table);
    else if (is_start_with(src_operand, HASH) == TRUE)
        add_constant_to_memory(memory_value, src_operand);
    else
        add_referenced_symbol_value(memory_value, src_operand, symbol_table);
}

/*Handle all the arguments after the command*/
void handle_arguments(List *symbol_table)
{
    char *src_operand = strtok(NULL, TOKENS);
    char *dst_operand = strtok(NULL, TOKENS);

    if (src_operand == NULL && dst_operand == NULL)
        return;
    else if (src_operand != NULL && dst_operand == NULL)
        handle_src_operand(src_operand, symbol_table);
    else
        handle_all_operands(src_operand, dst_operand, symbol_table);
}

/*Calculate the data offset between symbol data types (DATA, STRUCT, STRING).*/
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

/*Handling symbol definition and add its data into the memory*/
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

/*Handling entry symbol and insert it into the table*/
void handle_entry_symbol(char *str, List *symbol_table)
{
    Symbol *s;

    s = create_symbol_type(str, 0, ENTRY);
    handle_symbol(s, symbol_table);
}

/*Handling the line arguments, and convert then into a machine code*/
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
        memory[instruction_counter] = instruction_storage[instruction_counter - data_counter]; /*Issue*/
        instruction_counter++;
    }
    handle_arguments(symbol_table);
}

/*Creating the entry file. If not entry exists, will not produce one*/
void create_entry_file(File *file)
{
    char *new_file_name;
    char *base_32_convertion;
    FILE *fp;
    SpecialSymbol *ss;
    
    if (is_list_empty(entry_table) == TRUE)
        return;

    ss = (SpecialSymbol *)get_head_element(entry_table);
    new_file_name = change_file_extension(file->file_name, ENTRY_EXTENSION);
    fp = fopen(new_file_name, "w");

    while (ss != NULL)
    {
        base_32_convertion = convert_memory_value_to_machine_code(ss->address);
        fprintf(fp, "%s\t%s\n", ss->symbol_name, base_32_convertion);
        ss = (SpecialSymbol *)get_next_element(entry_table);
    }
    fclose(fp);
}

/*Creating the extern file. If not extern exists, will not produce one*/
void create_extern_file(File *file)
{
    char *new_file_name;
    char *base_32_convertion;
    FILE *fp;
    SpecialSymbol *ss;
    
    if (is_list_empty(extern_table) == TRUE)
        return;

    ss = (SpecialSymbol *)get_head_element(extern_table);
    new_file_name = change_file_extension(file->file_name, EXTERN_EXTENSION);
    fp = fopen(new_file_name, "w");

    while (ss != NULL)
    {
        base_32_convertion = convert_memory_value_to_machine_code(ss->address);
        fprintf(fp, "%s\t%s\n", ss->symbol_name, base_32_convertion);
        ss = (SpecialSymbol *)get_next_element(extern_table);
    }
    fclose(fp);
}

/*Creating the object file of the assembler code*/
void create_object_file(File *file)
{
    char *new_file_name;
    char *base_32_instruction;
    char *base_32_address;
    FILE *fp;
    MemoryCell memory_address_value = MEMORY_ADDRESS_OFFSET;
    int i;

    new_file_name = change_file_extension(file->file_name, OBJECT_EXTENSION);
    fp = fopen(new_file_name, "w");
    for (i = 0; i < instruction_counter; i++)
    {
        base_32_instruction = convert_memory_value_to_machine_code(memory[i]);
        base_32_address = convert_memory_value_to_machine_code(memory_address_value);
        fprintf(fp, "%s\t%s\n", base_32_address, base_32_instruction);
        memory_address_value++;
    }
    fclose(fp);
}

/*Starting the second stage to complete the entry, extern and the object files*/
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
    create_extern_file(file);
    create_entry_file(file);
    create_object_file(file);
}