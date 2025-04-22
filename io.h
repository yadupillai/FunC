#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File I/O functions
char* read_file(const char* filename, size_t* file_size);
int write_file(const char* filename, const char* data, size_t data_size);

// Error handling
void handle_error(const char* message);
void handle_memory_error(void);

// Debug logging
#ifdef DEBUG
#define DEBUG_LOG(fmt, ...) fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_LOG(fmt, ...)
#endif

#endif // IO_H 