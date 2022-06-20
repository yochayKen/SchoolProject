#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
    FILE *fptr;
    char buffer[1000];

    fptr = fopen("/tests/table_test.py", "r");
    fread(buffer, 1000, 1, fptr);
    printf("%s\n", buffer);
    

    return 0;
}