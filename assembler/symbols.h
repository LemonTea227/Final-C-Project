/*
 * File: symbols.h
 * Description: Data structures and function prototypes for the Symbol Table.
 */

#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "globals.h"

/* Symbol attribute flags */
#define SYMBOL_TYPE_CODE 1
#define SYMBOL_TYPE_DATA 2
#define SYMBOL_TYPE_EXTERNAL 3
#define SYMBOL_TYPE_ENTRY 4

/* Linked list node for a symbol */
typedef struct symbol_node {
    char name[MAX_LABEL_LEN];
    int address;
    int type;
    struct symbol_node *next;
} symbol_node;

/* Function Prototypes */
void add_symbol(symbol_node **head, const char *name, int address, int type);
symbol_node *find_symbol(symbol_node *head, const char *name);
void free_symbol_table(symbol_node *head);

#endif