#include <hashmap.h>
#include <stdbool.h>
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
  printf("{\n");

  for (size_t i = 0; i < map->size; i++) {
    map_items_t items = map->items;
    map_item_t* item = items[i];

    while (item != NULL) {
      printf("  %s: 0x%lX\n", item->key, (long)item->value);
      item = item->next;
    }
  }

  printf("}\n");
}

// TODO: there's a memory leak here because we don't free the old item
void map_put(map_t map, char* key, void* value) {
  uint32_t hash = fnv1(key);
  size_t idx = hash % map->size;

  map_item_t* new_item = malloc(sizeof(map_item_t));
  new_item->key = key;
  new_item->value = value;
  new_item->next = NULL;
  new_item->hash = hash;

  map_items_t items = map->items;
  map_item_t* item = items[idx];

  if (item == NULL) {
    items[idx] = new_item;
    return;
  }

  bool match = false;
  map_item_t* previous_item = NULL;

  while (!match && item->next) {
    previous_item = item;
    item = item->next;
    match = hash == item->hash && strcmp(key, item->key) == 0;
  }

  if (strcmp(key, item->key) == 0) {
    if (previous_item == NULL) {
      items[idx] = new_item;
    } else {
      new_item->next = item->next;
      previous_item->next = new_item;
    }
  } else {
    item->next = new_item;
  }
}

void* map_get(map_t map, char* key) {
  uint32_t hash = fnv1(key);
  size_t idx = hash % map->size;

  map_item_t* item = map->items[idx];

  if (item == NULL) {
    return NULL;
  }

  while (hash != item->hash && strcmp(key, item->key) != 0 && item->next) {
    item = item->next;
  }

  return item->value;
}

map_t map_new() {
  size_t size = MAP_BASE_SIZE;

  map_t map = calloc(1, sizeof(map_t));

  map->items = calloc(size, sizeof(void*));
  map->size = size;

  return map;
}