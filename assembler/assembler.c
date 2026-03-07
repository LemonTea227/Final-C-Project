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

    /* Append .as extension */
    sprintf(full_filename, "%s.as", base_filename);

    /* Open file and check for success */
    fp = fopen(full_filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s' for reading.\n", full_filename);
        return; /* Skip to the next file */
    }

    printf("Processing file: %s\n", full_filename);

    /* * TODO: Implement the calls to your logical modules here:
     * 1. execute_pre_assembler(fp, base_filename);
     * 2. execute_first_pass(...);
     * 3. execute_second_pass(...);
     */

    /* Close the file properly */
    fclose(fp);
}