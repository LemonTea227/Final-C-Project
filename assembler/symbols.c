/*
 * symbols.c
 * Implements the symbol table management functions for the assembler.
 */

#include "symbols.h"

/*
 * Adds a new symbol to the end of the symbol table linked list.
 * Parameters:
 *   head    - pointer to the head pointer of the list
 *   name    - symbol name
 *   address - memory address of the symbol
 *   type    - code/data/extern/entry
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
 * Searches for a symbol by its name in the linked list.
 * Returns pointer to symbol_node if found, or NULL if not found.
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
 * Frees all memory allocated for the symbol table.
 * Parameter: head - pointer to the start of the list.
 */
void free_symbol_table(symbol_node *head) {
    symbol_node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}