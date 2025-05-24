#include "../include/search.h"
#include "../include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SearchResult* search_text(const char* text, const char* keyword) {
    if (!text || !keyword) {
        handle_error("Invalid input for search");
        return NULL;
    }

    SearchResult* head = NULL;
    SearchResult* current = NULL;
    int line_number = 1;
    const char* line_start = text;
    const char* line_end;

    while ((line_end = strchr(line_start, '\n')) != NULL) {
        size_t line_length = line_end - line_start;
        char* line = malloc(line_length + 1);
        if (!line) {
            handle_memory_error();
            free_search_results(head);
            return NULL;
        }

        strncpy(line, line_start, line_length);
        line[line_length] = '\0';

        if (strstr(line, keyword) != NULL) {
            SearchResult* result = malloc(sizeof(SearchResult));
            if (!result) {
                handle_memory_error();
                free(line);
                free_search_results(head);
                return NULL;
            }

            result->line_number = line_number;
            result->line = line;
            result->next = NULL;

            if (!head) {
                head = result;
                current = result;
            } else {
                current->next = result;
                current = result;
            }
        } else {
            free(line);
        }

        line_start = line_end + 1;
        line_number++;
    }

    // Check last line
    if (*line_start != '\0') {
        char* line = my_strdup(line_start);
        if (!line) {
            handle_memory_error();
            free_search_results(head);
            return NULL;
        }

        if (strstr(line, keyword) != NULL) {
            SearchResult* result = malloc(sizeof(SearchResult));
            if (!result) {
                handle_memory_error();
                free(line);
                free_search_results(head);
                return NULL;
            }

            result->line_number = line_number;
            result->line = line;
            result->next = NULL;

            if (!head) {
                head = result;
            } else {
                current->next = result;
            }
        } else {
            free(line);
        }
    }

    return head;
}

void free_search_results(SearchResult* results) {
    while (results) {
        SearchResult* next = results->next;
        free(results->line);
        free(results);
        results = next;
    }
}

void print_search_results(SearchResult* results) {
    if (!results) {
        printf("No matches found.\n");
        return;
    }

    printf("Search results:\n");
    while (results) {
        printf("%d: %s\n", results->line_number, results->line);
        results = results->next;
    }
} 