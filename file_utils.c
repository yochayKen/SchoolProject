#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "file_utils.h"

/*Specified at the workbook*/
#define NUM_OF_CHARS 100

void read_file_content(char *file_content, FILE *file)
{
    char c;
    int i = 1;
    while ((c = fgetc(file)) != EOF)
    {
        file_content[i-1] = c;
        file_content[i] = '\0';
        i++;
    }
}


/*Handling writing content to a file*/
int handle_write_file(char *file_name, char *content)
{
    return 0;
}


/*Handling reading the content of a file*/
int handle_read_file(char *file_path, char **file_content)
{
    FILE *fp = NULL;
    *file_content = (char *)malloc(NUM_OF_CHARS);
    if ((fp = fopen(file_path, "r")) == NULL)
    {
        printf("Could not open file: %s\n", file_path);
        return 1;
    }

    read_file_content(*file_content, fp);
   
    fclose(fp);
    return 0;
}