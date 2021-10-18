#ifndef __OPS_H
#define __OPS_H

#include <stdint.h>
#include <stdio.h>

typedef uint8_t opcode_t;

#define op(c) (opcode_t)(c)

// opcodes
#define op_mov op(0x00)
#define op_inc op(0x01)
#define op_dec op(0x02)
#define op_add op(0x03)

int emit_opcode(FILE* file, opcode_t opcode);

#endif