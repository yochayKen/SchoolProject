#ifndef _HANDLE_STRING_H_
#define _HANDLE_STRING_H_

#include "utils.h"
#include "file_utils.h"
#include "list.h"

typedef struct line_info{
    unsigned int line_number;
    char *line_content;
}LineInfo;

/*=== API ===*/
char *skip_white_spaces(char *);
LineInfo *get_line(const char *, unsigned int);
Bool is_word_exists(char *, char *);
List *convert_file_lines_to_list(File *);

#endif
