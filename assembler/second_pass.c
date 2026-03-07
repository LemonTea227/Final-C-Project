/*
 * File: second_pass.c
 * Description: Executes the second pass. Translates instructions into machine code
 * and generates the object (.ob), entry (.ent), and external (.ext) files.
 */

#include "second_pass.h"

/*
 * Function: execute_second_pass
 * Description: Re-scans the .am file, encodes the instructions using the complete symbol table,
 * and writes the final output files.
 * Receives: base_filename (string), sym_table (head of the symbol table),
 * ic (final Instruction Counter), dc (final Data Counter).
 * Returns: 1 if successful, 0 if errors occurred.
 */
int execute_second_pass(const char *base_filename, symbol_node *sym_table, int ic, int dc) {
    FILE *am_file;
    char full_filename[MAX_FILE_NAME];
    char line[MAX_LINE_LEN];
    int error_found = 0;
    int line_number = 0;

    sprintf(full_filename, "%s.am", base_filename);
    am_file = fopen(full_filename, "r");

    if (am_file == NULL) {
        return 0;
    }

    /* * TODO: Open output files (.ob, .ent, .ext).
     * Write the header to .ob file (IC and DC).
     */

    while (fgets(line, MAX_LINE_LEN, am_file) != NULL) {
        line_number++;
        /* * TODO:
         * 1. Skip labels (they were handled in the first pass).
         * 2. Encode instructions to machine words (considering the funct and opcode).
         * 3. Resolve symbols using find_symbol().
         * 4. Write machine words to the .ob file.
         * 5. If an external symbol is used, write to .ext file.
         * 6. If an entry symbol is defined, write to .ent file.
         */
    }

    fclose(am_file);
    /* TODO: Close output files */

    return !error_found;
}