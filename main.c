#include <stdio.h>
#include "error.h"
#include "file_utils.h"
#include "handle_string.h"
#include "utils.h"
#include "list.h"
#include "preprocessor.h"
#include "first_stage.h"
#include "second_stage.h"

#define MIN_NUM_OF_FILES 2

int validate_num_of_arguments(int, char *);
void start_run_through_files(int ,char **);

int main(int argc, char *argv[])
{
    if (validate_num_of_arguments(argc, argv[0]) == TRUE)
        return TRUE;

    start_run_through_files(argc, argv);
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

/*Running through all the files given as an argument*/
void start_run_through_files(int argc, char *argv[])
{
    int i;
    File *current_file;
    List *symbol_table;
    
    for (i = 1; i < argc; i++)
    {
        current_file = read_file(argv[i]);
        update_current_file_name(current_file->file_name);
        symbol_table = create_list();

        if (check_file_extension(current_file->file_name) == FALSE)
        {
            declare_an_error(UNKNOWN_FILE_EXTENSION, 0);
            continue;
        }
        if (start_preprocess_stage(current_file) == FALSE)
            continue;
        else
            write_file(current_file);
        update_current_file_name(current_file->file_name);
        if (start_first_stage(current_file, symbol_table) == FALSE)
            continue;
        start_second_stage(current_file, symbol_table);
    }
}