#ifndef _FIRST_STAGE_H_
#define _FIRST_STAGE_H_

#include "utils.h"
#include "file_utils.h"
#include "list.h"

#define LINE_BUFFER_SIZE 80

#define COLON ':'
#define SEMI_COLON ';'
#define COMMA ','
#define QUOTATION_MARK '"'
#define HASH '#'
#define DOT '.'
#define TOKENS " ,\n\0\t\r"

typedef enum{
    ENTRY,
    EXTERN,
    DATA,
    STRING,
    STRUCT,
    INSTRUCTION,
    UNKNOWN
}FieldType;

typedef enum{
    IMMEDIATE,
    DIRECT,
    RECORD,
    REGISTER
}Method;

typedef unsigned int MemoryCell;

typedef struct symbol
{
    char *symbol_name;
    MemoryCell declared_address;
    MemoryCell start_memory_index;
    FieldType type;
}Symbol;

MemoryCell *get_data_storage();
MemoryCell *get_instruction_storage();
MemoryCell get_data_counter();
void handle_symbol(Symbol *, List *);
Symbol *create_symbol_type(char *, unsigned int, FieldType);
Bool start_first_stage(File *, List *);

#endif