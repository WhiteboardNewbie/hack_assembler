#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* append_strings(const char*a, const char* b) {

  // String, String -> String
  // Appends string b to string a and returns a new allocated string

  size_t len_a = strlen(a);
  size_t len_b = strlen(b);

  char *result = malloc(len_a + len_b + 1);
  strcpy(result, a);
  strcat(result, b);

  return result;
}

char* read_line(FILE *fp) {

  // FILE* -> String
  // Reads a line from the given file pointer and returns it as a newly allocated string
  // Stops at newline or EOF, does not include the newline

    size_t size = 128;
    size_t len = 0;
    char *buffer = malloc(size);
    if (!buffer) return NULL;

    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (len + 1 >= size) {
            size *= 2;
            buffer = realloc(buffer, size);
            if (!buffer) return NULL;
        }
        if (c == '\n') break;
        buffer[len++] = c;
    }

    if (len == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[len] = '\0';
    return buffer;
}

char* clear_line(char *line) {

  // String -> String
  // Removes spaces and comments from a line and returns a newly allocated cleaned string

    size_t size = 128;
    size_t len = 0;

    char* buffer = malloc(size);
    if (!buffer) return NULL;

    int i = 0;
    while(i != strlen(line)) {

        if(line[i] == '/') break; // stop at comment start

        if (len + 1 >= size) {
            size *= 2;
            char *tmp = realloc(buffer, size);
            if (!tmp) {
                free(buffer);
                return NULL;
            }
            buffer = tmp;
        }

        if (line[i] != ' ') {
            buffer[len++] = line[i];
        }

        i++;
    }

    buffer[len] = '\0';
    return buffer;
}

char* convert_to_binary(char* num) {

  // String -> String
  // Converts a decimal number string to its binary representation as a newly allocated string

    int int_num = strtol(num, NULL, 10);
    if(int_num == 0) {
        char* result = malloc(2 * sizeof(char));
        strcpy(result, "0");
        return result;
    }

    size_t size = 64;
    char* buffer = malloc(size);
    int i = 0;

    while (int_num > 0) {

        if((i + 1) >= size) {
            size *= 2;
            char* tmp = realloc(buffer, size);
            if (!tmp) {
                free(buffer);
                return NULL;
            }
            buffer = tmp;
        } 

        buffer[i++] = (int_num % 2) + '0';
        int_num = int_num / 2;
    }
    buffer[i++] = '\0';

    char* result = reverse_string(buffer);
    free(buffer);

    return result;
}

char* reverse_string(char* str) {

  // String -> String
  // Returns a newly allocated string which is the reverse of the input string

    char* reversed = malloc(strlen(str) + 1);

    for(int j = 0; j <= strlen(str); ++j) {
        reversed[j] = str[strlen(str) - j - 1];
    }
    reversed[strlen(str)] = '\0';

    return reversed;
}

char* pad_left(char* num, int total_len) {

  // String, int -> String
  // Pads the given string with '0's on the left up to total_len and returns a new string

    int len = strlen(num);
    if (len >= total_len) {
        char* copy = malloc(len + 1);
        strcpy(copy, num);
        return copy;
    }

    char* padded = malloc(total_len + 1);
    int pad_len = total_len - len;

    for (int i = 0; i < pad_len; i++) {
        padded[i] = '0';
    }

    strcpy(padded + pad_len, num);
    padded[total_len] = '\0';
    return padded;  
}

int string_search(const char* str, char c) {

  // String, char -> int
  // Returns the index of character c in str, or -1 if not found

    size_t len = strlen(str);
    for(int i = 0; i < len; ++i) {
        if (str[i] == c) return i;
    }
    return -1;
}

char* substring(const char* str, size_t start, size_t end) {

  // String, start index, end index -> String
  // Returns a newly allocated substring of str from start (inclusive) to end (exclusive)

    size_t len = strlen(str);
    if (start > len) start = len;
    if (end > len) end = len;
    if (end < start) end = start;

    size_t new_len = end - start;
    char *result = malloc(new_len + 1);
    if (!result) return NULL;

    strncpy(result, str + start, new_len);
    result[new_len] = '\0';
    return result;
}

char* int_to_string(int num) {

  // int -> String
  // Converts an integer to a newly allocated string representation

    int length = snprintf(NULL, 0,"%d", num);
    char* str = malloc(length + 1);
    snprintf(str, length + 1, "%d", num);
    return str;
}