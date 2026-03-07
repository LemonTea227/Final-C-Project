/*
 * File: utils.c
 * Description: Contains utility functions used across the assembler program, 
 * such as safe memory allocation.
 */

#include "globals.h"

/*
 * Function: safe_malloc
 * Description: Allocates memory and immediately checks if the allocation was successful.
 * If the allocation fails, it prints an error and terminates the program.
 * Receives: size (the amount of bytes to allocate).
 * Returns: A pointer to the allocated memory.
 */
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    
    /* Check if memory allocation succeeded */
    if (ptr == NULL) {
        fprintf(stderr, "Fatal Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE); /* Exit immediately with error code */
    }
    
    return ptr;
}