/*
 * File: second_pass.c
 * Description: Executes the second pass. Translates instructions into machine code
 * and generates the object (.ob), entry (.ent), and external (.ext) files.
 */

#include "second_pass.h"
#include <ctype.h>

/* ARE Constants */
#define ABSOLUTE 0
#define EXTERNAL 1
#define RELOCATABLE 2

/*
 * Function: build_instruction_word
 * Description: Constructs a 12-bit instruction word using bitwise operations.
 * The word format: Opcode (bits 8-11), Source mode (bits 4-5), 
 * Destination mode (bits 2-3), ARE (bits 0-1).
 * Receives: opcode (instruction ID 0-15), src_mode (source addressing mode 0-3),
 * dest_mode (destination addressing mode 0-3).
 * Returns: A 12-bit unsigned integer representing the encoded instruction word.
 */
static unsigned int build_instruction_word(int opcode, int src_mode, int dest_mode) {
    unsigned int word = 0;
    word |= (opcode << OPCODE_SHIFT);        /* Opcode at bits 8-11 */
    word |= (src_mode << SRC_MODE_SHIFT);    /* Src mode at bits 4-5 */
    word |= (dest_mode << DEST_MODE_SHIFT);  /* Dest mode at bits 2-3 */
    word |= ABSOLUTE;                        /* ARE for instruction is Absolute (00) */
    return word & WORD_MASK;                 /* Ensure it's 12 bits */
}

/*
 * Function: execute_second_pass
 * Description: Re-scans the .am file, encodes the instructions using the complete symbol table,
 * and writes the final output files.
 * Receives: base_filename (string), sym_table (head of the symbol table),
 * ic (final Instruction Counter), dc (final Data Counter).
 * Returns: 1 if successful, 0 if errors occurred.
 */
int execute_second_pass(const char *base_filename, symbol_node *sym_table, int ic, int dc) {
    FILE *am_file, *ob_file, *ent_file, *ext_file;
    char filename[MAX_FILE_NAME];
    char line[MAX_LINE_LEN];
    int current_ic = START_ADDR;
    int error_found = 0;
    int line_num = 0;
    int has_entry = 0, has_extern = 0;

    sprintf(filename, "%s.am", base_filename);
    am_file = fopen(filename, "r");
    if (am_file == NULL) return 0;

    sprintf(filename, "%s.ob", base_filename);
    ob_file = fopen(filename, "w");
    
    sprintf(filename, "%s.ent", base_filename);
    ent_file = fopen(filename, "w");

    sprintf(filename, "%s.ext", base_filename);
    ext_file = fopen(filename, "w");

    if (!ob_file || !ent_file || !ext_file) {
        fprintf(stderr, "Error creating output files.\n");
        return 0;
    }

    /* Print Object File Header */
    fprintf(ob_file, "  %d %d\n", ic - START_ADDR, dc);

    while (fgets(line, MAX_LINE_LEN, am_file) != NULL) {
        char *token;
        char temp_line[MAX_LINE_LEN];
        line_num++;
        strcpy(temp_line, line);
        token = strtok(temp_line, " \t\n\r");

        if (token == NULL || *token == ';') continue;

        /* Skip label definition */
        if (strchr(token, ':') != NULL) {
            token = strtok(NULL, " \t\n\r");
            if (token == NULL) continue;
        }

        if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0 || strcmp(token, ".extern") == 0) {
            continue; /* Data is processed separately, extern already handled */
        }

        if (strcmp(token, ".entry") == 0) {
            char *entry_label = strtok(NULL, " \t\n\r");
            symbol_node *sym = find_symbol(sym_table, entry_label);
            if (sym != NULL) {
                sym->type = SYMBOL_TYPE_ENTRY;
                fprintf(ent_file, "%s\t%04d\n", entry_label, sym->address);
                has_entry = 1;
            } else {
                fprintf(stderr, "Error at line %d: .entry label '%s' not defined.\n", line_num, entry_label);
                error_found = 1;
            }
            continue;
        }

        /* Here we process actual commands. 
         * Note: This is a simplified encoding logic. In a full implementation, 
         * you extract operands, determine addressing modes (0,1,2,3), and build the extra words. */
        {
            unsigned int machine_word = build_instruction_word(0, 0, 0); /* Dummy default word */
            
            /* Print the instruction word to object file */
            fprintf(ob_file, "%04d\t%03X\n", current_ic++, machine_word);

            /* Parse operands and generate extra words... */
            token = strtok(NULL, " ,\t\n\r");
            while (token != NULL) {
                if (token[0] == 'r') { /* Register */
                    fprintf(ob_file, "%04d\t%03X\n", current_ic++, (atoi(&token[1]) << DEST_MODE_SHIFT) | ABSOLUTE);
                } else if (token[0] == '#') { /* Immediate */
                    int val = atoi(&token[1]);
                    fprintf(ob_file, "%04d\t%03X\n", current_ic++, ((val << DEST_MODE_SHIFT) | ABSOLUTE) & WORD_MASK);
                } else { /* Label */
                    /* Clean potential symbols like '%' */
                    char *clean_label = (token[0] == '%' || token[0] == '&') ? token + 1 : token;
                    symbol_node *sym = find_symbol(sym_table, clean_label);
                    if (sym != NULL) {
                        int are = (sym->type == SYMBOL_TYPE_EXTERNAL) ? EXTERNAL : RELOCATABLE;
                        fprintf(ob_file, "%04d\t%03X\n", current_ic++, ((sym->address << DEST_MODE_SHIFT) | are) & WORD_MASK);
                        if (are == EXTERNAL) {
                            fprintf(ext_file, "%s\t%04d\n", clean_label, current_ic - 1);
                            has_extern = 1;
                        }
                    } else {
                        fprintf(stderr, "Error at line %d: Undefined label '%s'\n", line_num, clean_label);
                        error_found = 1;
                    }
                }
                token = strtok(NULL, " ,\t\n\r");
            }
        }
    }

    fclose(am_file);
    fclose(ob_file);
    fclose(ent_file);
    fclose(ext_file);

    /* Clean up empty files if no entries/externals existed or on error */
    if (!has_entry || error_found) {
        sprintf(filename, "%s.ent", base_filename);
        remove(filename);
    }
    if (!has_extern || error_found) {
        sprintf(filename, "%s.ext", base_filename);
        remove(filename);
    }
    if (error_found) {
        sprintf(filename, "%s.ob", base_filename);
        remove(filename);
    }

    return !error_found;
}