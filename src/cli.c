#include "../include/cli.h"
#include "../include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Options* parse_cli(int argc, char** argv) {
    if (argc < 2) {
        print_help();
        return NULL;
    }

    Options* opts = malloc(sizeof(Options));
    if (!opts) {
        handle_memory_error();
        return NULL;
    }

    // Initialize options
    opts->mode = MODE_INVALID;
    opts->input_file = NULL;
    opts->output_file = NULL;
    opts->key = NULL;
    opts->search_term = NULL;
    opts->debug_mode = 0;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--compress") == 0) {
            opts->mode = MODE_COMPRESS;
        } else if (strcmp(argv[i], "--decompress") == 0) {
            opts->mode = MODE_DECOMPRESS;
        } else if (strcmp(argv[i], "--encrypt") == 0) {
            opts->mode = MODE_ENCRYPT;
        } else if (strcmp(argv[i], "--decrypt") == 0) {
            opts->mode = MODE_DECRYPT;
        } else if (strcmp(argv[i], "--search") == 0) {
            opts->mode = MODE_SEARCH;
        } else if (strcmp(argv[i], "--sort") == 0) {
            opts->mode = MODE_SORT;
        } else if (strcmp(argv[i], "--help") == 0) {
            opts->mode = MODE_HELP;
        } else if (strcmp(argv[i], "--debug") == 0) {
            opts->debug_mode = 1;
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            opts->input_file = strdup(argv[++i]);
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            opts->output_file = strdup(argv[++i]);
        } else if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            opts->key = strdup(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            opts->search_term = strdup(argv[++i]);
        }
    }

    // Validate required options
    if (opts->mode != MODE_HELP && !opts->input_file) {
        handle_error("Input file is required");
        free_options(opts);
        return NULL;
    }

    if ((opts->mode == MODE_COMPRESS || opts->mode == MODE_ENCRYPT || 
         opts->mode == MODE_DECRYPT || opts->mode == MODE_SORT) && !opts->output_file) {
        handle_error("Output file is required for this mode");
        free_options(opts);
        return NULL;
    }

    if ((opts->mode == MODE_ENCRYPT || opts->mode == MODE_DECRYPT) && !opts->key) {
        handle_error("Encryption key is required");
        free_options(opts);
        return NULL;
    }

    if (opts->mode == MODE_SEARCH && !opts->search_term) {
        handle_error("Search term is required");
        free_options(opts);
        return NULL;
    }

    return opts;
}

void free_options(Options* opts) {
    if (opts) {
        free(opts->input_file);
        free(opts->output_file);
        free(opts->key);
        free(opts->search_term);
        free(opts);
    }
}

void print_help(void) {
    printf("File Processor - Data Compression and Encryption Tool\n\n");
    printf("Usage: file_processor [options]\n\n");
    printf("Options:\n");
    printf("  --compress      Compress input file\n");
    printf("  --decompress    Decompress input file\n");
    printf("  --encrypt       Encrypt input file\n");
    printf("  --decrypt       Decrypt input file\n");
    printf("  --search        Search in input file\n");
    printf("  --sort          Sort lines in input file\n");
    printf("  --help          Show this help message\n");
    printf("  --debug         Enable debug mode\n");
    printf("  -i <file>       Input file\n");
    printf("  -o <file>       Output file\n");
    printf("  -k <key>        Encryption key\n");
    printf("  -s <term>       Search term\n\n");
    printf("Examples:\n");
    printf("  file_processor --compress -i input.txt -o output.rle\n");
    printf("  file_processor --encrypt -i input.txt -o output.enc -k secret\n");
    printf("  file_processor --search -i input.txt -s keyword\n");
} 