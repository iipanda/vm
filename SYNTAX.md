# The syntax

## Overview

The language consists of instructions each one placed on a separate line.

Variables (aka registers) can currently only be numbers and are written as a `%` character followed by any sequence of characters other than space or new line.

## Instructions

### `mov [n1: number] [v1: variable]`

Sets the value of the variable v1 to the number n1;

### `inc [v1: variable]`

Adds 1 to the v1 variable.

### `dec [v1: variable]`

Subtracts 1 from the v1 variable.

### `add [v1: variable] [v2: variable]`

Adds the values of v1 and v2 and stores the result in v1.
