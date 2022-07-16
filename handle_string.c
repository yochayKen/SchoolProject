#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "file_utils.h"
#include "handle_string.h"
#include "list.h"

#define BUFFER_SIZE 81

LineInfo *init_line_info(char *line, unsigned int line_number)
{
    LineInfo *line_info = (LineInfo *) malloc(sizeof (LineInfo));
    line_info->line_number = line_number;
    line_info->line_content = line;
    return line_info;
}

char *skip_white_spaces(char *str)
{
    int i = 0;
    while ((str[i] != ' ') && (str[i] != '\t') && (str[i] != EOF))
        i++;
    return str + i;
}

Bool is_empty_line(char *line)
{
    char *ptr;
    ptr = skip_white_spaces(line);
    if(*ptr == '\n')
        return TRUE;
    return FALSE;
}

Bool is_word_exists(char *line, char *word)
{
    char *ret;

    ret = strstr(line, word);
    if (ret)
        return TRUE;
    return FALSE;
}

void copy_line(const char *str, char *buffer, int i)
{
    char *tmp = (char *) calloc(0, sizeof(BUFFER_SIZE));
    int j = 1;
    while (TRUE)
    {
        if (str[i] == '\0')
            break;
        else if (str[i] == '\n')
        {
            tmp[j - 1] = str[i];
            tmp[j] = '\0';
            break;
        }
        i++;
        j++;
    }
    strncpy(buffer, tmp, j);
}

LineInfo *get_line(const char *str, unsigned int line_number)
{
    char *buffer = (char *) calloc(0, sizeof(BUFFER_SIZE));
    int i = 0, current_line = 1;

    while (TRUE)
    {
        if (current_line == line_number)
        {
            copy_line(str, buffer, i);
            printf("%s\n", buffer);
            return init_line_info(buffer, current_line);
        }
        if (str[i] == '\n')
            current_line++;
        i++;
    }
}

List *convert_file_lines_to_list(File *file)
{
    List *line_list = create_list();
    unsigned int num_of_lines = file->num_of_lines;
    LineInfo *line_info;
    int i;

    for (i = 1; i < num_of_lines; i++)
    {
        line_info = get_line(file->file_content, i);
        append_to_list(line_list, (void *) line_info);
    }
    return line_list;
}