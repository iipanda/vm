#include <futils.h>
#include <hashmap.h>
#include <ops.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define is(op) strcmp(op, buf) == 0

void interpreter(const char* file) {
  FILE* bin = fopen(file, "rb");

  map_t memory = map_new();

  long flength = flen(bin);
  int bytes_read = 0;

  while (bytes_read < flength) {
    // read the opcode
    char opcode;
    fread(&opcode, sizeof(char), 1, bin);
    bytes_read++;

    if (opcode == op_mov) {
      long value;
      fread(&value, 1, sizeof(long), bin);
      bytes_read += sizeof(long);

      char* reg = calloc(1024, sizeof(char));
      bytes_read += readstring(bin, reg);

      map_put(&memory, reg, (void*)value);

      printf("mov %ld %s\n", value, reg);
      map_print(&memory);
      printf("\n");
    } else if (opcode == op_inc) {
      char* reg = calloc(1024, sizeof(char));
      bytes_read += readstring(bin, reg);

      map_put(&memory, reg, (void*)((long)map_get(&memory, reg) + 1));

      printf("inc %s\n", reg);
      map_print(&memory);
      printf("\n");
    } else if (opcode == op_dec) {
      char* reg = calloc(1024, sizeof(char));
      bytes_read += readstring(bin, reg);

      map_put(&memory, reg, (void*)((long)map_get(&memory, reg) - 1));

      printf("dec %s\n", reg);
      map_print(&memory);
      printf("\n");
    } else if (opcode == op_add) {
      char* reg_a = calloc(1024, sizeof(char));
      bytes_read += readstring(bin, reg_a);

      char* reg_b = calloc(1024, sizeof(char));
      bytes_read += readstring(bin, reg_b);

      long a = (long)map_get(&memory, reg_a);
      long b = (long)map_get(&memory, reg_b);
      map_put(&memory, reg_a, (void*)(a + b));

      printf("add %s %s\n", reg_a, reg_b);
      map_print(&memory);
      printf("\n");
    }
  }
}

void compile(const char* infile, const char* outfile) {
  FILE* in_f = fopen(infile, "r");
  FILE* out_f = fopen(outfile, "wb");

  // stores the current instruction
  char* buf = calloc(1024, sizeof(char));
  // current char
  char current;
  // keep track of how many chars we read
  size_t i = 0;

  while ((current = fgetc(in_f))) {

    // if this condition passes that means the current part (or the file) ended
    if (current == ' ' || current == '\n' || current == EOF) {
      if (is("mov")) {
        emit_opcode(out_f, op_mov);
      } else if (is("dec")) {
        emit_opcode(out_f, op_dec);
      } else if (is("inc")) {
        emit_opcode(out_f, op_inc);
      } else if (is("add")) {
        emit_opcode(out_f, op_add);
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
