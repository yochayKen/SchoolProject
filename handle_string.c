#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "list.h"

typedef enum {
    TOKEN_OPCODE,
    TOKEN_REGISTER,
    TOKEM_LABEL
}Token_t;

typedef struct token{
    char *value;
    Token_t type;
}Token;

char *skip_white_spaces(char *str)
{
    int i = 0;
    while ((str[i] = ' ') || (str[i] == '\t') || (str[i] == EOF) || (str[i] == '\n'))
        i++;
    return str + i;
}

Token *init_token(char *value, int type)
{
    Token *tok = (Token *) malloc(sizeof(Token));
    tok->value = value;
    tok->type = type;
    return tok;
}

Bool is_label(char *str)
{
    return TRUE;
}

Bool is_register(char *str)
{
    return TRUE;
}

Bool is_opcode(char *str)
{
    return TRUE;
}

List *get_list_of_tokens(char *line)
{
    List *token_list = create_list();
    while ()
}


void line_analyzer(char *line)
{
    while (TRUE)
    {
        line = skip_white_spaces(line);

    }
}