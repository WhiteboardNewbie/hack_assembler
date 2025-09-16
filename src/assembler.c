#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "utils.h"
#include <stdbool.h>

void assemble(FILE *input, FILE *output) {

  // FILE*, FILE* -> void
  // Main assembler function: reads assembly code from input, translates instructions, and writes binary to output

  SymbolTable table;
  symbol_table_init(&table, 23);
  add_predefined_symbols(&table);

  char *line;
  size_t line_number = 0;

  // First pass: process labels and add them to the symbol table
  while((line = read_line(input)) != NULL) {
    char* cleared_line = clear_line(line);
    if (strlen(cleared_line) == 0) {
      free(cleared_line);
      free(line);
      continue;
    }

    if(is_label(cleared_line)) {
      char* label = extract_label(cleared_line);
      symbol_table_add(&table, label, line_number);
      free(label);
      continue;
    }

    line_number++;
  }

  fseek(input, 0, SEEK_SET);
  size_t variable_address = 16;

  // Second pass: translate instructions
  while((line = read_line(input)) != NULL) {
    char* cleared_line = clear_line(line);
    if (strlen(cleared_line) == 0) {
      free(cleared_line);
      free(line);
      continue;
    }

    if (is_label(cleared_line)) continue;

    if (cleared_line[0] == '@') {  // A-instruction
      if (is_A_instruction(cleared_line)) {
        char* translated_instruction = translate_A_instruction(cleared_line);
        write_instruction_to_file(translated_instruction, output);
        free(translated_instruction);
      } 
      else {  // variable
        char* variable_name = cleared_line + 1;
        if (symbol_table_contains(&table, variable_name)) {
          int address = 0;
          symbol_table_get_address(&table, variable_name, &address);
          char* str_address = int_to_string(address);
          char* binary_address = convert_to_binary(str_address);
          char* translated_instruction = pad_left(binary_address, 16);
          write_instruction_to_file(translated_instruction, output);
          free(str_address);
          free(binary_address);
          free(translated_instruction);
        }
        else {
          symbol_table_add(&table, variable_name, variable_address++);
          int address = 0;
          symbol_table_get_address(&table, variable_name, &address);
          char* str_address = int_to_string(address);
          char* binary_address = convert_to_binary(str_address);
          char* translated_instruction = pad_left(binary_address, 16);
          write_instruction_to_file(translated_instruction, output);
          free(binary_address);
          free(translated_instruction);
        }
      }
    }
    else { // C-instruction
      char* translated_instruction = translate_C_instruction(cleared_line);
      write_instruction_to_file(translated_instruction, output);
      free(translated_instruction);
    }

    free(cleared_line);
    free(line);
  }

  symbol_table_free(&table);
}

void add_predefined_symbols(SymbolTable *st) {

  // SymbolTable* -> void
  // Adds the predefined Hack assembly symbols to the symbol table

    char reg_name[4];
    for (int i = 0; i <= 15; i++) {
      snprintf(reg_name, sizeof(reg_name), "R%d", i);
      symbol_table_add(st, reg_name, i);
    }

    symbol_table_add(st, "SP", 0);
    symbol_table_add(st, "LCL", 1);
    symbol_table_add(st, "ARG", 2);
    symbol_table_add(st, "THIS", 3);
    symbol_table_add(st, "THAT", 4);
    symbol_table_add(st, "SCREEN", 16384);
    symbol_table_add(st, "KBD", 24576);
}

bool is_label(char* line) {

  // String -> bool
  // Returns true if the line represents a label (enclosed in parentheses)

  return line[0] == '(';
}

char* extract_label(char* line) {

  // String -> String
  // Extracts the label name from a label line and returns a newly allocated string

  return substring(line, 1, strlen(line) - 1);
}

bool is_A_instruction(char* ins) {

  // String -> bool
  // Checks if the instruction is a numeric A-instruction (e.g., @123)

  return ins[0] == '@' && (ins[1] >= '0' && ins[1] <= '9');
}

char* translate_A_instruction(char* ins) {

  // String -> String
  // Translates a numeric A-instruction into a 16-bit binary string

  char* binary_number = convert_to_binary(ins + 1);
  char* binary_instruction = pad_left(binary_number, 16);
  free(binary_number);
  return binary_instruction;
}

char* translate_C_instruction(char* ins) {

  // String -> String
  // Translates a C-instruction into a 16-bit binary string

  CInstructionParts* parts = extract_C_instruction_parts(ins);
  char *dest = parts->dest ? strdup(parts->dest) : NULL;
  char *comp = parts->comp ? strdup(parts->comp) : NULL;
  char *jmp  = parts->jmp  ? strdup(parts->jmp)  : NULL;

  char* A_bit = NULL;
  char* M_bit = NULL;
  char* D_bit = NULL; 
  char* translated_dest;
  char* translated_comp;
  char* translated_jmp;
  char* translated_instruction;

  // Encode destination bits
  if (dest == NULL) {
    translated_dest = strdup("000");
  } else {
    A_bit = strdup(string_search(dest, 'A') != -1 ? "1" : "0");
    M_bit = strdup(string_search(dest, 'M') != -1 ? "1" : "0");
    D_bit = strdup(string_search(dest, 'D') != -1 ? "1" : "0");
    char* tmp = append_strings(A_bit, D_bit);
    translated_dest = append_strings(tmp, M_bit);
    free(tmp);
  }

  // Encode computation bits
  if (strcmp(comp, "0") == 0) translated_comp = strdup("0101010");
  else if (strcmp(comp, "1") == 0) translated_comp = strdup("0111111");
  else if (strcmp(comp, "-1") == 0) translated_comp = strdup("0111010");
  else if (strcmp(comp, "D") == 0) translated_comp = strdup("0001100");
  else if (strcmp(comp, "A") == 0) translated_comp = strdup("0110000");
  else if (strcmp(comp, "!D") == 0) translated_comp = strdup("0001101");
  else if (strcmp(comp, "!A") == 0) translated_comp = strdup("0110001");
  else if (strcmp(comp, "-D") == 0) translated_comp = strdup("0001111");
  else if (strcmp(comp, "-A") == 0) translated_comp = strdup("0110011");
  else if (strcmp(comp, "D+1") == 0) translated_comp = strdup("0011111");
  else if (strcmp(comp, "A+1") == 0) translated_comp = strdup("0110111");
  else if (strcmp(comp, "D-1") == 0) translated_comp = strdup("0001110");
  else if (strcmp(comp, "A-1") == 0) translated_comp = strdup("0110010");
  else if (strcmp(comp, "D+A") == 0) translated_comp = strdup("0000010");
  else if (strcmp(comp, "D-A") == 0) translated_comp = strdup("0010011");
  else if (strcmp(comp, "A-D") == 0) translated_comp = strdup("0000111");
  else if (strcmp(comp, "D&A") == 0) translated_comp = strdup("0000000");
  else if (strcmp(comp, "D|A") == 0) translated_comp = strdup("0010101");
  else if (strcmp(comp, "M") == 0) translated_comp = strdup("1110000");
  else if (strcmp(comp, "!M") == 0) translated_comp = strdup("1110001");
  else if (strcmp(comp, "-M") == 0) translated_comp = strdup("1110011");
  else if (strcmp(comp, "M+1") == 0) translated_comp = strdup("1110111");
  else if (strcmp(comp, "M-1") == 0) translated_comp = strdup("1110010");
  else if (strcmp(comp, "D+M") == 0) translated_comp = strdup("1000010");
  else if (strcmp(comp, "D-M") == 0) translated_comp = strdup("1010011");
  else if (strcmp(comp, "M-D") == 0) translated_comp = strdup("1000111");
  else if (strcmp(comp, "D&M") == 0) translated_comp = strdup("1000000");
  else if (strcmp(comp, "D|M") == 0) translated_comp = strdup("1010101");

  // Encode jump bits
  if (jmp == NULL || strcmp(jmp, "") == 0) translated_jmp = strdup("000");
  else if (strcmp(jmp, "JGT") == 0) translated_jmp = strdup("001");
  else if (strcmp(jmp, "JEQ") == 0) translated_jmp = strdup("010");
  else if (strcmp(jmp, "JGE") == 0) translated_jmp = strdup("011");
  else if (strcmp(jmp, "JLT") == 0) translated_jmp = strdup("100");
  else if (strcmp(jmp, "JNE") == 0) translated_jmp = strdup("101");
  else if (strcmp(jmp, "JLE") == 0) translated_jmp = strdup("110");
  else if (strcmp(jmp, "JMP") == 0) translated_jmp = strdup("111");

  // Combine into final instruction
  char* tmp1 = append_strings("111", translated_comp);
  char* tmp2 = append_strings(tmp1, translated_dest);
  translated_instruction = append_strings(tmp2, translated_jmp);

  free(tmp1);
  free(tmp2);
  free(translated_dest);
  free(translated_comp);
  free(translated_jmp);
  free(A_bit);
  free(D_bit);
  free(M_bit);
  free(dest);
  free(comp);
  free(jmp);
  free(parts->dest);
  free(parts->comp);
  free(parts->jmp);
  free(parts);

  return translated_instruction;
}

CInstructionParts* extract_C_instruction_parts(char* ins) {

  // String -> CInstructionParts*
  // Extracts dest, comp, and jump fields from a C-instruction
  // Returns a newly allocated struct containing pointers to allocated strings

  size_t ins_len = strlen(ins);

  CInstructionParts* parts = malloc(sizeof(CInstructionParts));
  parts->dest = NULL;
  parts->comp = NULL;
  parts->jmp  = NULL;
  int equals_loc = string_search(ins, '=');
  int semicolon_loc = string_search(ins, ';');
 
  if (equals_loc == -1) {
    parts->dest = NULL;
    if (semicolon_loc == -1) {
      parts->comp = substring(ins, 0, ins_len);
      parts->jmp = NULL;
    } else {
      parts->comp = substring(ins, 0, semicolon_loc);
      parts->jmp = substring(ins, semicolon_loc + 1, ins_len);
    }
  }
  else {
    parts->dest = substring(ins, 0, equals_loc);
    if (semicolon_loc == -1) {
      parts->comp = substring(ins, equals_loc + 1, ins_len);
      parts->jmp = NULL;
    } else {
      parts->comp = substring(ins, equals_loc + 1, semicolon_loc);
      parts->jmp = substring(ins, semicolon_loc + 1, ins_len);
    }
  }
  return parts;
}

void write_instruction_to_file(char* ins, FILE* output) {

  // String, FILE* -> void
  // Writes a binary instruction string to the output file with a newline

  for(int i = 0; ins[i] != '\0'; ++i) {
    fputc(ins[i], output);
  }
  fputc('\n', output);
}
