/*
 * File: macro.h
 * Description: Data structures for macro management.
 */
#ifndef MACRO_H
#define MACRO_H

#include "globals.h"

/* Linked list node for a macro */
typedef struct macro_node {
    char name[MAX_LABEL_LEN];
    char *content; /* Dynamically allocated string holding the macro lines */
    struct macro_node *next;
} macro_node;

void add_macro(macro_node **head, const char *name, const char *content);
macro_node *find_macro(macro_node *head, const char *name);
void free_macro_table(macro_node *head);

#endif