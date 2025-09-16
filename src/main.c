#include <stdio.h>
#include "symbol_table.h"
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "assembler.h"
#include <unistd.h>

int main(int argc, char *argv[]) {

    // int, char** -> int
    // Entry point for the assembler program
    // Usage: ./assembler <inputfile> [outputfile]
    // Reads an assembly (.asm) file, assembles it, and writes the output to a .hack file

    if (argc < 2) {
        printf("Usage: %s <inputfile> [outputfile]\n", argv[0]);
        return 1;
    }

    const char *input_file_name = argv[1];
    const char *output_file_name = (argc >= 3) ? argv[2] : NULL;

    const char *output_ext = ".hack";

    char *input_complete_name = strdup(input_file_name); // duplicate input filename
    char *output_complete_name;

    // Determine output file name
    if (output_file_name) {
        output_complete_name = append_strings(output_file_name, output_ext); // user-specified output file
    } else {
        char *dot = strrchr(input_file_name, '.');
        if (dot && strcmp(dot, ".asm") == 0) {
            // Replace .asm extension with .hack
            size_t base_len = dot - input_file_name;
            output_complete_name = malloc(base_len + strlen(output_ext) + 1);
            strncpy(output_complete_name, input_file_name, base_len);
            output_complete_name[base_len] = '\0';
            strcat(output_complete_name, output_ext);
        } else {
            // fallback: just append .hack
            output_complete_name = append_strings(input_file_name, output_ext);
        }
    }

    // Open input file for reading
    FILE *input = fopen(input_complete_name, "r");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    // Open output file for writing
    FILE *output = fopen(output_complete_name, "w");
    if (!output) {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    // Assemble input file into output file
    assemble(input, output);

    // Remove the trailing newline at the end of the output file
    fflush(output);
    long pos = ftell(output);
    ftruncate(fileno(output), pos - 1);

    // Clean up
    fclose(input);
    fclose(output);
    free(input_complete_name);
    free(output_complete_name);

    return 0;
}