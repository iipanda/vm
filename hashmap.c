#include <hashmap.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// FNV-1 hash function
uint32_t fnv1(char *input) {
  uint32_t hash = FNV_OFFSET_BASIS;

  for (size_t i = 0; i < strlen(input); i++) {
    hash *= FNV_PRIME;
    hash ^= input[i];
  }

  return hash;
}

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