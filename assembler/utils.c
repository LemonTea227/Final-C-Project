/*
 * utils.c
 * Utility functions for the assembler, such as safe memory allocation.
 */

#include "globals.h"

/*
 * Allocates memory and checks if the allocation was successful.
 * If allocation fails, prints an error and terminates the program.
 * Parameter: size - number of bytes to allocate.
 * Returns: pointer to the allocated memory.
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