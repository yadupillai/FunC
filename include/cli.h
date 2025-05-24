#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Command line modes
typedef enum {
    MODE_COMPRESS,
    MODE_DECOMPRESS,
    MODE_ENCRYPT,
    MODE_DECRYPT,
    MODE_SEARCH,
    MODE_SORT,
    MODE_HELP,
    MODE_INVALID
} Mode;

// Command line options structure
typedef struct {
    Mode mode;
    char* input_file;
    char* output_file;
    char* key;
    char* search_term;
} Options;

// Function declarations
Options* parse_cli(int argc, char** argv);
void free_options(Options* opts);
void print_help(void);

#endif // CLI_H 