#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// XOR cipher functions
char* xor_encrypt(const char* input, size_t input_len, const char* key, size_t* output_len);
char* xor_decrypt(const char* input, size_t input_len, const char* key, size_t* output_len);

// Caesar cipher functions
char* caesar_encrypt(const char* input, size_t input_len, int shift, size_t* output_len);
char* caesar_decrypt(const char* input, size_t input_len, int shift, size_t* output_len);

#endif // ENCRYPT_H 