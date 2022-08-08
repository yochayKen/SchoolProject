#ifndef _ERROR_H_
#define _ERROR_H_

#include "utils.h"

typedef enum error_value{
    INVALID_MACRO_DECLERATION,
    INVALID_SYMBOL_DECLERATION,
    SYMBOL_ALREADY_DECLARED,
    UNKNOWN_FILE_EXTENSION,
    UNKNOWN_DECLARED_STR,
    NONE
}ErrorValue;

/*=== API ===*/
void declare_an_error(ErrorValue, unsigned int);
void update_current_file_name(char *);
void set_error();
Bool get_error();
void set_error_type(ErrorValue);
ErrorValue get_error_type();


#endif
