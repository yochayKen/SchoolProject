#ifndef _ERROR_H_
#define _ERROR_H_

typedef enum error_value{
    INVALID_MACRO_DECLERATION,
    NONE
}ErrorValue;

/*=== API ===*/
void declare_an_error(ErrorValue, unsigned int);
void update_current_file_name(char *);

#endif
