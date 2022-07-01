#include <stdio.h>
#include <stdlib.h>
#include "file_utils.h"
#include "utils.h"

#define MIN_NUM_OF_FILES 2

int validate_num_of_arguments(int, char *);

int main(int argc, char *argv[])
{
    /*Aggregation of all the original file content*/
    char *original_file_content;

    /*Flag state which will change if something went wrong*/
    int state_flag = 0;

    if (validate_num_of_arguments(argc, argv[0]) == 1)
        return TRUE;

    int i;
    for (i = 1; i < argc; i++)
    {
        original_file_content = read_file(argv[i]);
        if (original_file_content != NULL)
            printf("Content: %s\n", original_file_content);
    }
    return FALSE;
}

/*Validate number of arguments passed into the program*/
int validate_num_of_arguments(int num_of_args, char *name_of_file)
{
    if (num_of_args < MIN_NUM_OF_FILES)
    {
        printf("error: insufficiant number of files\n");
        printf("Usage: %s <file_path_1> <file_path_2> ... <file_path_N>\n", name_of_file);
        return 1;
    }
    return 0;
}