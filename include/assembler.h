#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdbool.h>
#include "symbol_table.h"

typedef struct CInstructionParts{
  char* dest;
  char* comp;
  char* jmp;
} CInstructionParts;

// Core assembler
void assemble(FILE *input, FILE *output);

// Predefined symbols
void add_predefined_symbols(SymbolTable *st);

// Instruction type checks
bool is_A_instruction(char* ins);
bool is_label(char* line);

// Translation functions
char* translate_A_instruction(char* ins);
char* translate_C_instruction(char* ins);
CInstructionParts* extract_C_instruction_parts(char* ins);
char* extract_label(char* line);

// Output
void write_instruction_to_file(char* ins, FILE* output);

#endif