#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utils.h"

#define NUM_OF_CHARS 256

File *init_file_struct(char *file_name, char *file_content, size_t num_of_lines)
{
    File *file = (File *) malloc(sizeof (File));
    file->file_name = file_name;
    file->file_content = file_content;
    file->num_of_lines = num_of_lines;
    return file;
}

File *read_file_content(FILE *file, char *file_name)
{
    char c;
    size_t num_of_lines = 1;
    char *file_content = (char *)malloc(NUM_OF_CHARS);
    int i = 1, chunks = 1;
    while ((c = fgetc(file)) != EOF)
    {
        file_content[i - 1] = c;
        file_content[i] = '\0';
        if (file_content[i - 1] == '\n')
            num_of_lines++;
        if (i == (chunks * NUM_OF_CHARS))
        {
            chunks++;
            file_content = (char *)realloc(file_content, chunks * NUM_OF_CHARS);
        }
        i++;
    }
    return init_file_struct(file_name, file_content, num_of_lines);
}


/*Writing content to a file*/
int write_file(File *file)
{
    FILE *fp;
    size_t file_size = strlen(file->file_content);
    if ((fp = fopen(file->file_name, "w")) == NULL)
    {
        printf("Could not create file: %s\n", file->file_name);
        return -1;
    }
    fprintf(fp, file->file_content, file_size);
    fclose(fp);
    return 0;
}


/*Reading the content from a file*/
File *read_file(char *file_name)
{
    FILE *fp;
    File *file;
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        printf("Could not open file: %s\n", file_name);
        return NULL;
    }
    file = read_file_content(fp, file_name);
    fclose(fp);
    return file;
}