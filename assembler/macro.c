#include "macro.h"

/*
 * Adds a new macro definition to the macro table linked list.
 * Parameters:
 *   head    - pointer to macro table head
 *   name    - macro name
 *   content - macro body text
 */
void add_macro(macro_node **head, const char *name, const char *content) {
    macro_node *new_node = (macro_node *)safe_malloc(sizeof(macro_node));
    macro_node *current = *head;

    strcpy(new_node->name, name);
    new_node->content = (char *)safe_malloc(strlen(content) + 1);
    strcpy(new_node->content, content);
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
 * Searches for a macro by name in the macro table.
 * Returns pointer to macro_node if found, NULL otherwise.
 */
macro_node *find_macro(macro_node *head, const char *name) {
    macro_node *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }
    return NULL;
}

void free_macro_table(macro_node *head) {
    macro_node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->content);
        free(temp);
    }
}