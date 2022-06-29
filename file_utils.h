#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

/* Types*/
typedef struct file_desc{
    char *file_name;
    char *file_content;
}File;

/* API */
char *read_file(char *);
int write_file(File *);

#endif