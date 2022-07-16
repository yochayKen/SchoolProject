#include <stdio.h>
#include "file_utils.h"
#include "handle_string.h"
#include "utils.h"

#define MIN_NUM_OF_FILES 2

int validate_num_of_arguments(int, char *);

int main(int argc, char *argv[])
{
    /*Aggregation of all the original file content*/
    char *original_file_content;
    char *new_char;

    /*Flag state which will change if something went wrong*/
    int state_flag = 0;

    if (validate_num_of_arguments(argc, argv[0]) == 1)
        return TRUE;

    original_file_content = read_file(argv[1]);
    if ((new_char = get_line(original_file_content, 8)) == NULL)
    {
        printf("Line number does not exists\n");
        return TRUE;
    }
    printf("%s\n", new_char);
    if (is_word_exists(new_char, "mov") == TRUE)
    {
        printf("Found word: \"mov\"\n");
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