#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <stdint.h>

// FNV-1 hash constants
#define FNV_OFFSET_BASIS 2166136261;
#define FNV_PRIME 16777619

uint32_t fnv1(char *input);

// the virtual machine memory (a hashmap)
#define MEMSIZE 100

typedef struct mem_register {
  char *reg;
  void *val;
  struct mem_register *next;
} mem_register_t;

typedef mem_register_t **mem_t;

// print the whole memory contents, used for debugging purposes
void mem_dump(mem_t mem);

void mem_put(mem_t mem, char *reg, void *value);

void *mem_get(mem_t mem, char *reg);

// create the hashmap
mem_t make_memory();

#endif