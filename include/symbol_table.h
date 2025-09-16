#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

typedef struct Symbol{
    const char *name;
    int address;
} Symbol;

typedef struct SymbolTable{
    Symbol *symbols;
    size_t size;
    size_t capacity;
} SymbolTable;

// Initialization and cleanup
void symbol_table_init(SymbolTable *table, size_t initial_capacity);
void symbol_table_free(SymbolTable *table);

// Symbol operations
bool symbol_table_contains(SymbolTable *table, const char *name);
bool symbol_table_get_address(SymbolTable *table, const char *name, int *address);
bool symbol_table_add(SymbolTable *table, const char *name, int address);

// Utilities
void symbol_table_print(SymbolTable *table);

#endif