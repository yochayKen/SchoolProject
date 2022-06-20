#include <stdio.h>

#define MIN_NUM_OF_FILES 2

int validate_num_of_arguments(int, char *);

int main(int argc, char *argv[])
{
    char *original_file_content;
    int state_flag = 0;

    if (validate_num_of_arguments(argc, argv[0]) == 0)
        return 1;

    
    
    return 0;
}

int validate_num_of_arguments(int num_of_args, char *arguments)
{
    if (num_of_args < MIN_NUM_OF_FILES)
    {
        printf("error: insufficiant number of files\nUsage: %s <file_path_1> <file_path_2> ... <file_path_N>\n", arguments);
        return 0;
    }
    return 1;
}