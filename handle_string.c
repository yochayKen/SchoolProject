#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "file_utils.h"
#include "handle_string.h"
#include "list.h"

#define BUFFER_SIZE 64

LineInfo *init_line_info(char *line, unsigned int line_number, unsigned int num_of_chars)
{
    LineInfo *line_info = (LineInfo *) malloc(sizeof (LineInfo));
    line_info->line_number = line_number;
    line_info->num_of_characters = num_of_chars;
    line_info->line_content = line;
    return line_info;
}

char *skip_whitespaces(char *str)
{
    int i = 0;
    while (isspace(str[i]))
        i++;
    return str + i;
}

char *get_nth_substring(char *str, int n)
{
    int i = 0, j = 1, num_of_strs = 0;
    char *buffer = (char *) malloc(BUFFER_SIZE);
    while (str[i] != '\0')
    {
        memset(buffer, 0, BUFFER_SIZE);
        str = skip_whitespaces(str);
        while(!isspace(str[i]))
        {
            if (str[i] == '\0')
                break;
            buffer[j - 1] = str[i++];
            buffer[j++] = '\0';
        }
        num_of_strs++;
        if (num_of_strs == n)
            return buffer;
        else
        {
            j = 1;
        }
        i++;
    }
    return NULL;
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
        append_to_list(line_list, (void *) init_line_info(tmp, line_number++, current_line_len));
        current_line = next_line ? (next_line + 1) : NULL;
    }
    return line_list;
}

char *convert_list_to_file_lines(List *file_content_list)
{
    char *buffer;
    LineInfo *line_info = (LineInfo *) get_head_element(file_content_list);

    while (line_info != NULL)
    {
        buffer = (char *) malloc(sizeof (line_info->num_of_characters));
        memcpy(buffer, line_info->line_content, line_info->num_of_characters);
        line_info = (LineInfo *) get_next_element(file_content_list);
    }
    delete_list(file_content_list);
    return buffer;
}
