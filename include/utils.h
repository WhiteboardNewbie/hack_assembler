#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// String manipulation
char* append_strings(const char *a, const char *b);
char* reverse_string(char* str);
char* pad_left(char* num, int total_len);
char* substring(const char* str, size_t start, size_t end);

// File and line handling
char* read_line(FILE *fp);
char* clear_line(char* line);

// Number conversion
char* convert_to_binary(char* num);
char* int_to_string(int num);

// Utilities
int string_search(const char* str, char c);

#endif