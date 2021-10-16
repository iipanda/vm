#include <hashmap.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t opcode_t;

// opcodes
#define op_mov 0x00
#define op_inc 0x01
#define op_dec 0x02

int emit_opcode(FILE *file, opcode_t opcode) {
  return fwrite(&opcode, sizeof(opcode), 1, file);
}

// interpret the bytecode
void interpreter(const char *file) {
  FILE *bin = fopen(file, "rb");

  mem_t memory = make_memory();

  // check the file length
  fseek(bin, 0L, SEEK_END);
  long filelen = ftell(bin);
  rewind(bin);

  // counter to keep track of how many bytes we read
  int i = 0;

  while (i < filelen) {
    char opcode;
    fread(&opcode, sizeof(char), 1, bin);
    i++;

    if (opcode == op_mov) {
      long value;
      fread(&value, 1, sizeof(long), bin);
      i += sizeof(long);

      int curri = 0;
      char *reg = calloc(1024, sizeof(char));

      while (true) {
        fread(reg + curri, sizeof(char), 1, bin);
        if (reg[curri] == 0x0)
          break;
        curri++;
      }

      i += curri + 1;

      mem_put(memory, reg, (void *)value);

      printf("mov %ld %s\n", value, reg);
      mem_dump(memory);
      printf("\n");
    } else if (opcode == op_inc) {
      int curri = 0;
      char *reg = calloc(1024, sizeof(char));

      while (true) {
        fread(reg + curri, sizeof(char), 1, bin);
        if (reg[curri] == 0x0)
          break;
        curri++;
      }

      i += curri + 1;

      mem_put(memory, reg, (void *)((long)mem_get(memory, reg) + 1));

      printf("inc %s\n", reg);
      mem_dump(memory);
      printf("\n");
    } else if (opcode == op_dec) {
      int curri = 0;
      char *reg = calloc(1024, sizeof(char));

      while (true) {
        fread(reg + curri, sizeof(char), 1, bin);
        if (reg[curri] == 0x0)
          break;
        curri++;
      }

      i += curri + 1;

      mem_put(memory, reg, (void *)((long)mem_get(memory, reg) - 1));

      printf("dec %s\n", reg);
      mem_dump(memory);
      printf("\n");
    }
  }
}

void compile(const char *infile, const char *outfile) {
  FILE *in_f = fopen(infile, "r");
  FILE *out_f = fopen(outfile, "wb");

  // stores the current instruction
  char *buf = calloc(1024, sizeof(char));
  // current char
  char current;
  // keep track of how many chars we read
  size_t i = 0;

  while ((current = fgetc(in_f))) {

// macro to define the if's easier
#define is(op) strcmp(op, buf) == 0

    // if this condition passes that means the current part ended
    if (current == ' ' || current == '\n' || current == EOF) {
      if (is("mov")) {
        emit_opcode(out_f, op_mov);
      } else if (is("dec")) {
        emit_opcode(out_f, op_dec);
      } else if (is("inc")) {
        emit_opcode(out_f, op_inc);
      } else if (buf[0] == '%') {
        fwrite(buf + 1, strlen(buf), 1, out_f);
      } else {
        long parsed = atol(buf);
        fwrite(&parsed, sizeof(parsed), 1, out_f);
      }

      // reset the buffer and char count
      buf = calloc(1024, sizeof(char));
      i = 0;

      // break at the end of file
      if (current == EOF) {
        break;
      }

      continue;
    }

    // append current character to the buffer
    buf[i] = current;
    // increment the current char count
    i++;
  }

  fclose(out_f);
  fclose(in_f);
}

int main(void) {

  compile("example.vl", "bin");

  interpreter("bin");

  return 0;
}
