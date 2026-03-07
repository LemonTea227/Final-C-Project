/*
 * File: first_pass.c
 * Description: Executes the first pass. Updates the Instruction Counter (IC) 
 * and Data Counter (DC), and populates the symbol table.
 */

#include "first_pass.h"
#include <ctype.h>

/* Defines for addressing modes and instruction sizes */
#define START_IC 100
#define MAX_OPCODES 16

/* Opcode Table Structure */
typedef struct {
    char *name;
    int opcode;
    int expected_operands;
} op_struct;

/* Standard 16 instructions */
static op_struct opcodes[MAX_OPCODES] = {
    {"mov", 0, 2}, {"cmp", 1, 2}, {"add", 2, 2}, {"sub", 3, 2},
    {"not", 4, 1}, {"clr", 5, 1}, {"lea", 6, 2}, {"inc", 7, 1},
    {"dec", 8, 1}, {"jmp", 9, 1}, {"bne", 10, 1}, {"red", 11, 1},
    {"prn", 12, 1}, {"jsr", 13, 1}, {"rts", 14, 0}, {"stop", 15, 0}
};

/* Helper: skip whitespaces */
static char *skip_white_spaces(char *str) {
    while (*str && isspace((unsigned char)*str)) str++;
    return str;
}

/* Helper: check if a string is a valid opcode */
static int is_opcode(const char *word) {
    int i;
    for (i = 0; i < MAX_OPCODES; i++) {
        if (strcmp(opcodes[i].name, word) == 0) return i;
    }
    return -1;
}

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
    int line_num = 0;

    sprintf(full_filename, "%s.am", base_filename);
    am_file = fopen(full_filename, "r");

    if (am_file == NULL) {
        fprintf(stderr, "Error: Could not open %s for the first pass.\n", full_filename);
        return 0;
    }

    *ic = START_IC;
    *dc = 0;

    while (fgets(line, MAX_LINE_LEN, am_file) != NULL) {
        char temp_line[MAX_LINE_LEN];
        char *first_word;
        char *colon_ptr;
        int has_label = 0;
        char label_name[MAX_LABEL_LEN] = {0};

        line_num++;
        strcpy(temp_line, line);
        first_word = skip_white_spaces(temp_line);

        /* Skip empty lines and comments */
        if (*first_word == '\0' || *first_word == ';') continue;

        /* Check for label */
        colon_ptr = strchr(first_word, ':');
        if (colon_ptr != NULL) {
            has_label = 1;
            strncpy(label_name, first_word, colon_ptr - first_word);
            label_name[colon_ptr - first_word] = '\0';
            first_word = skip_white_spaces(colon_ptr + 1);
        }

        /* Extract the directive or command */
        {
            char *token = strtok(first_word, " \t\n\r");
            if (token == NULL) continue;

            if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0) {
                if (has_label) {
                    if (find_symbol(*sym_table, label_name) != NULL) {
                        fprintf(stderr, "Error at line %d: Label %s already defined.\n", line_num, label_name);
                        error_found = 1;
                    } else {
                        add_symbol(sym_table, label_name, *dc, SYMBOL_TYPE_DATA);
                    }
                }
                
                if (strcmp(token, ".data") == 0) {
                    /* Count commas to determine number of data elements */
                    char *ptr = strtok(NULL, "");
                    if(ptr != NULL) {
                        int count = 1;
                        while (*ptr) { if (*ptr == ',') count++; ptr++; }
                        *dc += count;
                    }
                } else if (strcmp(token, ".string") == 0) {
                    char *str_start = strchr(line, '\"');
                    if (str_start) {
                        char *str_end = strrchr(str_start + 1, '\"');
                        if (str_end) *dc += (str_end - str_start); /* chars + null terminator */
                    }
                }
            } else if (strcmp(token, ".extern") == 0) {
                char *ext_label = strtok(NULL, " \t\n\r");
                if (ext_label) add_symbol(sym_table, ext_label, 0, SYMBOL_TYPE_EXTERNAL);
            } else if (strcmp(token, ".entry") == 0) {
                /* Handled in second pass */
                continue;
            } else {
                /* It's an instruction */
                int op_idx = is_opcode(token);
                if (op_idx != -1) {
                    if (has_label) {
                        if (find_symbol(*sym_table, label_name) != NULL) {
                            fprintf(stderr, "Error at line %d: Label %s already defined.\n", line_num, label_name);
                            error_found = 1;
                        } else {
                            add_symbol(sym_table, label_name, *ic, SYMBOL_TYPE_CODE);
                        }
                    }
                    /* Simple IC calculation: 1 word for instruction + 1 for each operand (approx logic) */
                    /* Note: If both operands are registers, they usually share 1 word. */
                    *ic += 1; /* The instruction word */
                    if (opcodes[op_idx].expected_operands == 1) *ic += 1;
                    else if (opcodes[op_idx].expected_operands == 2) *ic += 2; /* Simplified */
                } else {
                    fprintf(stderr, "Error at line %d: Unknown command '%s'\n", line_num, token);
                    error_found = 1;
                }
            }
        }
    }

    /* Update data symbols with final IC */
    {
        symbol_node *curr = *sym_table;
        while (curr != NULL) {
            if (curr->type == SYMBOL_TYPE_DATA) curr->address += *ic;
            curr = curr->next;
        }
    }

    fclose(am_file);
    return !error_found;
}