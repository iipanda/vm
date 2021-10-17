#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <stdint.h>

// FNV-1 hash constants
#define FNV_OFFSET_BASIS 2166136261;
#define FNV_PRIME 16777619

uint32_t fnv1(char *input);

#define MAPSIZE 100

typedef struct map_bucket {
  char *reg;
  void *val;
  struct map_bucket *next;
} map_bucket_t;

typedef map_bucket_t **map_t;

// print the whole memory contents, used for debugging purposes
void map_print(map_t mem);

void map_put(map_t mem, char *reg, void *value);

void *map_get(map_t mem, char *reg);

// create the hashmap
map_t map_new();

#endif