#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "file_utils.h"
#include "handle_string.h"
#include "list.h"

LineInfo *init_line_info(char *line, unsigned int line_number)
{
    LineInfo *line_info = (LineInfo *) malloc(sizeof (LineInfo));
    line_info->line_number = line_number;
    line_info->line_content = line;
    return line_info;
}

Bool is_start_with(const char *str, char prefix)
{
    if (str[0] == prefix)
        return TRUE;
    return FALSE;
}

Bool is_end_with(const char *str, char postfix)
{
    size_t str_len = strlen(str);
    if (str[str_len] == postfix)
        return TRUE;
    return FALSE;
}

Bool is_word_exists(const char *line,const char *word)
{
    char *ret;

    ret = strstr(line, word);
    if (ret)
        return TRUE;
    return FALSE;
}

List *convert_file_lines_to_list(File *file)
{
    List *line_list = create_list();
    const char *current_line = file->file_content;
    unsigned int line_number = 1;

    while(current_line)
    {
        const char *next_line = strchr(current_line, '\n');
        size_t current_line_len = next_line ? (next_line - current_line) : strlen(current_line);
        char *tmp = (char *)malloc(current_line_len + 1);
        memcpy(tmp, current_line, current_line_len);
        tmp[current_line_len] = '\0';
        append_to_list(line_list, (void *) init_line_info(tmp, line_number++));
        current_line = next_line ? (next_line + 1) : NULL;
    }
    return line_list;
}