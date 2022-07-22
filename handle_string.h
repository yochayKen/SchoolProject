#ifndef _HANDLE_STRING_H_
#define _HANDLE_STRING_H_

#include "utils.h"
#include "file_utils.h"
#include "list.h"

typedef struct line_info{
    unsigned int line_number;
    unsigned int num_of_characters;
    char *line_content;
}LineInfo;

/*=== API ===*/
LineInfo *get_line(const char *, unsigned int);
char *skip_whitespaces(char *);
char *get_nth_substring(char *, int);
Bool is_start_with(const char *, char);
Bool is_end_with(const char *, char);
Bool is_word_exists(const char *,const char *);
List *convert_file_lines_to_list(File *);
char *convert_list_to_file_lines(List *);

#endif
