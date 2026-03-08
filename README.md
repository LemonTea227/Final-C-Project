# Assembler Project - Virtual 12-Bit CPU 

This project is a custom two-pass assembler written in pure ANSI C. It translates a specific assembly language into a 12-bit machine code format, generating object code, entry, and external reference files. 



## Features
* **Strict ANSI C Compliance:** Compiled with `-Wall -ansi -pedantic`, ensuring zero warnings and strict memory safety.
* **Pre-Assembler (Macro Expansion):** Automatically detects, stores, and expands macros (`mcro` / `mcroend`) before the main assembly process begins.
* **Two-Pass Architecture:** * **Pass 1:** Validates syntax, calculates memory addresses (IC/DC), and builds the symbol table.
  * **Pass 2:** Encodes operations, resolves symbol addresses, and generates final binary outputs.
* **Robust Error Handling:** Scans the entire file for syntax, memory, and logical errors, reporting the exact line numbers without crashing.

## Project Structure
The project is highly modular, split into specific functional components:

* `assembler.c` / `assembler.h`: The main entry point. Coordinates the processing of multiple files.
* `pre_assembler.c` / `pre_assembler.h`: Handles the macro unrolling phase.
* `first_pass.c` / `first_pass.h`: Analyzes syntax, updates counters, and populates the symbol table.
* `second_pass.c` / `second_pass.h`: Translates commands into 12-bit machine words and writes to output files.
* `macro.c` / `macro.h`: Manages the dynamic linked list for macro definitions.
* `symbols.c` / `symbols.h`: Manages the dynamic linked list for the symbol table.
* `utils.c` / `globals.h`: Contains shared constants, memory allocation wrappers (`safe_malloc`), and string manipulation helpers.

## Getting Started

### Prerequisites
* GCC Compiler
* Linux/Ubuntu environment (or WSL on Windows)
* Make

### Compilation
To build the project, run the provided Makefile in the terminal:
```bash
make
