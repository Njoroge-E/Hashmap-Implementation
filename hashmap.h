#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    uint16_t birth_year;
    char *first_name;
    char *last_name;
} Details;

typedef enum {
    EMPTY = 0,
    OCCUPIED = 1,
    TOMBSTONE = 2
} SlotState;

typedef struct {
    char *username;
    Details details;
    SlotState state;
} Person;

typedef struct {
    Person *array;
    size_t capacity;
    size_t num_elements;
} HashMap;

/* Public API */
HashMap *hashmap_init(size_t cap);
bool hashmap_insert(HashMap *map, const char *key, Details details);
Person *hashmap_find(HashMap *map, const char *key);
bool hashmap_remove_entry(HashMap *map, const char *key);
void hashmap_delete(HashMap *map);
