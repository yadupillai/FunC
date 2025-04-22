#include "compress.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* rle_compress(const char* input, size_t input_len, size_t* output_len) {
    if (!input || input_len == 0) {
        handle_error("Invalid input for compression");
        return NULL;
    }

    // Allocate worst-case output buffer (2x input size)
    char* output = malloc(input_len * 2 + 1);
    if (!output) {
        handle_memory_error();
        return NULL;
    }

    size_t output_pos = 0;
    char current_char = input[0];
    int count = 1;

    for (size_t i = 1; i < input_len; i++) {
        if (input[i] == current_char && count < 9) {
            count++;
        } else {
            // Write compressed sequence
            output[output_pos++] = current_char;
            if (count > 1) {
                output[output_pos++] = '0' + count;
            }
            current_char = input[i];
            count = 1;
        }
    }

    // Write last sequence
    output[output_pos++] = current_char;
    if (count > 1) {
        output[output_pos++] = '0' + count;
    }

    output[output_pos] = '\0';
    *output_len = output_pos;

    // Reallocate to exact size
    char* final_output = realloc(output, output_pos + 1);
    if (!final_output) {
        handle_memory_error();
        free(output);
        return NULL;
    }

    return final_output;
}

char* rle_decompress(const char* input, size_t input_len, size_t* output_len) {
    if (!input || input_len == 0) {
        handle_error("Invalid input for decompression");
        return NULL;
    }

    // Calculate output size
    size_t max_output_size = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (i + 1 < input_len && input[i + 1] >= '2' && input[i + 1] <= '9') {
            max_output_size += input[i + 1] - '0';
            i++;
        } else {
            max_output_size++;
        }
    }

    // Allocate output buffer
    char* output = malloc(max_output_size + 1);
    if (!output) {
        handle_memory_error();
        return NULL;
    }

    size_t output_pos = 0;
    for (size_t i = 0; i < input_len; i++) {
        char current_char = input[i];
        int count = 1;

        if (i + 1 < input_len && input[i + 1] >= '2' && input[i + 1] <= '9') {
            count = input[i + 1] - '0';
            i++;
        }

        for (int j = 0; j < count; j++) {
            output[output_pos++] = current_char;
        }
    }

    output[output_pos] = '\0';
    *output_len = output_pos;

    // Reallocate to exact size
    char* final_output = realloc(output, output_pos + 1);
    if (!final_output) {
        handle_memory_error();
        free(output);
        return NULL;
    }

    return final_output;
}

RLE_Node* create_rle_list(const char* input, size_t input_len) {
    if (!input || input_len == 0) {
        return NULL;
    }

    RLE_Node* head = NULL;
    RLE_Node* current = NULL;
    char prev_char = input[0];
    int count = 1;

    for (size_t i = 1; i < input_len; i++) {
        if (input[i] == prev_char) {
            count++;
        } else {
            RLE_Node* node = malloc(sizeof(RLE_Node));
            if (!node) {
                handle_memory_error();
                free_rle_list(head);
                return NULL;
            }

            node->character = prev_char;
            node->count = count;
            node->next = NULL;

            if (!head) {
                head = node;
                current = node;
            } else {
                current->next = node;
                current = node;
            }

            prev_char = input[i];
            count = 1;
        }
    }

    // Add last node
    RLE_Node* node = malloc(sizeof(RLE_Node));
    if (!node) {
        handle_memory_error();
        free_rle_list(head);
        return NULL;
    }

    node->character = prev_char;
    node->count = count;
    node->next = NULL;

    if (!head) {
        head = node;
    } else {
        current->next = node;
    }

    return head;
}

char* rle_list_to_string(RLE_Node* head, size_t* output_len) {
    if (!head) {
        return NULL;
    }

    // Calculate output size
    size_t size = 0;
    RLE_Node* current = head;
    while (current) {
        size += (current->count > 1) ? 2 : 1;
        current = current->next;
    }

    // Allocate output buffer
    char* output = malloc(size + 1);
    if (!output) {
        handle_memory_error();
        return NULL;
    }

    // Fill output buffer
    size_t pos = 0;
    current = head;
    while (current) {
        output[pos++] = current->character;
        if (current->count > 1) {
            output[pos++] = '0' + current->count;
        }
        current = current->next;
    }

    output[pos] = '\0';
    *output_len = pos;
    return output;
}

void free_rle_list(RLE_Node* head) {
    while (head) {
        RLE_Node* next = head->next;
        free(head);
        head = next;
    }
} 