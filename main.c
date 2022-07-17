#include <stdio.h>
#include "file_utils.h"
#include "handle_string.h"
#include "utils.h"
#include "list.h"

#define MIN_NUM_OF_FILES 2

int validate_num_of_arguments(int, char *);

void print_list_vars(void *value)
{
    LineInfo *li;
    li = (LineInfo *)value;
    printf("%d %s\n",li->line_number, li->line_content);
}

int main(int argc, char *argv[])
{
    /*Aggregation of all the original file content*/
    File *file;
    List *l;

    if (validate_num_of_arguments(argc, argv[0]) == TRUE)
        return TRUE;

    file = read_file(argv[1]);
    l = convert_file_lines_to_list(file);
    printf("Printing list:\n");
    print_list(l, print_list_vars);
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