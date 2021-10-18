#include <ops.h>
#include <stdio.h>

int emit_opcode(FILE *file, opcode_t opcode) {
  return fwrite(&opcode, sizeof(opcode), 1, file);
}
