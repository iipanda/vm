#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FNV_OFFSET_BASIS 2166136261;
#define FNV_PRIME 16777619

uint32_t fnv1(char *input) {
  uint32_t hash = FNV_OFFSET_BASIS;

  for (size_t i = 0; i < strlen(input); i++) {
    hash *= FNV_PRIME;
    hash ^= input[i];
  }

  return hash;
}

typedef uint8_t opcode_t;

#define op_mov 0x00
#define op_inc 0x01
#define op_dec 0x02

typedef void *oparg_t;

int emit_opcode(FILE *file, opcode_t opcode) {
  return fwrite(&opcode, sizeof(opcode), 1, file);
}

#define MEMSIZE 100

typedef struct mem_register {
  char *reg;
  void *val;
  struct mem_register *next;
} mem_register_t;

typedef mem_register_t **mem_t;

void mem_dump(mem_t mem) {
  printf("{ ");
  for (size_t i = 0; i < MEMSIZE; i++) {
    mem_register_t *mem_reg = mem[i];
    while (mem_reg != NULL) {
      printf("%s:0x%lX ", mem_reg->reg, (long)mem_reg->val);
      mem_reg = mem_reg->next;
    }
  }
  printf("}\n");
}

void mem_put(mem_t mem, char *reg, void *value) {
  size_t idx = fnv1(reg) % MEMSIZE;
  mem_register_t *mem_reg = mem[idx];

  mem_register_t *new_reg = malloc(sizeof(mem_register_t));
  new_reg->reg = reg;
  new_reg->val = value;
  new_reg->next = NULL;

  if (mem_reg == NULL) {
    mem[idx] = new_reg;
    return;
  }

  while (strcmp(reg, mem_reg->reg) != 0 && mem_reg->next) {
    mem_reg = mem_reg->next;
  }

  if (strcmp(reg, mem_reg->reg) != 0) {
    mem_reg->next = new_reg;
  } else {
    mem[idx] = new_reg;
  }
}

void *mem_get(mem_t mem, char *reg) {
  uint32_t idx = fnv1(reg) % MEMSIZE;
  mem_register_t *mem_reg = mem[idx];

  if (mem_reg == NULL) {
    return NULL;
  }

  while (strcmp(reg, mem_reg->reg) != 0 && mem_reg->next) {
    mem_reg = mem_reg->next;
  }

  return mem_reg->val;
}

mem_t make_memory() {
  return calloc(MEMSIZE, sizeof(void *));
}

void interpreter(const char *file) {
  FILE *bin = fopen(file, "rb");

  mem_t memory = make_memory();

  fseek(bin, 0L, SEEK_END);
  long filelen = ftell(bin);
  rewind(bin);

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
        if (reg[curri] == 0x0) break;
        curri++;
      }

      i += curri + 1;

      mem_put(memory, reg, (void*)value);

      printf("mov %ld %s\n", value, reg);
      mem_dump(memory);
      printf("\n");
    } else if (opcode == op_inc) {
      int curri = 0;
      char *reg = calloc(1024, sizeof(char));

      while (true) {
        fread(reg + curri, sizeof(char), 1, bin);
        if (reg[curri] == 0x0) break;
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
        if (reg[curri] == 0x0) break;
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

int main(void) {
  FILE *input = fopen("incdec.vl", "r");
  FILE *bin = fopen("bin", "wb");
  
  char *buf = calloc(1024, sizeof(char));
  char current;
  size_t i = 0;

  while ((current = fgetc(input))) {

    if (current == ' ' || current == '\n' || current == EOF) {
      if (strcmp("mov", buf) == 0) {
        emit_opcode(bin, op_mov);
      } else if (strcmp("dec", buf) == 0) {
        emit_opcode(bin, op_dec);
      } else if (strcmp("inc", buf) == 0) {
        emit_opcode(bin, op_inc);
      } else if (buf[0] == '%') {
        fwrite(buf + 1, strlen(buf), 1, bin);
      } else {
        long parsed = atol(buf);
        fwrite(&parsed, sizeof(parsed), 1, bin);
      }

      buf = calloc(1024, sizeof(char));
      i = 0;

      if (current == EOF) {
        break;
      }

      continue;
    }

    buf[i] = current;
    i++;
  }

  fclose(bin);
  fclose(input);

  interpreter("bin");

  return 0;
}
