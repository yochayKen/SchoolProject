#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "file_utils.h"
#include "handle_string.h"
#include "list.h"
#include "error.h"

#define START_MACRO "macro"
#define END_MACRO "endmacro"

#define MACRO_NAME_ARG_POSITION 2
#define MACRO_DECLERATION_POSITION 1
#define BUFFER_SIZE 80

typedef struct macro_info{
    char *macro_name;
    unsigned int decleration_line;
    List *command_lines;
}MacroInfo;

MacroInfo *create_macro_instance(char *macro_name, unsigned int line)
{
    MacroInfo *macro = (MacroInfo *) malloc(sizeof (MacroInfo));
    macro->macro_name = macro_name;
    macro->decleration_line = line;
    macro->command_lines = create_list();
    return macro;
}

int compare_macro_lines(void *line_info, void *current_line_info)
{
    LineInfo *li = (LineInfo *)line_info;
    LineInfo *cli = (LineInfo *)current_line_info;
    if (li->line_number == cli->line_number && strcmp(li->line_content, cli->line_content) == 0)
        return 0;
    return 1;
}

Bool validate_macros_decleration(List *file_content_linst, List *macros)
{
    LineInfo *line_info = (LineInfo *) get_head_element(file_content_linst);

    while (line_info != NULL)
    {
        MacroInfo *macro = (MacroInfo *) get_head_element(macros);
        while (macro != NULL)
        {
            if (strcmp(get_nth_substring(line_info->line_content, MACRO_DECLERATION_POSITION), macro->macro_name) == 0)
            {
                if (macro->decleration_line > line_info->line_number)
                {
                    declare_an_error(INVALID_MACRO_DECLERATION, line_info->line_number);
                    return FALSE;
                }
            }
            macro = (MacroInfo *) get_next_element(macros);
        }
        line_info = (LineInfo *) get_next_element(file_content_linst);
    }
    return TRUE;
}

void convert_macro_declerations(List *file_content_list, List *macros)
{
    MacroInfo *macro_info = (MacroInfo *) get_head_element(macros);
    LineInfo *next_line_info;
    char *first_word;

    while (TRUE)
    {
        next_line_info = (LineInfo *) check_next_element_in_list(file_content_list);
        if (next_line_info == NULL)
            break;
        while (macro_info != NULL)
        {
            first_word = get_nth_substring(next_line_info->line_content, MACRO_DECLERATION_POSITION);
            if (strcmp(first_word, macro_info->macro_name) == 0)
            {
                insert_sublist_in_list(file_content_list->current_element_addr, macro_info->command_lines);
                break;
            }
            else
                macro_info = (MacroInfo *) get_next_element(macros);
        }
        macro_info = (MacroInfo *) get_head_element(macros);
        next_line_info = (LineInfo *) get_next_element(file_content_list);
    }
}

char *get_macro_name(char *line)
{
    return get_nth_substring(line, MACRO_NAME_ARG_POSITION);
}

List *search_for_macros(List *file_content_list)
{
    int is_macro = 0;
    List *macros = create_list();
    char *macro_name;
    MacroInfo *tmp;
    LineInfo *line_info = (LineInfo *) get_head_element(file_content_list);

    while (line_info != NULL)
    {
        if (is_word_exists(line_info->line_content, START_MACRO) == TRUE && is_macro == 0)
        {
            is_macro = 1;
            macro_name = (char *)malloc(BUFFER_SIZE);
            memcpy(macro_name, get_macro_name(line_info->line_content), BUFFER_SIZE);
            tmp = create_macro_instance(macro_name, line_info->line_number);
            remove_from_list(file_content_list, (void *)line_info, compare_macro_lines);
        }
        else if (is_macro == 1)
        {
            if (is_word_exists(line_info->line_content, END_MACRO) == TRUE)
            {
                MacroInfo *macro = (MacroInfo *) malloc(sizeof (MacroInfo));
                is_macro = 0;
                memcpy(macro, tmp, sizeof(MacroInfo));
                append_to_list(macros, (void *)macro);
                free(tmp);
            }
            else
            {
                LineInfo *new_line_info = (LineInfo *) malloc(sizeof(LineInfo));
                memcpy(new_line_info, line_info, sizeof (LineInfo));
                append_to_list(tmp->command_lines, (void *) new_line_info);
            }
            remove_from_list(file_content_list, (void *)line_info, compare_macro_lines);
        }
        line_info = (LineInfo *) get_next_element(file_content_list);
    }
    return macros;
}

Bool start_preprocess_stage(File *file)
{
    List *file_content_list = convert_file_lines_to_list(file);
    List *macros = search_for_macros(file_content_list);
    if (macros == NULL)
        return TRUE;

    if (validate_macros_decleration(file_content_list, macros) == FALSE)
        return FALSE;

    convert_macro_declerations(file_content_list, macros);
    free(file->file_content);
    file->file_content = convert_list_to_file_lines(file_content_list);
    return TRUE;
}
