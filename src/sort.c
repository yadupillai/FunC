#include "../include/sort.h"
#include "../include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LineArray* split_into_lines(const char* text) {
    if (!text) {
        handle_error("Invalid input for line splitting");
        return NULL;
    }

    // Count lines
    size_t line_count = 1;
    const char* p = text;
    while (*p) {
        if (*p == '\n') {
            line_count++;
        }
        p++;
    }

    // Allocate line array
    LineArray* lines = malloc(sizeof(LineArray));
    if (!lines) {
        handle_memory_error();
        return NULL;
    }

    lines->lines = malloc(line_count * sizeof(char*));
    if (!lines->lines) {
        handle_memory_error();
        free(lines);
        return NULL;
    }

    lines->count = 0;

    // Split into lines
    const char* line_start = text;
    const char* line_end;
    while ((line_end = strchr(line_start, '\n')) != NULL) {
        size_t line_length = line_end - line_start;
        lines->lines[lines->count] = malloc(line_length + 1);
        if (!lines->lines[lines->count]) {
            handle_memory_error();
            free_line_array(lines);
            return NULL;
        }

        strncpy(lines->lines[lines->count], line_start, line_length);
        lines->lines[lines->count][line_length] = '\0';
        lines->count++;

        line_start = line_end + 1;
    }

    // Add last line if not empty
    if (*line_start != '\0') {
        lines->lines[lines->count] = strdup(line_start);
        if (!lines->lines[lines->count]) {
            handle_memory_error();
            free_line_array(lines);
            return NULL;
        }
        lines->count++;
    }

    return lines;
}

void sort_lines(LineArray* lines) {
    if (!lines || !lines->lines) {
        return;
    }

    // Simple bubble sort
    for (size_t i = 0; i < lines->count - 1; i++) {
        for (size_t j = 0; j < lines->count - i - 1; j++) {
            if (strcmp(lines->lines[j], lines->lines[j + 1]) > 0) {
                char* temp = lines->lines[j];
                lines->lines[j] = lines->lines[j + 1];
                lines->lines[j + 1] = temp;
            }
        }
    }
}

char* join_lines(LineArray* lines) {
    if (!lines || !lines->lines || lines->count == 0) {
        return NULL;
    }

    // Calculate total size
    size_t total_size = 0;
    for (size_t i = 0; i < lines->count; i++) {
        total_size += strlen(lines->lines[i]) + 1; // +1 for newline
    }

    // Allocate output buffer
    char* output = malloc(total_size + 1);
    if (!output) {
        handle_memory_error();
        return NULL;
    }

    // Join lines
    size_t pos = 0;
    for (size_t i = 0; i < lines->count; i++) {
        size_t line_len = strlen(lines->lines[i]);
        strcpy(output + pos, lines->lines[i]);
        pos += line_len;
        output[pos++] = '\n';
    }

    output[pos] = '\0';
    return output;
}

void free_line_array(LineArray* lines) {
    if (lines) {
        if (lines->lines) {
            for (size_t i = 0; i < lines->count; i++) {
                free(lines->lines[i]);
            }
            free(lines->lines);
        }
        free(lines);
    }
} 