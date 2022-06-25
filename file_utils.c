#include <stdio.h>
#include <stdlib.h>
#include "file_utils.h"

#define NUM_OF_CHARS 256

char *read_file_content(FILE *file)
{
    char c;
    char *file_content = (char *)malloc(NUM_OF_CHARS);
    int i = 1, chunks = 1;
    while ((c = fgetc(file)) != EOF)
    {
        file_content[i - 1] = c;
        file_content[i] = '\0';
        if (i == (chunks * NUM_OF_CHARS))
        {
            chunks++;
            file_content = (char *)realloc(file_content, chunks * NUM_OF_CHARS);
        }
        i++;
    }
    return file_content;
}


/*Writing content to a file*/
int write_file(char *file_name, char *content)
{
    return 0;
}


/*Reading the content from a file*/
char *read_file(char *file_path)
{
    FILE *fp;
    char *content;
    if ((fp = fopen(file_path, "r")) == NULL)
    {
        printf("Could not open file: %s\n", file_path);
        return NULL;
    }
    content = read_file_content(fp);
    fclose(fp);
    return content;
}