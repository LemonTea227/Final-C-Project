/*
 * File: first_pass.h
 * Description: Prototypes for the first pass phase.
 */

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"
#include "symbols.h"

int execute_first_pass(const char *base_filename, symbol_node **sym_table, int *ic, int *dc);

#endif