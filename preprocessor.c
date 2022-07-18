#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "file_utils.h"
#include "handle_string.h"
#include "list.h"

#define START_MACRO "macro"
#define END_MACRO "endmacro"
#define MACRO_NAME_ARG_POSITION 2
#define BUFFER_SIZE 50

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

Bool validate_macros_decleration(List *macros)
{
    return TRUE;
}

void convert_macro_declerations(List *file_content_list, List *macros)
{
    return;
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
            macro_name = get_macro_name(line_info->line_content);
            tmp = create_macro_instance(macro_name, line_info->line_number);
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
                append_to_list(tmp->command_lines, (void *)line_info);

        }
        line_info = (LineInfo *) get_next_elemet(file_content_list);
    }
    return macros;
}

void print_content(void *content)
{
    LineInfo *l = (LineInfo *)content;
    printf("Command: %s\n", l->line_content);
}

void print_macros(void *macro)
{
    MacroInfo *m = (MacroInfo *)macro;
    printf("Macro name is: %s declared at line %d\n", m->macro_name, m->decleration_line);
    print_list(m->command_lines, print_content);
}

Bool start_preprocess_stage(File *file)
{
    List *file_content_list = convert_file_lines_to_list(file);
    List *macros = search_for_macros(file_content_list);
    print_list(macros, print_macros);
    return TRUE;
}

/*
if (macros == NULL)
{
file->file_content = convert_list_to_file_lines(file_content_list);
return TRUE;
}

if (validate_macros_decleration(macros) == FALSE)
return FALSE;

convert_macro_declerations(file_content_list, macros);
file->file_content = convert_list_to_file_lines(file_content_list);
return TRUE;
 */