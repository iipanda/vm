#include <hashmap.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FNV-1 hash function
uint32_t fnv1(char* input) {
  uint32_t hash = FNV_OFFSET_BASIS;

  for (size_t i = 0; i < strlen(input); i++) {
    hash *= FNV_PRIME;
    hash ^= input[i];
  }

  return hash;
}

void map_print(map_t map) {
  printf("{ ");

  for (size_t i = 0; i < MAP_BASE_SIZE; i++) {
    map_bucket_t* bucket = map[i];
    while (bucket != NULL) {
      printf("%s:0x%lX ", bucket->reg, (long)bucket->val);
      bucket = bucket->next;
    }
  }

  printf("}\n");
}

void map_put(map_t map, char* reg, void* value) {
  size_t idx = fnv1(reg) % MAP_BASE_SIZE;
  map_bucket_t* bucket = map[idx];

  map_bucket_t* new_reg = malloc(sizeof(map_bucket_t));
  new_reg->reg = reg;
  new_reg->val = value;
  new_reg->next = NULL;

  if (bucket == NULL) {
    map[idx] = new_reg;
    return;
  }

  while (strcmp(reg, bucket->reg) != 0 && bucket->next) {
    bucket = bucket->next;
  }

  if (strcmp(reg, bucket->reg) != 0) {
    bucket->next = new_reg;
  } else {
    map[idx] = new_reg;
  }
}

void* map_get(map_t map, char* reg) {
  uint32_t idx = fnv1(reg) % MAP_BASE_SIZE;
  map_bucket_t* bucket = map[idx];

  if (bucket == NULL) {
    return NULL;
  }

  while (strcmp(reg, bucket->reg) != 0 && bucket->next) {
    bucket = bucket->next;
  }

  return bucket->val;
}

map_t map_new() {
  return calloc(MAP_BASE_SIZE, sizeof(void*));
}