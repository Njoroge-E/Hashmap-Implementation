#include "hashmap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *str = malloc(n);
    if (!str) {
        perror("strdup_safe malloc failed!\n");
        return NULL;
    }
    memcpy(str, s, n);
    return str;
}


static uint32_t hash_sum(const char *key) {
    uint32_t num = 0; 
    for (const unsigned char *p = (const unsigned char *)key; *p; ++p) {
        num += *p;
    }
    return num;
}


static void person_free(Person *p) {
    if (!p) return;
    if (p->username) {
        free(p->username);
        p->username = NULL;
    }
    if (p->details.first_name) {
        free(p->details.first_name);
        p->details.first_name = NULL;
    }
    if (p->details.last_name) {
        free(p->details.last_name);
        p->details.last_name = NULL;
    }
    p->state = EMPTY;
}


void hashmap_delete(HashMap *map) {
    if (!map) return;
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->array[i].state == OCCUPIED) {
            person_free(&map->array[i]);
        }
        else {
            // defensive for TOMBSTONE
            person_free(&map->array[i]);
        }
    }
    free(map->array);
    map->array = NULL;
    map->capacity = map->num_elements = 0;
}


static bool person_init(Person *p, const char *username, const Details *d) {
    if (!p || !username || !d) return false;
    p->username = strdup_safe(username);
    if (!p->username) return false;
    p->details.first_name = d->first_name ? strdup_safe(d->first_name) : NULL;
    p->details.last_name = d->last_name ? strdup_safe(d->last_name) : NULL;
    p->details.birth_year = d->birth_year;
    p->state = OCCUPIED;
    return true;
}


HashMap *hashmap_init(size_t cap) {
    if (cap == 0) cap = 10;
    HashMap *map = calloc(1, sizeof(HashMap));
    if (!map) {
        perror("calloc'ing map failed!\n");
        return NULL;
    }
    map->array = calloc(cap, sizeof(Person));
    if (!map->array) {
        perror("calloc'ing map->array failed!\n");
        return NULL;
    }
    map->capacity = cap;
    map->num_elements = 0;
    return map;
}


static double load_factor(const HashMap *map) {
    if (!map || map->capacity == 0) return 0.0;
    return (double)map->num_elements / (double)map->capacity;
}


static bool resize_array(HashMap *map) {
    if (!map) return false;
    size_t new_cap = map->capacity * 2;
    Person *new_array = calloc(new_cap, sizeof(Person));
    if (!new_array) {
        perror("calloc'ing Person array in resizing failed!\n");
        return false;
    }
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->array[i].state == OCCUPIED) {
            uint32_t idx = hash_sum(map->array[i].username) % (uint32_t)new_cap;
            // linear probing
            while (new_array[idx].state == OCCUPIED) {
                idx = (idx + 1) % (uint32_t)new_cap;
            }
            // shallow copy
            new_array[idx].username = map->array[i].username;
            new_array[idx].details = map->array[i].details;
            new_array[idx].state = OCCUPIED;
            // null pointers to avoid double free
            map->array[i].username = NULL;
            map->array[i].details.first_name = NULL;
            map->array[i].details.last_name = NULL;
            map->array[i].state = EMPTY;
        }
    }
    // free old array
    free(map->array);
    map->array = new_array;
    map->capacity = new_cap;
    return true;
}


Person *hashmap_find(HashMap *map, const char *key) {
    if (!map || !key) return NULL;
    size_t cap = map->capacity;
    uint32_t idx = hash_sum(key) % (uint32_t)cap;

    for (size_t probes = 0; probes < cap; ++probes) {
        Person *p = &map->array[idx];
        if (p->state == EMPTY) return NULL;
        if (p->state == OCCUPIED && p->username && strcmp(key, p->username) == 0) {
            return p;
        }
        idx = (idx + 1) % cap;
    }
    return NULL;

}


bool hashmap_remove_entry(HashMap *map, const char *key) {
    if (!map || !key) return false;
    size_t cap = map->capacity;
    uint32_t idx = hash_sum(key) % (uint32_t)cap;

    for (size_t probes = 0; probes < cap; ++probes) {
        Person *p = &map->array[idx];
        if (p->state == EMPTY) return false;
        if (p->state == OCCUPIED && p->username && strcmp(p->username, key) == 0) {
            // free heap pointers and TOMBSTONE it
            person_free(p);
            p->state = TOMBSTONE;
            if (map->num_elements > 0) map->num_elements--;
            return true;
        }
        idx = (idx + 1) % (uint32_t)cap;
    }
    return false;
}


bool hashmap_insert(HashMap *map, const char *key, Details details) {
    if (!map || !key) return false;

    if (load_factor(map) >= 0.7) {
        if (!resize_array(map)) return false;
    }

    size_t cap = map->capacity;
    uint32_t idx = hash_sum(key) % (uint32_t)cap;

    // probing
    for (size_t probes = 0; probes < cap; ++probes) {
        Person *p = &map->array[idx];

        if (p->state == EMPTY) {
            if (!person_init(p, key, &details)) return false;
            map->num_elements++;
            return true;
        }
        else if (p->state == TOMBSTONE) {
            if (!person_init(p, key, &details)) return false;
            map->num_elements++; 
            return true;

        }
        // if p->state == OCCUPIED
        else {
            if (p->username && strcmp(p->username, key) == 0) {
                // free earlier heap pointers and copy new ones
                free(p->details.first_name);
                free(p->details.last_name);
                p->details.first_name = details.first_name ? strdup_safe(details.first_name) : NULL;
                p->details.last_name = details.last_name ? strdup_safe(details.last_name) : NULL;
                p->details.birth_year = details.birth_year;
                return true;
            }
        }
        idx = (idx + 1) % (uint32_t)cap;
    }
    // shouldn't happen because we resize
    return false;
}




















