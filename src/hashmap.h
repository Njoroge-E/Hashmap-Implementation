#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    uint16_t birth_year;
    char *first_name;
    char *last_name;
} Details;

typedef struct Person{
    char *username;     // key
    Details details;    // value
    struct Person *next;
} Person;

typedef struct {
    Person **array;
    size_t bucket_count;
    size_t bucket_size;
    size_t size;        // num_of_elements
} HashMap;


/* Public API */

HashMap *hashmap_init(size_t bucket_count);
bool hashmap_insert(HashMap *map, const char *key, Details details);
Person *hashmap_find(HashMap *map, const char *key);
bool hashmap_remove_entry(HashMap *map, const char *key);
void hashmap_delete(HashMap *map);
