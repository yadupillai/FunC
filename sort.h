#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Line array structure
typedef struct {
    char** lines;
    size_t count;
} LineArray;

// Sort functions
LineArray* split_into_lines(const char* text);
void sort_lines(LineArray* lines);
char* join_lines(LineArray* lines);
void free_line_array(LineArray* lines);

#endif // SORT_H 