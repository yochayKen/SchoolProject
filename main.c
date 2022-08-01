#include <stdio.h>
#include "file_utils.h"
#include "handle_string.h"
#include "utils.h"
#include "list.h"
#include "preprocessor.h"

#define MIN_NUM_OF_FILES 2

int validate_num_of_arguments(int, char *);

int main(int argc, char *argv[])
{
    /*Aggregation of all the original file content*/
    File *file;

    if (validate_num_of_arguments(argc, argv[0]) == TRUE)
        return TRUE;

    file = read_file(argv[1]);
    start_preprocess_stage(file);
    write_file(file);
    return FALSE;
}

/*Validate number of arguments passed into the program*/
int validate_num_of_arguments(int num_of_args, char *name_of_file)
{
    if (num_of_args < MIN_NUM_OF_FILES)
    {
        printf("error: insufficiant number of files\n");
        printf("Usage: %s <file_path_1> <file_path_2> ... <file_path_N>\n", name_of_file);
        return TRUE;
    }
    return FALSE;
}