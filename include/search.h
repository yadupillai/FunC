#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Search result structure
typedef struct {
    int line_number;
    char* line;
    struct SearchResult* next;
} SearchResult;

// Search functions
SearchResult* search_text(const char* text, const char* keyword);
void free_search_results(SearchResult* results);
void print_search_results(SearchResult* results);

#endif // SEARCH_H 