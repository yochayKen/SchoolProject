#ifndef _FIRST_STAGE_H_
#define _FIRST_STAGE_H_

#include "utils.h"
#include "file_utils.h"

typedef enum{
    ENTRY,
    EXTERN,
    DATA,
    STRING,
    STRUCT,
    INSTRUCTION,
    UNKNOWN
}FieldType;

typedef unsigned int MemoryCell;

typedef struct symbol
{
    char *symbol_name;
    MemoryCell declared_address;
    FieldType type;
}Symbol;

Bool start_first_stage(File *);

#endif