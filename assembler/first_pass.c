/*
 * File: first_pass.c
 * Description: Executes the first pass. Updates the Instruction Counter (IC) 
 * and Data Counter (DC), and populates the symbol table.
 */

#include "first_pass.h"

/*
 * Function: execute_first_pass
 * Description: Scans the .am file to build the symbol table and check for basic syntax errors.
 * Receives: base_filename (string), sym_table (pointer to symbol table head), 
 * ic (pointer to Instruction Counter), dc (pointer to Data Counter).
 * Returns: 1 if the pass completed without errors, 0 if syntax errors were found.
 */
int execute_first_pass(const char *base_filename, symbol_node **sym_table, int *ic, int *dc) {
    FILE *am_file;
    char full_filename[MAX_FILE_NAME];
    char line[MAX_LINE_LEN];
    int error_found = 0;
    int line_number = 0;

    sprintf(full_filename, "%s.am", base_filename);
    am_file = fopen(full_filename, "r");

    if (am_file == NULL) {
        fprintf(stderr, "Error: Could not open %s for the first pass.\n", full_filename);
        return 0;
    }

    *ic = 100; /* Initial memory address according to instructions */
    *dc = 0;

    while (fgets(line, MAX_LINE_LEN, am_file) != NULL) {
        line_number++;
        /* * TODO: 
         * 1. Ignore empty lines and comments (;).
         * 2. Check for labels. If label exists, add to symbol table with current IC or DC.
         * 3. Identify instruction (.data, .string, or command).
         * 4. Update IC or DC based on the number of words the instruction requires.
         * 5. Report syntax errors with line_number.
         */
    }

    fclose(am_file);
    return !error_found;
}