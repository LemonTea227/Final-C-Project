/*
 * assembler.c
 * Main entry point for the assembler program.
 * Handles command-line arguments, iterates over input files, and coordinates all assembler phases.
 */

#include "globals.h"
#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "symbols.h"

/*
 * Entry point for the assembler.
 * Iterates through all files provided as command-line arguments and processes each one.
 * Exits with error if no files are provided.
 *
 * Parameters:
 *   argc - argument count
 *   argv - argument vector (filenames)
 * Returns:
 *   0 on success, exits with failure code if arguments are missing.
 */
int main(int argc, char *argv[]) {
    int i;

    /* Argument validation */
    if (argc < 2) {
        fprintf(stderr, "Error: Missing input files.\n");
        fprintf(stderr, "Usage: ./assembler file1 file2 ...\n");
        exit(EXIT_FAILURE); /* Exiting with proper error code */
    }

    for (i = 1; i < argc; i++) {
        process_file(argv[i]);
    }

    return 0;
}

/*
 * Processes a single input file through all assembler phases:
 *   1. Pre-assembler (macro expansion)
 *   2. First pass (symbol table, IC/DC calculation)
 *   3. Second pass (machine code generation, output files)
 * Handles file opening/closing and error reporting for each phase.
 *
 * Parameters:
 *   base_filename - name of the file (without .as extension)
 */
void process_file(char *base_filename) {
    FILE *fp;
    char full_filename[MAX_FILE_NAME];
    symbol_node *symbol_table = NULL;
    int ic, dc;

    sprintf(full_filename, "%s.as", base_filename);
    fp = fopen(full_filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", full_filename);
        return;
    }

    printf("Processing %s...\n", full_filename);

    if (!execute_pre_assembler(fp, base_filename)) {
        fclose(fp);
        return;
    }
    fclose(fp); /* Done with .as file */

    if (!execute_first_pass(base_filename, &symbol_table, &ic, &dc)) {
        printf("Errors found in first pass. Stopping processing for %s.\n", base_filename);
        free_symbol_table(symbol_table);
        return;
    }

    if (!execute_second_pass(base_filename, symbol_table, ic, dc)) {
        printf("Errors found in second pass. Stopping processing for %s.\n", base_filename);
    } else {
        printf("Successfully compiled %s.\n", base_filename);
    }

    free_symbol_table(symbol_table);
}