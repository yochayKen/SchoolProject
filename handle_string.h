#ifndef _HANDLE_STRING_H_
#define _HANDLE_STRING_H_

#include "utils.h"

/*=== API ===*/
char *skip_white_spaces(char *);
char *get_line(char *, unsigned int);
Bool is_word_exists(char *, char *);

#endif
