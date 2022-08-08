#include <string.h>
#include "tables.h"
#include "first_stage.h"

typedef enum{
    INSTRUCTION_T,
    REGISTER_T,
    STRUCT_T
}TableType;

typedef struct encoder
{
    char symbol;
    unsigned int value: 5; 
}Encoder;

static const Encoder encoding_table[] = {
    {'!', 0}, {'@', 1}, {'#', 2}, {'$', 3},
    {'%', 4}, {'^', 5}, {'&', 6}, {'*', 7},
    {'<', 8}, {'>', 9}, {'a', 10}, {'b', 11},
    {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15},
    {'g', 16}, {'h', 17}, {'i', 18}, {'j', 19},
    {'k', 20}, {'l', 21}, {'m', 22}, {'n', 23},
    {'o', 24}, {'p', 25}, {'q', 26}, {'r', 27},
    {'s', 28}, {'t', 29}, {'u', 30}, {'v', 31}
};

typedef struct variable_type{
    char *variable_name;
    unsigned int value: 4;
}VarType;

static VarType instruction_table[] = {
    {"mov", 0}, {"cmp", 1}, {"add", 2}, {"sub", 3},
    {"not", 4}, {"clr", 5}, {"lea", 6}, {"inc", 7},
    {"dec", 8}, {"jmp", 9}, {"bne", 10}, {"get", 11},
    {"prn", 12}, {"jsr", 13}, {"rts", 14}, {"hlt", 15}
};

static VarType register_table[] = {
    {"r0", 0}, {"r1", 1}, {"r2", 2}, {"r3", 3},
    {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7}
};

static VarType struct_table[] = {
    {".data", DATA},  {".string", STRING},  {".struct", STRUCT},
    {".extern", EXTERN}, {".entry", ENTRY}
};

char get_encode_base_32(int num)
{
    int i;
    for (i = 0; i < (sizeof(encoding_table) / sizeof(Encoder)); i++)
    {
        if (encoding_table[i].value == num)
            return encoding_table[i].symbol;
    }
    return '\0';
}

int search_in_table(char *str, TableType type)
{
    int i, table_size;
    VarType *table;

    switch (type)
    {
    case INSTRUCTION_T:
        table_size = (sizeof(instruction_table)/sizeof(VarType));
        table = instruction_table;
        break;
    case REGISTER_T:
        table_size = (sizeof(register_table)/sizeof(VarType));
        table = register_table;
        break;
    default:
        table_size = (sizeof(struct_table)/sizeof(VarType));
        table = struct_table;
        break;
    }

    for (i = 0; i < table_size; i++)
    {
        if (strcmp(table[i].variable_name, str) == 0)
            return table[i].value;
    }
    return -1;
}

int get_instruction_value(char *str)
{
    return search_in_table(str, INSTRUCTION_T);
}

int get_register_value(char *str)
{
    return search_in_table(str, REGISTER_T);
}

int get_struct_value(char *str)
{
    return search_in_table(str, STRUCT_T);
}