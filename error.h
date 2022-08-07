#ifndef _ERROR_H_
#define _ERROR_H_

typedef enum error_value{
    INVALID_MACRO_DECLERATION,
    INVALID_SYMBOL_DECLERATION,
    SYMBOL_ALREADY_DECLARED,
    UNKNOWN_FILE_EXTENSION
}ErrorValue;

/*=== API ===*/
void declare_an_error(ErrorValue, unsigned int);
void update_current_file_name(char *);

#endif
