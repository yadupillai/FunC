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

// String utilities
char* my_strdup(const char* s);

#endif // IO_H 