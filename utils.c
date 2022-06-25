#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define CHAR_BUFFER_SIZE 128

char *copy_string(char *str)
{
    int len = strlen(str) + 1; /*Plus 1 to fit the size of the buffer*/
    char *buffer = (char *)malloc(len);
    return strcpy(buffer, str);
}

List *convert_string_to_list(char *str, char delimiter)
{
    char *buffer = (char *)malloc(CHAR_BUFFER_SIZE);
    int i = 0, j = 0;
    List *list;

    list = create_list();
    while ((buffer[i] = str[j]) != '\0')
    {
        if ((buffer[i] == delimiter) && (strlen(buffer) != 0))
        {
            buffer[i] = '\0';
            append_to_list(list, copy_string(buffer));
            memset(buffer, 0, CHAR_BUFFER_SIZE);
            i = 0;
        }
        else
        {
            i++;
        }
        j++;
    }
    buffer[i] = '\0';
    append_to_list(list, copy_string(buffer)); /*Edge case for the last line*/
    return list;
}