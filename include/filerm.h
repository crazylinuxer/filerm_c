#include <stdbool.h>
#include "dynamic_array.h"

typedef struct
{
    bool help;
    bool recursively;
    bool hidden;
    const char* pattern;
    const char* directory;
}
parameters;

void print_help(void);
void exit_on_syntax_error(void);

// This function returns ptr to allocated memory that needs to be freed later
char* join_path(const char* path1, const char* path2);

// Append multiple files to an array, checking if they are hidden and match pattern
dynamic_array* append_files(const char* directory, dynamic_array* files, dynamic_array* array, parameters params);

// Checks the pointer and exits program if in is null
void* check_pointer_after_malloc(void* ptr);

// List files in the directiry filtering by given d_type
dynamic_array* list_files_of_type(const char* directory, bool allow_hidden, uint8_t type);

// List directories in the directory
dynamic_array* list_directories(const char* directory, bool allow_hidden);

// List files (without directories, symlinks etc.) in the directory
dynamic_array* list_files(const char* directory, bool allow_hidden);

// List files (without directories, symlinks etc.) in the directory and its subdirectories recursively
dynamic_array* list_files_recursively(const char* directory, dynamic_array* result, bool allow_hidden);

// Print the dynamic array using each item like char*
void print_array_of_strings(dynamic_array*);


const char* help = R"(filerm.py - utility that removes only files but does not affect directories

usage:
    python3 filerm.py [-rhi] [-p PATTERN] [DIRECTORY]

    DIRECTORY is "./" by default

flags:
    -r, --recursively
        remove files from all child directories

    -h, --help
        print this help message

    -i, --hidden
        affect hidden files (remove files that begin with '.')

    -p, --pattern
        simple pattern that can match part of file name or the whole file name,
        for example pattern "[a-z, A-Z, 0-9]*.jpeg" will affect only files that end with ".jpeg",
        and pattern "non_important_file.txt" will affect only files named "non_important_file.txt"
)";
