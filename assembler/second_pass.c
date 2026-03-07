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

/* Helper to build a standard 12-bit word. 
 * ADJUST THESE SHIFTS according to your 2026 course booklet if needed. */
static unsigned int build_instruction_word(int opcode, int src_mode, int dest_mode) {
    unsigned int word = 0;
    word |= (opcode << 6);      /* Opcode usually at bits 6-9 */
    word |= (src_mode << 4);    /* Src mode at bits 4-5 */
    word |= (dest_mode << 2);   /* Dest mode at bits 2-3 */
    word |= ABSOLUTE;           /* ARE for instruction word is always Absolute (00) */
    return word & 0xFFF;        /* Ensure it's 12 bits */
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
    int current_ic = 100;
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
    fprintf(ob_file, "  %d %d\n", final_ic - 100, final_dc);

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
                    fprintf(ob_file, "%04d\t%03X\n", current_ic++, (atoi(&token[1]) << 2) | ABSOLUTE);
                } else if (token[0] == '#') { /* Immediate */
                    int val = atoi(&token[1]);
                    fprintf(ob_file, "%04d\t%03X\n", current_ic++, ((val << 2) | ABSOLUTE) & 0xFFF);
                } else { /* Label */
                    /* Clean potential symbols like '%' */
                    char *clean_label = (token[0] == '%' || token[0] == '&') ? token + 1 : token;
                    symbol_node *sym = find_symbol(sym_table, clean_label);
                    if (sym != NULL) {
                        int are = (sym->type == SYMBOL_TYPE_EXTERNAL) ? EXTERNAL : RELOCATABLE;
                        fprintf(ob_file, "%04d\t%03X\n", current_ic++, ((sym->address << 2) | are) & 0xFFF);
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
    if (!has_entry || error_found) remove(sprintf(filename, "%s.ent", base_filename), filename);
    if (!has_extern || error_found) remove(sprintf(filename, "%s.ext", base_filename), filename);
    if (error_found) remove(sprintf(filename, "%s.ob", base_filename), filename);

    return !error_found;
}