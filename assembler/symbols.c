/*
 * File: symbols.c
 * Description: Implementation of the Symbol Table management functions.
 */

#include "symbols.h"

/*
 * Function: add_symbol
 * Description: Adds a new symbol to the end of the symbol table linked list.
 * Receives: head (pointer to the head pointer of the list), name (symbol name),
 * address (memory address of the symbol), type (code/data/extern/entry).
 * Returns: None.
 */
void add_symbol(symbol_node **head, const char *name, int address, int type) {
    symbol_node *new_node = (symbol_node *)safe_malloc(sizeof(symbol_node));
    symbol_node *current = *head;

    strcpy(new_node->name, name);
    new_node->address = address;
    new_node->type = type;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

/*
 * Function: find_symbol
 * Description: Searches for a symbol by its name in the linked list.
 * Receives: head (pointer to the start of the list), name (the symbol name to find).
 * Returns: A pointer to the symbol_node if found, or NULL if not found.
 */
symbol_node *find_symbol(symbol_node *head, const char *name) {
    symbol_node *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/*
 * Function: free_symbol_table
 * Description: Safely frees all memory allocated for the symbol table.
 * Receives: head (pointer to the start of the list).
 * Returns: None.
 */
void free_symbol_table(symbol_node *head) {
    symbol_node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}