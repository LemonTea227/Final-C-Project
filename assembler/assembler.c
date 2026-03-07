/*
 * File: assembler.c
 * Description: Main entry point for the assembler program. 
 * Coordinates the reading of input files and passing them to the translation phases.
 */

#include "globals.h"

/*
 * Function: main
 * Description: The entry point of the assembler. Iterates through all files provided
 * in the command line arguments and processes them.
 * Receives: argc (argument count), argv (argument vector/strings).
 * Returns: 0 upon successful completion, or exits with failure code on missing arguments.
 */
int main(int argc, char *argv[]) {
    int i;

    /* Check if at least one file was provided */
    if (argc < 2) {
        fprintf(stderr, "Error: Missing input files.\n");
        fprintf(stderr, "Usage: ./assembler file1 file2 ...\n");
        exit(EXIT_FAILURE); /* Exiting with proper error code */
    }

    /* Iterate over each file provided in the command line */
    for (i = 1; i < argc; i++) {
        process_file(argv[i]);
    }

    return 0;
}

/*
 * Function: process_file
 * Description: Opens the given file and manages the pre-assembler, first pass, and second pass.
 * Receives: base_filename (the name of the file without the .as extension).
 * Returns: None.
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

    /* 1. Pre-Assembler */
    if (!execute_pre_assembler(fp, base_filename)) {
        fclose(fp);
        return;
    }
    fclose(fp); /* Done with .as file */

    /* 2. First Pass */
    if (!execute_first_pass(base_filename, &symbol_table, &ic, &dc)) {
        printf("Errors found in first pass. Stopping processing for %s.\n", base_filename);
        free_symbol_table(symbol_table);
        return;
    }

    /* 3. Second Pass */
    if (!execute_second_pass(base_filename, symbol_table, ic, dc)) {
        printf("Errors found in second pass. Stopping processing for %s.\n", base_filename);
    } else {
        printf("Successfully compiled %s.\n", base_filename);
    }

    /* Cleanup */
    free_symbol_table(symbol_table);
}