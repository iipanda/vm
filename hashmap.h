#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <stdint.h>
#include <stdlib.h>

// FNV-1 hash constants
#define FNV_OFFSET_BASIS 2166136261;
#define FNV_PRIME 16777619

uint32_t fnv1(char* input);

#define MAP_BASE_SIZE (size_t)100

typedef struct map_item {
  char* key;
  void* value;
  uint32_t hash;
  struct map_item* next;
} map_item_t;

typedef map_item_t** map_items_t;

typedef struct map {
  map_items_t items;
  size_t size;
} map_t;

void map_print(map_t* map);

void map_put(map_t* map, char* reg, void* value);

void* map_get(map_t* map, char* reg);

map_t map_new();

map_item_t item_new();

#endif