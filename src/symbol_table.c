#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void symbol_table_init(SymbolTable *table, size_t initial_capacity) {
    table->symbols = (Symbol *)malloc(initial_capacity * sizeof(Symbol));
    if (!table->symbols) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    table->size = 0;
    table->capacity = initial_capacity;
}

void symbol_table_print(SymbolTable *table) {
    printf("Symbol Table:\n");
    printf("-------------------\n");
    for (size_t i = 0; i < table->size; i++) {
        printf("%s -> %d\n", table->symbols[i].name, table->symbols[i].address);
    }
    printf("-------------------\n");
}

void symbol_table_free(SymbolTable *table) {
    for (size_t i = 0; i < table->size; i++) {
        free((void *)table->symbols[i].name);
    }
    free(table->symbols);
    table->symbols = NULL;
    table->size = 0;
    table->capacity = 0;
}

bool symbol_table_contains(SymbolTable *table, const char *name) {
    for (size_t i = 0; i < table->size; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

bool symbol_table_get_address(SymbolTable *table, const char *name, int *address) {
    for (size_t i = 0; i < table->size; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            *address = table->symbols[i].address;
            return true;
        }
    }
    return false;
}

bool symbol_table_add(SymbolTable *table, const char *name, int address) {
    if (symbol_table_contains(table, name)) {
        return false;
    }

    if (table->size >= table->capacity) {
        size_t new_capacity = table->capacity * 2;
        Symbol *new_symbols = (Symbol *)realloc(table->symbols, new_capacity * sizeof(Symbol));
        if (!new_symbols) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
        table->symbols = new_symbols;
        table->capacity = new_capacity;
    }

    table->symbols[table->size].name = strdup(name);
    if (!table->symbols[table->size].name) {
        fprintf(stderr, "Memory allocation for symbol name failed\n");
        exit(1);
    }
    table->symbols[table->size].address = address;
    table->size++;

    return true;
}