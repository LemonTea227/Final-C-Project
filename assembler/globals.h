/*
 * File: globals.h
 * Description: Global constants, macros, and data structures for the assembler.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants to avoid magic numbers */
#define MAX_LINE_LEN 82 /* 81 characters + \0 */
#define MAX_FILE_NAME 256
#define MAX_LABEL_LEN 32

/* Function Prototypes */
void *safe_malloc(size_t size);
void process_file(char *base_filename);

#endif