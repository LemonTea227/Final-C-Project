#include "pre_assembler.h"
#include "macro.h"
#include <ctype.h>

/*
 * Skips leading whitespace characters in a string.
 * Returns pointer to first non-whitespace character.
 */
char *skip_whitespaces(char *str) {
    while (*str && isspace((unsigned char)*str)) str++;
    return str;
}

/*
 * Performs macro expansion (pre-assembler phase):
 *   - Reads a .as file, identifies macro definitions (mcro...mcroend)
 *   - Outputs an expanded .am file with macro calls replaced by their bodies
 *   - Reports errors if output file cannot be created
 *
 * Parameters:
 *   input_file    - open FILE pointer to .as file
 *   base_filename - file name without extension
 * Returns:
 *   1 if successful, 0 if file creation failed
 */
int execute_pre_assembler(FILE *input_file, const char *base_filename) {
    FILE *output_file;
    char am_filename[MAX_FILE_NAME];
    char line[MAX_LINE_LEN];
    char *first_word;
    char macro_name[MAX_LABEL_LEN];
    char macro_content[MAX_LINE_LEN * 20] = ""; /* Buffer for macro body */
    int in_macro_def = 0;
    macro_node *macro_table = NULL;
    macro_node *found_macro;

    sprintf(am_filename, "%s.am", base_filename);
    output_file = fopen(am_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error: Could not create output file %s\n", am_filename);
        return 0;
    }

    while (fgets(line, MAX_LINE_LEN, input_file) != NULL) {
        char temp_line[MAX_LINE_LEN];
        strcpy(temp_line, line);
        first_word = strtok(skip_whitespaces(temp_line), " \t\n\r");

        if (first_word == NULL) {
            if (!in_macro_def) fputs(line, output_file);
            continue;
        }

        /* Check for end of macro definition */
        if (strcmp(first_word, "mcroend") == 0) {
            in_macro_def = 0;
            add_macro(&macro_table, macro_name, macro_content);
            continue;
        }

        /* Check for start of macro definition */
        if (strcmp(first_word, "mcro") == 0) {
            char *name = strtok(NULL, " \t\n\r");
            if (name != NULL) {
                in_macro_def = 1;
                strcpy(macro_name, name);
                macro_content[0] = '\0'; /* Reset buffer */
            }
            continue;
        }

        /* Inside a macro definition - append to buffer */
        if (in_macro_def) {
            strcat(macro_content, line);
            continue;
        }

        /* Check if the first word is a known macro */
        found_macro = find_macro(macro_table, first_word);
        if (found_macro != NULL) {
            fputs(found_macro->content, output_file);
        } else {
            /* Normal line */
            fputs(line, output_file);
        }
    }

    fclose(output_file);
    free_macro_table(macro_table);
    return 1;
}