#include "../include/cli.h"
#include "../include/compress.h"
#include "../include/encrypt.h"
#include "../include/io.h"
#include "../include/search.h"
#include "../include/sort.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    // Parse command line arguments
    Options* opts = parse_cli(argc, argv);
    if (!opts) {
        return 1;
    }

    if (opts->mode == MODE_HELP) {
        print_help();
        free_options(opts);
        return 0;
    }

    // Read input file
    size_t input_size;
    char* input_data = read_file(opts->input_file, &input_size);
    if (!input_data) {
        free_options(opts);
        return 1;
    }

    size_t output_size;
    char* output_data = NULL;
    int result = 0;

    // Process based on mode
    switch (opts->mode) {
        case MODE_COMPRESS:
            output_data = huffman_compress(input_data, input_size, &output_size);
            break;
        case MODE_DECOMPRESS:
            output_data = huffman_decompress(input_data, input_size, &output_size);
            break;
        case MODE_ENCRYPT:
            output_data = xor_encrypt(input_data, input_size, opts->key, &output_size);
            break;
        case MODE_DECRYPT:
            output_data = xor_decrypt(input_data, input_size, opts->key, &output_size);
            break;
        case MODE_SEARCH: {
            SearchResult* results = search_text(input_data, opts->search_term);
            if (results) {
                print_search_results(results);
                free_search_results(results);
            }
            break;
        }
        case MODE_SORT: {
            LineArray* lines = split_into_lines(input_data);
            if (lines) {
                sort_lines(lines);
                output_data = join_lines(lines);
                output_size = strlen(output_data) + 1;
                free_line_array(lines);
            }
            break;
        }
        default:
            handle_error("Invalid mode");
            result = 1;
            break;
    }

    // Write output if applicable
    if (output_data && opts->output_file) {
        if (!write_file(opts->output_file, output_data, output_size)) {
            result = 1;
        }
        free(output_data);
    }

    // Cleanup
    free(input_data);
    free_options(opts);

    return result;
}
