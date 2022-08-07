#include "tables.h"

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

typedef struct instruction{
    char *instruction_name;
    unsigned int value: 4;
}Instruction;

static const Instruction instruction_table[] = {
    {"mov", 0}, {"cmp", 1}, {"add", 2}, {"sub", 3},
    {"not", 4}, {"clr", 5}, {"lea", 6}, {"inc", 7},
    {"dec", 8}, {"jmp", 9}, {"bne", 10}, {"get", 11},
    {"prn", 12}, {"jsr", 13}, {"rts", 14}, {"hlt", 15}
};

typedef struct register_type{
    char *register_name;
    unsigned int value: 3;
}Register;

static const Register register_table[] = {
    {"r0", 0}, {"r1", 1}, {"r2", 2}, {"r3", 3},
    {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7}
};