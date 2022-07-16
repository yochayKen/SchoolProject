#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

char *skip_white_spaces(char *str)
{
    int i = 0;
    while ((str[i] = ' ') || (str[i] == '\t') || (str[i] == EOF) || (str[i] == '\n'))
        i++;
    return str + i;
}

Bool is_word_exists(char *line, char *word)
{
    char *ret;

    ret = strstr(line, word);
    if (ret)
        return TRUE;
    return FALSE;
}

int calculate_new_line_offset(char *str, unsigned int line_number)
{
    unsigned int current_line = 1;
    int i = 0;

    if (current_line == line_number)
        return i;

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
            current_line++;

        if (current_line == line_number)
            return ++i;
        i++;
    }
    return -1;
}

char *get_line(char *str, unsigned int line_number)
{
    char *buffer;
    int i;
    int offset = calculate_new_line_offset(str, line_number);

    if (offset == -1)
        return NULL;
    str = str + offset;

    for (i = 0; (str[i] != '\n') && (str[i] != '\0'); i++);
    buffer = (char *) malloc(i);
    strncpy(buffer, str, i);
    buffer[i] = '\0';
    return buffer;
}