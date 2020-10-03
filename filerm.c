#include "include/filerm.h"
#include <string.h>
#include <regex.h>
#include <dirent.h>
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

char* join_path(const char* path1, const char* path2)
{
    while (*path2 == '/')
    {
        path2++;
    }
    char* dest = malloc(strlen(path1) + strlen(path2));
    if (!dest)
    {
        fprintf(stderr, "%s", "An error occurred while allocating memory! Exiting\n");
        exit(1);
    }
    strcpy(dest, path1);
    for (char* searcher = dest + strlen(path1) - 1; searcher >= path1 && *searcher == '/'; searcher--)
    {
        *searcher = 0;
    }
    strcat(dest, "/");
    strcat(dest, path2);

    return dest;
}

dynamic_array* append_if_not_hidden(const char* directory, const char* file, bool allow_hidden, dynamic_array* array)
{
    if (file[0] != '.' || allow_hidden)
    {
        return array_append_value(array, join_path(directory, file));
    }
    return array;
}

dynamic_array* append_files(const char* directory, dynamic_array* files, dynamic_array* array, parameters params)
{
    for (int i = 0; i < array_get_size(files); i++)
    {
        const char* file = *array_get_by_index(files, i);
        if (params.pattern)
        {
            regex_t pattern;
            if (!regcomp(&pattern, params.pattern, 0))
            {
                fprintf(stderr, "%s", "Incorret pattern given! Exiting\n");
                exit(1);
            }
            if (regexec(&pattern, file, 0, NULL, 0))
            {
                return array;
            }
        }
        array = append_if_not_hidden(directory, file, params.hidden, array);
    }
    return array;
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
                else if (!strcmp(argv[param], "--recursively"))
                {
                    params.recursively = true;
                }
                else if (!strcmp(argv[param], "--hidden"))
                {
                    params.hidden = true;
                }
                else if (!strcmp(argv[param], "--pattern"))
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
                else if (param < argc - 1)
                {
                    exit_on_syntax_error();
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


    array_delete_each(files_to_delete);
    array_delete(files_to_delete);
    return 0;
}
