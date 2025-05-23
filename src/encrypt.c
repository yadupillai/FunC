#include "../include/encrypt.h"
#include "../include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* xor_encrypt(const char* input, size_t input_len, const char* key, size_t* output_len) {
    if (!input || !key || input_len == 0) {
        handle_error("Invalid input for encryption");
        return NULL;
    }

    size_t key_len = strlen(key);
    if (key_len == 0) {
        handle_error("Empty encryption key");
        return NULL;
    }

    char* output = malloc(input_len + 1);
    if (!output) {
        handle_memory_error();
        return NULL;
    }

    for (size_t i = 0; i < input_len; i++) {
        output[i] = input[i] ^ key[i % key_len];
    }

    output[input_len] = '\0';
    *output_len = input_len;
    return output;
}

char* xor_decrypt(const char* input, size_t input_len, const char* key, size_t* output_len) {
    /* XOR decryption is the same as encryption */
    return xor_encrypt(input, input_len, key, output_len);
} 