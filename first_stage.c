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

#define BUFFER_SIZE 32
#define STORAGE_SPACE 1024
#define SYMBOL_DECLARATION_POS 1
#define INSTRUCTION_SHIFTS 6
#define SRC_ARG_SHIFT 4
#define DST_ARG_SHIFT 2
#define ADDR_OFFSET 100

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
    SRC,
    DST
}ArgType;

/*Return the value of data counter*/
MemoryCell get_data_counter()
{
    return data_counter;
}

/*Returning reference to data storage*/
MemoryCell *get_data_storage()
{
    return data_storage;
}

/*Return a reference to instruction storage*/
MemoryCell *get_instruction_storage()
{
    return instruction_storage;
}

/*Checks if a string is a struct*/
int is_a_struct(char *str)
{
    return get_struct_value(str);
}

/*Checking if string is an instruction*/
int is_an_instruction(char *str)
{
    return get_instruction_value(str);
}

/*Add number into the memory*/
void add_number_to_memory(int number)
{
    data_storage[data_counter++] = number;
    data_storage[data_counter] = '\0';
}

/*Add character into the memory*/
void add_char_to_memory(unsigned char character)
{
    data_storage[data_counter++] = character;
    data_storage[data_counter] = '\0';
}

/*Put instruction into a memory*/
void add_instruction_to_memory(unsigned int data)
{
    instruction_storage[instruction_counter++] = data;
    instruction_storage[instruction_counter] = '\0';
}

/*updating instruction code in a memory value*/
void update_instruction_code(unsigned int *instruction_code, unsigned int instruction_value, int offset)
{
    instruction_value <<= offset;
    *instruction_code |= instruction_value;
}

/*validate if the method is supported*/
void validate_address_method_support(unsigned int address_method_options, AddrMethod method)
{
    if ((address_method_options & method) == 0)
        set_error_type(UNSUPPORTED_ADDR_METHOD);
}

/*Create symbol type*/
Symbol *create_symbol_type(char *symbol_name, unsigned int address, FieldType type)
{
    Symbol *s = (Symbol *)malloc(sizeof(Symbol));
    s->symbol_name = symbol_name;
    s->declared_address = address;
    s->type = type;
    return s;
}

/*checking if its record declaration*/
Bool is_record_declaration(char *str)
{
    if (strchr(str, DOT) != NULL && is_start_with(str, DOT) == FALSE)
        return TRUE;
    return FALSE;
}

/*Checking if comma exists inside the string*/
void check_for_comma(char *content, char *str)
{
    char *buffer = (char *)malloc(BUFFER_SIZE);
    int line_length = strlen(content);
    Bool found_str = FALSE;

    int i = 0, j = 1;
    while (i < line_length)
    {
        if (found_str == TRUE)
        {
            if (!isspace(content[i]) && content[i] == COMMA)
                return;
            else if (isspace(content[i]))
            {
                i++;
                continue;
            }
            else
                break;
        }
        if (isspace(content[i]))
        {
            i++;
            j = 1;
            if (strlen(buffer) != 0)
                memset(buffer, 0, BUFFER_SIZE);
            continue;
        }
        else
        {
            buffer[j - 1] = content[i];
            buffer[j] = '\0';
            if (strcmp(buffer, str) == 0)
                found_str = TRUE;
            j++;
            i++;
            continue;
        }
    }
    set_error_type(NO_COMMA_FOUND);
}

/*Checking the method type and validate its type*/
int check_method_type(char *arg, unsigned int type)
{
    if (is_start_with(arg, HASH) == TRUE)
    {
        validate_address_method_support(type, IMMEDIATE_ADDR);
        return IMMEDIATE;
    }
    else if (get_register_value(arg) != -1)
    {
        validate_address_method_support(type, REGISTER_ADDR);
        return REGISTER;
    }
    else if (is_record_declaration(arg) == TRUE)
    {
        validate_address_method_support(type, RECORD_ADDR);
        return RECORD;
    }
    else
    {
        validate_address_method_support(type, DIRECT_ADDR);
        return DIRECT;
    }
}

/*Creating an offset between an instrucion to its arguments in the memory*/
void create_offset_to_instruction_pointer(char *src, char *dst)
{
    int src_val = -1;
    int dst_val;
    if (src != NULL)
    {
        src_val = check_method_type(src, 0xf);
    }
    dst_val = check_method_type(dst, 0xf);
    switch (dst_val)
    {
    case REGISTER:
        if (src_val == REGISTER)
            instruction_counter++;
        else if (src_val == RECORD)
            instruction_counter+=3;
        else if (src_val == DIRECT || src_val == IMMEDIATE)
            instruction_counter+=2;
        else
            instruction_counter++;
        break;
    case IMMEDIATE:
        if (src_val == RECORD)
            instruction_counter+=3;
        else if (src_val == REGISTER || src_val == DIRECT)
            instruction_counter+=2;
        else
            instruction_counter++;
        break;
    case DIRECT:
        if (src_val == RECORD)
            instruction_counter+=3;
        else if (src_val == REGISTER || src_val == IMMEDIATE)
            instruction_counter+=2;
        else
            instruction_counter++;
        break;
    default:
        if (src_val == RECORD)
            instruction_counter+=4;
        else if (src_val == REGISTER || src_val == IMMEDIATE || src_val == DIRECT)
            instruction_counter+=3;
        else
            instruction_counter+=2;
        break;
    }
}

/*Handling instruction by getting its all arguments*/
void handle_instruction_type(char *content, List *symbol_table)
{
    char *instruction;
    char *src = NULL;
    char *dst = NULL;

    unsigned int instruction_code = 0;
    unsigned int content_len = strlen(content);

    int instruction_value;

    InstructionType *it = (InstructionType *)malloc(sizeof(InstructionType));

    char *tmp = (char *)malloc(content_len + 1);
    strcpy(tmp, content);
    
    instruction = strtok(tmp, TOKENS);
    if (is_end_with(instruction, COLON) == TRUE)
        instruction = strtok(NULL, TOKENS);
    
    if (get_instruction(instruction, it) == FALSE)
    {
        set_error_type(COULD_NOT_FIND_INSTRUCTION);
        return;
    }
    update_instruction_code(&instruction_code, it->value, INSTRUCTION_SHIFTS);
    if (it->max_args == 0)
    {
        if (strtok(NULL, TOKENS) != NULL)
        {
            set_error_type(TO_MANY_ARGUEMNTS);
            return;
        }
        add_instruction_to_memory(instruction_code);
        return;
    }

    if (it->addr_src_support != 0)
    {
        src = strtok(NULL, TOKENS);
        instruction_value = check_method_type(src, it->addr_src_support);
        update_instruction_code(&instruction_code, instruction_value, SRC_ARG_SHIFT);
        check_for_comma(content, src);
        if (get_error() == TRUE)
            return;
    }
    dst = strtok(NULL, TOKENS);
    instruction_value = check_method_type(dst, it->addr_dst_support);
    update_instruction_code(&instruction_code, instruction_value, DST_ARG_SHIFT);

    add_instruction_to_memory(instruction_code);
    create_offset_to_instruction_pointer(src, dst);
}

/*Handle string structure and put its value into a data storage*/
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
        str++;
        while(str != '\0')
        {
            if (*str == QUOTATION_MARK)
                break;
            add_char_to_memory(*str);
            str++;
        }
        add_char_to_memory('\0');
        if (type == STRUCT)
        {
            if (get_error() == TRUE)
                set_error_type(ERROR_IN_STRUCT_DECLARATION);
            return;
        }
    }
}

/*Handling symbol data type and put its data into a data storage*/
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
        if (type == STRUCT)
        {
            if (get_error() == TRUE)
                set_error_type(ERROR_IN_STRUCT_DECLARATION);
            return;
        }
    }
}

/*Adding symbol into the symbol table*/
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

/*Handling struct type symbol*/
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

/*Handling extern symbol type*/
void handle_extern_symbol(char *content, int pos, List *symbol_table)
{
    char *str = get_nth_substring(content, ++pos);
    Symbol *ext_symbol = create_symbol_type(str, 0, EXTERN);
    add_symbol_to_table(ext_symbol, symbol_table);
}

/*Checking a struct type by a symbol type field*/
void handle_struct_type(FieldType type, char *content, int pos, List *symbol_table)
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
        handle_extern_symbol(content, pos, symbol_table);
        break;
    case ENTRY:
        break;
    default:
        set_error_type(UNKNOWN_DATA_STRUCT_TYPE);
    }
}

/*Checking if symbols has the same name*/
int check_symbol_name(void *current_symbol, void *orig_symbol)
{
    Symbol *s1 = (Symbol *)current_symbol;
    Symbol *s2 = (Symbol *)orig_symbol;

    if (strcmp(s1->symbol_name, s2->symbol_name) == 0)
        return 0;
    return 1;
}

/*updating symbol declaration memory address*/
void update_symbol_address(Symbol *symbol, FieldType type, List *symbol_table)
{
    Symbol *s = (Symbol *)get_head_element(symbol_table);
    while (s != NULL)
    {
        if (strcmp(s->symbol_name, symbol->symbol_name) == 0)
        {
            s->type = type;
            if (type == INSTRUCTION)
                s->start_memory_index = instruction_counter;
            else
                s->start_memory_index = data_counter;
            s->declared_address = instruction_counter + data_counter + ADDR_OFFSET;
            return;
        }
        s = (Symbol *)get_next_element(symbol_table);
    }
}

/*Removing a symbol from a table*/
void remove_symbol_from_table(Symbol *symbol, List *symbol_table)
{
    remove_from_list(symbol_table, (void *)symbol, check_symbol_name);
}

/*Symbol validator*/
Bool validate_symbol(char *symbol_name)
{
    char c = get_first_char(symbol_name);
    if (isalpha(c) != 0 || c == '_')
        return TRUE;

    set_error_type(INVALID_SYMBOL_DECLERATION);
    return FALSE;
}

/*Handling a symbol - validate and adding into a table*/
void handle_symbol(Symbol *symbol, List *symbol_table)
{
    if (validate_symbol(symbol->symbol_name) == FALSE)
        return;
    add_symbol_to_table(symbol, symbol_table);
}

/*Checking is a string is a symbol*/
Bool is_a_symbol(char *str, int pos)
{
    if (is_end_with(str, COLON) == TRUE && pos == SYMBOL_DECLARATION_POS)
        return TRUE;
    return FALSE;
}

/*Handle a single line of instructions*/
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
        remove_last_char(str);
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
        handle_struct_type(struct_type, content, str_pos, symbol_table);
    }
    else if ((instruction_type = is_an_instruction(str)) != -1)
    {
        
        if (is_symbol == TRUE)
            update_symbol_address(symbol, INSTRUCTION, symbol_table);
        handle_instruction_type(content, symbol_table);
    }
    else
        set_error_type(UNKNOWN_DECLARED_STR);
    return;
}

/*Starting first stage. Creating the instruction memory values and looking for symbols declerations*/
Bool start_first_stage(File *file, List *symbol_table)
{
    unsigned int line_number = 1;
    char *line_content = (char *)malloc(LINE_BUFFER_SIZE);
    FILE *fp = fopen(file->file_name, "r");

    data_counter = 0;
    instruction_counter = 0;

    while(fgets(line_content, LINE_BUFFER_SIZE, fp) != NULL)
    {
        read_line(line_content, symbol_table);
        if (get_error() == TRUE)
        {
            declare_an_error(get_error_type(), line_number);
            if (is_list_empty(symbol_table) == FALSE)
                delete_list(symbol_table);
            return FALSE;
        }
        line_number++;
    }
    fclose(fp);
    return TRUE;
}
