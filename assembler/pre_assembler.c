/*
 * File: pre_assembler.c
 * Description: Handles the pre-assembler phase, finding macros in the .as file
 * and generating a new .am file with all macros expanded.
 */

#include "pre_assembler.h"

/*
 * Function: execute_pre_assembler
 * Description: Reads the original assembly file, expands macros, and writes to a .am file.
 * Receives: input_file (pointer to the open .as file), base_filename (string of the file name).
 * Returns: 1 if successful, 0 if an error occurred.
 */
int execute_pre_assembler(FILE *input_file, const char *base_filename) {
    FILE *output_file;
    char am_filename[MAX_FILE_NAME];
    char line[MAX_LINE_LEN];

    sprintf(am_filename, "%s.am", base_filename);
    output_file = fopen(am_filename, "w");

    if (output_file == NULL) {
        fprintf(stderr, "Error: Could not create output file %s\n", am_filename);
        return 0;
    }

    /* * TODO: Implement macro table linked list.
     * Loop over input_file line by line using fgets(line, MAX_LINE_LEN, input_file).
     * If line is 'mcro', save the following lines until 'mcroend'.
     * If line is a macro name, print its saved lines to output_file.
     * Otherwise, print the line as-is to output_file.
     */
    
    while (fgets(line, MAX_LINE_LEN, input_file) != NULL) {
        /* Placeholder: currently just copies the file */
        fputs(line, output_file);
    }

    fclose(output_file);
    return 1;
}