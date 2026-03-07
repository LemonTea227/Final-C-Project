/*
 * File: pre_assembler.h
 * Description: Prototypes for the pre-assembler macro expansion phase.
 */

#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "globals.h"

int execute_pre_assembler(FILE *input_file, const char *base_filename);

#endif