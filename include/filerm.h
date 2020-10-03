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

// Append file to an array, checking if it is hidden
dynamic_array* append_if_not_hidden(const char* directory, const char* file, bool allow_hidden, dynamic_array* array);

// Append multiple files to an array, checking if they are hidden and match pattern
dynamic_array* append_files(const char* directory, dynamic_array* files, dynamic_array* array, parameters params);


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
