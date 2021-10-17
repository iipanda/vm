# The syntax

## The basics

The language consists of instructions each one placed on a separate line.

Variables are written as a `%` character followed by any sequence of characters other than space or new line and their value can only be numbers for now.

## Instructions

### `mov [n1: number] [v1: variable]`

Sets the value of the variable v1 to the number n1;

### `inc [v1: variable]`

Adds 1 to the v1 variable.

### `dec [v1: variable]`

Subtracts 1 from the v1 variable.

### `add [v1: variable] [v2: variable]`

Adds the values of v1 and v2 and stores the result in v1.

## Proposed instructions (not implemented)

### `mov [v1: variable] [v2: variable]`

Stores the value of variable v2 in v1.

### `prin [v1: variable]`

Prints the v1 variable's value to the screen.

### `prin [n1: number]`

Prints the n1 number to the screen.

## Proposed changes

### Change `mov` argument order to be consistent with other instructions

`mov [n1: number] [v1: variable]` -> `mov [v1: variable] [n1: number]`
