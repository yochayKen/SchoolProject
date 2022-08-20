#ifndef _TABLE_H_
#define _TABLE_H_

#include "utils.h"

typedef struct instruction{
    char *instruction_name;
    unsigned int value: 4;
    unsigned int max_args: 2;
    unsigned int addr_src_support: 4;
    unsigned int addr_dst_support: 4;
}InstructionType;

int get_struct_value(char *);
int get_instruction_value(char *);
int get_register_value(char *);
char get_encode_base_32(int);

Bool get_instruction(char *, InstructionType *);

#endif