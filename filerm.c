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

void* check_pointer_after_malloc(void* ptr)
{
    if (!ptr)
    {
        fprintf(stderr, "%s", "An error occurred while allocating memory! Exiting\n");
        exit(1);
    }
    return ptr;
}

char* join_path(const char* path1, const char* path2)
{
    while (*path2 == '/')
    {
        path2++;
    }
    char* dest = malloc(strlen(path1) + strlen(path2) + 2);
    check_pointer_after_malloc(dest);
    strcpy(dest, path1);
    for (char* searcher = dest + strlen(path1) - 1; searcher >= path1 && *searcher == '/'; searcher--)
    {
        *searcher = 0;
    }
    int len_path1 = strlen(dest);
    if (len_path1 && dest[len_path1 - 1] != '/')
    {
        strcat(dest, "/");
    }
    strcat(dest, path2);

    return dest;
}

dynamic_array* append_files(const char* directory, dynamic_array* files, dynamic_array* array, parameters params)
{
    for (int i = 0; i < array_get_size(files); i++)
    {
        const char* file = *(const char**)array_get_by_index(files, i);
        if (params.pattern)
        {
            regex_t pattern;
            if (!regcomp(&pattern, params.pattern, 0))
            {
                fprintf(stderr, "%s", "Incorret pattern given! Exiting\n");
                exit(1);
            }
            if (!regexec(&pattern, file, 0, NULL, 0))
            {
                array = array_append_value(array, (uint64_t)join_path(directory, file));
            }
            regfree(&pattern);
        }
    }
    return array;
}

dynamic_array* list_files_of_type(const char* directory, bool allow_hidden, uint8_t type)
{
    DIR *dir;
    struct dirent *entity;
    dir = opendir(directory);
    dynamic_array* result = array_init();
    if (dir)
    {
        while (1)
        {
            entity = readdir(dir);
            if (!entity)
            {
                break;
            }
            if (entity->d_type == type && strcmp(entity->d_name, ".") && strcmp(entity->d_name, ".."))
            {
                if (entity->d_name[0] != '.' || allow_hidden)
                {
                    result = array_append_value(result, (uint64_t)join_path(directory, entity->d_name));
                }
            }
        }
        closedir(dir);
    }
    return result;
}

dynamic_array* list_directories(const char* directory, bool allow_hidden)
{
    return list_files_of_type(directory, allow_hidden, 4);
}

dynamic_array* list_files(const char* directory, bool allow_hidden)
{
    return list_files_of_type(directory, allow_hidden, 8);
}


dynamic_array* list_files_recursively(const char* directory, dynamic_array* result, bool allow_hidden)
{
    if (!result)
    {
        result = array_init();
    }

    dynamic_array* files = list_files(directory, allow_hidden);
    result = array_extend(result, files);
    array_delete(files);

    dynamic_array* directories = list_directories(directory, allow_hidden);
    for (int i = 0; i < array_get_size(directories); i++)
    {
        result = list_files_recursively(*(char**)array_get_by_index(directories, i), result, allow_hidden);
        free(*(char**)array_get_by_index(directories, i));
    }
    array_delete(directories);
    
    return result;
}

void print_array_of_strings(dynamic_array* array)
{
    for (int i = 0; i < array_get_size(array); i++)
    {
        printf("%s\n", *(char**)array_get_by_index(array, i));
    }
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

    bool skip = false;
    for (int param = 1; param < argc; param++)
    {
        if (skip)
        {
            param++;
            skip = false;
            if (param >= argc)
            {
                break;
            }
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

    dynamic_array* files_to_delete;

    if (params.recursively)
    {
        files_to_delete = list_files_recursively(params.directory, NULL, params.hidden);
    }
    else
    {
        files_to_delete = list_files(params.directory, params.hidden);
    }

    printf("%s\n", "The following files will be removed:");
    print_array_of_strings(files_to_delete);
    printf("%s", "Proceed? [y/N] ");
    char answer = 0;
    scanf("%c", &answer);
    if (answer == 'y' || answer == 'Y')
    {
        for (int i = 0; i < array_get_size(files_to_delete); i++)
        {
            char* filename = *(char**)array_get_by_index(files_to_delete, i);
            if (remove(filename))
            {
                fprintf(stderr, "Failed to remove %s\n", filename);
            }
        }
        printf("%s\n", "Done!");
    }
    else
    {
        printf("%s\n", "Cancelled");
    }
    
    array_delete_each(files_to_delete);
    array_delete(files_to_delete);
    return 0;
}
