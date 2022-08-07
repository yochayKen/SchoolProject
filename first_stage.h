#ifndef _FIRST_STAGE_H_
#define _FIRST_STAGE_H_

#include "utils.h"
#include "file_utils.h"

typedef enum{
    DATA,
    STRING,
    STRUCT,
    EXTERNAL,
    INTERNAL,
    NONE
}DataStructureType;

typedef struct symbol
{
    char *symbol_name;
    DataStructureType data_type;
    unsigned int decleration_line;
}Symbol;

Bool start_first_stage(File *);

#endif