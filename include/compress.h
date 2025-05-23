#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node for Huffman Tree
typedef struct HuffmanNode {
    char character;
    unsigned frequency;
    struct HuffmanNode *left, *right;
} HuffmanNode;

// Structure to store Huffman codes
typedef struct HuffmanCode {
    char character;
    char* code; // Binary string
    struct HuffmanCode* next; // For a list of codes
} HuffmanCode;


/*
 * Function: huffman_compress
 * Description: Compresses data using Huffman coding.
 *              The output format will include a header with frequency information
 *              to rebuild the Huffman tree for decompression.
 * Parameters:
 *   - input: Pointer to the input data.
 *   - input_len: Length of the input data.
 *   - output_len: Pointer to store the length of the compressed data.
 * Returns: Pointer to the compressed data or NULL on error.
 */
char* huffman_compress(const char* input, size_t input_len, size_t* output_len);

/*
 * Function: huffman_decompress
 * Description: Decompresses Huffman-coded data.
 *              Reads the frequency information from the header of the input data
 *              to rebuild the Huffman tree.
 * Parameters:
 *   - input: Pointer to the compressed data.
 *   - input_len: Length of the compressed data.
 *   - output_len: Pointer to store the length of the decompressed data.
 * Returns: Pointer to the decompressed data or NULL on error.
 */
char* huffman_decompress(const char* input, size_t input_len, size_t* output_len);

#endif // COMPRESS_H 