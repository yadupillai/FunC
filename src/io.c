#include "../include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(const char* filename, size_t* file_size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        handle_error("Failed to open input file");
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer
    char* buffer = malloc(*file_size + 1);
    if (!buffer) {
        handle_memory_error();
        fclose(file);
        return NULL;
    }

    // Read file
    size_t bytes_read = fread(buffer, 1, *file_size, file);
    if (bytes_read != *file_size) {
        handle_error("Failed to read file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[*file_size] = '\0';
    fclose(file);
    return buffer;
}

int write_file(const char* filename, const char* data, size_t data_size) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        handle_error("Failed to open output file");
        return 0;
    }

    size_t bytes_written = fwrite(data, 1, data_size, file);
    if (bytes_written != data_size) {
        handle_error("Failed to write file");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

void handle_error(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
}

void handle_memory_error(void) {
    fprintf(stderr, "Memory allocation error\n");
    exit(EXIT_FAILURE);
}

// Custom string duplication function for C99 compatibility
char* my_strdup(const char* s) {
    if (s == NULL) {
        return NULL;
    }
    size_t len = strlen(s) + 1;
    char* new_s = malloc(len);
    if (new_s == NULL) {
        handle_memory_error();
        return NULL;
    }
    memcpy(new_s, s, len);
    return new_s;
} 