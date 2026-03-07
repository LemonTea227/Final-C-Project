/*
 * File: second_pass.h
 * Description: Prototypes for the second pass phase.
 */

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"
#include "symbols.h"

int execute_second_pass(const char *base_filename, symbol_node *sym_table, int ic, int dc);

#endif