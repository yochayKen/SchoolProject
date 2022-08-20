#include <string.h>
#include "tables.h"
#include "first_stage.h"

typedef enum{
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

static InstructionType instruction_table[] = {
    {"mov", 0, 2, 0xf, 0xe}, {"cmp", 1, 2, 0xf, 0xf}, {"add", 2, 2, 0xf, 0xe}, {"sub", 3, 2, 0xf, 0xe},
    {"not", 4, 1, 0x0, 0xe}, {"clr", 5, 1, 0x0, 0xe}, {"lea", 6, 2, 0x6, 0xe}, {"inc", 7, 1, 0x0, 0xe},
    {"dec", 8, 1, 0x0, 0xe}, {"jmp", 9, 1, 0x0, 0xe}, {"bne", 10, 1, 0x0, 0xe}, {"get", 11, 1, 0x0, 0xe},
    {"prn", 12, 1, 0x0, 0xf}, {"jsr", 13, 1, 0x0, 0xe}, {"rts", 14, 0, 0x0, 0x0}, {"hlt", 15, 0, 0x0, 0x0}
};

typedef struct variable_type{
    char *variable_name;
    unsigned int value: 4;
}VarType;

static VarType register_table[] = {
    {"r0", 0}, {"r1", 1}, {"r2", 2}, {"r3", 3},
    {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7}
};

static VarType struct_table[] = {
    {".data", DATA},  {".string", STRING},  {".struct", STRUCT},
    {".extern", EXTERN}, {".entry", ENTRY}
};

/*Get character presenting the base 32 value*/
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

/*Searching a string in a given table*/
int search_in_table(char *str, TableType type)
{
    int i, table_size;
    VarType *table;

    switch (type)
    {
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

/*Retrieve instruction value from a table*/
int get_instruction_value(char *str)
{
    int i;
    for (i = 0; i < (sizeof(instruction_table)/sizeof(InstructionType)); i++)
    {
        if (strcmp(instruction_table[i].instruction_name, str) == 0)
            return instruction_table[i].value;
    }
    return -1;
}

/*Check if instruction exists in the table*/
Bool get_instruction(char *str, InstructionType *it)
{
    int i;
    for (i = 0; i < (sizeof(instruction_table)/sizeof(InstructionType)); i++)
    {
        if (strcmp(instruction_table[i].instruction_name, str) == 0)
        {
            memcpy(it, &instruction_table[i], sizeof(InstructionType));
            return TRUE;
        }
    }
    return FALSE;
}

/*Get register value from a table*/
int get_register_value(char *str)
{
    return search_in_table(str, REGISTER_T);
}

/*Get struct value from a table*/
int get_struct_value(char *str)
{
    return search_in_table(str, STRUCT_T);
}