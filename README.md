# Overview

This repository contains a fully functional assembler for the Hack assembly language.

The assembler converts `.asm` files into `.hack` binary files executable on the Hack platform simulator.

> ⚠️ Note: `unistd.h` is used which works only on Linux/macOS.

## Features

- Converts **A-instructions** (`@value`) and **C-instructions** (`dest=comp;jmp`) to 16-bit binary.
- Handles **labels** and **variables** automatically with a symbol table.
- Produces clean `.hack` output files which can be ran on the [Hack CPU simulator](https://nand2tetris.github.io/web-ide/cpu).

## How to use

- clone repo
  ```
  git clone https://github.com/yourusername/nand2tetris1.git
  cd nand2tetris1
  ```
- Build
  ```
  make
  ```
- Run the executable with the name of the `.asm` file. E.g,
  ```
  build/main Add.asm
  ```

## Architecture and Design

- `main.c` performs file handling and calls the assemble function from `assembler.c` which then performs the assembly operations.
- `symbol_table.c` provides an API for the lookup table which stores all symbols.
- `utils.c` provides helper functions such as decimal-to-binary operations, string operations etc.
- header files are hosted in `/include` and the source files in `\src`.
- The `Makefile` provides build instructions, and compiles to `\build`. The final executable is `main`.
