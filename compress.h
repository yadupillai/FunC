#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RLE compression functions
char* rle_compress(const char* input, size_t input_len, size_t* output_len);
char* rle_decompress(const char* input, size_t input_len, size_t* output_len);

// Linked list node for RLE
typedef struct RLE_Node {
    char character;
    int count;
    struct RLE_Node* next;
} RLE_Node;

// Linked list RLE functions
RLE_Node* create_rle_list(const char* input, size_t input_len);
char* rle_list_to_string(RLE_Node* head, size_t* output_len);
void free_rle_list(RLE_Node* head);

#endif // COMPRESS_H 