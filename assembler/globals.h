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
#define START_ADDR 100 /* Initial address for instructions */
#define MEMORY_SIZE 4096 /* Total memory words */
#define WORD_SIZE_BITS 12 /* CPU word size in bits */
#define WORD_MASK 0xFFF /* Mask for 12-bit word */
#define OPCODE_SHIFT 8 /* Opcode bit position */
#define SRC_MODE_SHIFT 4 /* Source addressing mode shift */
#define DEST_MODE_SHIFT 2 /* Destination addressing mode shift */
#define ARE_BITS_MASK 0x3 /* 2-bit ARE field mask */
#define NUM_REGISTERS 8 /* Number of registers (r0-r7) */
#define NUM_OPCODES 16 /* Number of instruction opcodes */

/* Function Prototypes */
void *safe_malloc(size_t size);
void process_file(char *base_filename);

#endif