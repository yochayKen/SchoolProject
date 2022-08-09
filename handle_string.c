#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "file_utils.h"
#include "handle_string.h"
#include "list.h"

#define BUFFER_SIZE 80
#define EXTENSION_CHARS 3
#define EXTENSION "as"

LineInfo *init_line_info(char *line, unsigned int line_number, unsigned int num_of_chars)
{
    LineInfo *line_info = (LineInfo *) malloc(sizeof (LineInfo));
    line_info->line_number = line_number;
    line_info->num_of_characters = num_of_chars;
    line_info->line_content = line;
    return line_info;
}

Bool check_file_extension(char *file_name)
{
    char buffer[EXTENSION_CHARS];
    int j, i = 0;
    while (file_name[i] != '\0')
    {
        if (file_name[i] == '.')
        {
            for (j = 0; j < EXTENSION_CHARS; j++)
                buffer[j] = file_name[++i];
            buffer[++j] = '\0';
            break;
        }
        i++;
    }
    if (strcmp(EXTENSION, buffer) == 0)
        return TRUE;
    return FALSE;
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
    Bool end_of_str = FALSE;
    char *buffer = (char *) malloc(BUFFER_SIZE);
    while (TRUE)
    {
        memset(buffer, 0, BUFFER_SIZE);
        if (!isspace(str[i]))
        {
            while(TRUE)
            {
                if (str[i] == '\0' || str[i] == '\n')
                {
                    end_of_str = TRUE;
                    break;
                }
                if (str[i] == ' ' || str[i] == '\t' || str[i] == ',')
                    break;
                buffer[j - 1] = str[i++];
                buffer[j++] = '\0';
            }
            num_of_strs++;
        }
        if (num_of_strs == n)
            return buffer;
        else
            j = 1;
        if (end_of_str == TRUE)
            break;
        i++;
    }
    return NULL;
}

void remove_last_char(char *str)
{
    int str_len = strlen(str);
    str[str_len - 1] = '\0';
}

char get_last_char(char *str)
{
    int str_len = strlen(str);
    return str[str_len - 1];
}

char get_first_char(char *str)
{
    return str[0];
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
    if (str[str_len - 1] == postfix)
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
    LineInfo *line_info = (LineInfo *) get_head_element(file_content_list);
    unsigned int total_num_of_chars = line_info->num_of_characters + 1, i = 0, j = 0;

    char *buffer = (char *)malloc(line_info->num_of_characters + 1);

    while (TRUE)
    {
        while (line_info->line_content[j] != '\0')
            buffer[i++] = line_info->line_content[j++];
        buffer[i] = '\n';
        if ((line_info = (LineInfo *) get_next_element(file_content_list)) == NULL)
        {
            buffer[i] = '\0';
            break;
        }
        i++;
        j = 0;
        total_num_of_chars += line_info->num_of_characters + 1;
        buffer = (char *)realloc(buffer, total_num_of_chars);
    }
    delete_list(file_content_list);
    return buffer;
}
