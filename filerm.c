#include "include/filerm.h"
#include "include/dynamic_array.h"
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>


void print_help(void)
{
    printf("%s", help);
}

void exit_on_syntax_error(void)
{
    fprintf(stderr, "%s", "Invalid command line parameters detected!\n");
    printf("\n");
    print_help();
    exit(1);
}


int main(int argc, char** argv)
{
    parameters params;
    dynamic_array* files_to_delete = array_init();


    array_delete(files_to_delete);
    return 0;
}
