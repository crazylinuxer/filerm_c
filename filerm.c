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
    parameters params = {
        .help = false,
        .recursively = false,
        .hidden = false,
        .pattern = NULL,
        .directory = "./"
    };

    dynamic_array* files_to_delete = array_init();

    bool skip = false;
    for (int param = 1; param < argc; param++)
    {
        if (skip)
        {
            param++;
            skip = false;
        }
        if (argv[param][0] == '-')
        {
            if (strlen(argv[param]) == 1)
            {
                if (param >= argc - 1)
                {
                    params.directory = argv[param];
                }
                else
                {
                    exit_on_syntax_error();
                }
            }
            else if (argv[param][1] == '-')
            {
                if (!strcmp(argv[param], "--help"))
                {
                    params.help = true;
                }
                if (!strcmp(argv[param], "--recursively"))
                {
                    params.recursively = true;
                }
                if (!strcmp(argv[param], "--hidden"))
                {
                    params.hidden = true;
                }
                if (!strcmp(argv[param], "--pattern"))
                {
                    if (param >= argc - 1)
                    {
                        params.directory = argv[param];
                    }
                    else if (param + 1 <= argc)
                    {
                        params.pattern = argv[param];
                        skip = true;
                    }
                    else
                    {
                        exit_on_syntax_error();
                    }
                }
            }
            else
            {
                for (int letter = 1; letter < strlen(argv[param]); letter++)
                {
                    if (argv[param][letter] == 'h')
                    {
                        params.help = true;
                    }
                    else if (argv[param][letter] == 'r')
                    {
                        params.recursively = true;
                    }
                    else if (argv[param][letter] == 'i')
                    {
                        params.hidden = true;
                    }
                    else if (argv[param][letter] == 'p')
                    {
                        if (param + 1 <= argc)
                        {
                            params.pattern = argv[param];
                            skip = true;
                        }
                        else
                        {
                            exit_on_syntax_error();
                        }
                    }
                    else
                    {
                        exit_on_syntax_error();
                    }
                }
            }
        }
        else if (param >= argc - 1)
        {
            params.directory = argv[param];
        }
        else
        {
            exit_on_syntax_error();
        }
    }

    if (params.help)
    {
        print_help();
        exit(0);
    }

    array_delete(files_to_delete);
    return 0;
}
