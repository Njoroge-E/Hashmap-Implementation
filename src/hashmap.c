#include "hashmap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


// copy string into heap
static char *strdup_safe(const char *s) {
    size_t len = strlen(s) + 1;
    char *heap_s = malloc(len);
    if (!heap_s) {
        perror("malloc failed! {ref: strdup}\n");
        return NULL;
    }
    memcpy(heap_s, s, len);
    return heap_s;
}

static char *str_reheap(char* earlier_ptr, const char *s) {
    if (earlier_ptr) {
        free(earlier_ptr);
        // No need to NULL earlier_ptr as C passes by value,, and this is now the local copy
    }
    size_t len = strlen(s) + 1;
    char *heap_s = malloc(len);
    if (!heap_s) {
        perror("malloc failed! {ref: strdup}\n");
        return NULL;
    }
    memcpy(heap_s, s, len);
    return heap_s;
}


static uint32_t hash(const char *key) {
    uint32_t num = 0;
    for (const unsigned char *p = (const unsigned char *)key; *p; ++p) {
        num += *p;
    }
    return num;
}


static double load_factor(HashMap *map) {
    return (double)map->bucket_size / (double)map->bucket_count;
}


static bool front_add_person(HashMap *map, Person** array ,uint32_t index, Person *p) {
    if (array[index]) {
        p->next = array[index];
        array[index] = p;
        ++map->size;
    }
    else {
        p->next = NULL;
        array[index] = p;
        ++map->bucket_size;
        ++map->size;
    }
    return true;

}

static bool resize_array(HashMap *map) {
    size_t old_bucket_count = map->bucket_count;    
    size_t new_bucket_count = old_bucket_count * 2;

    
    Person **new_array = calloc(new_bucket_count, sizeof(Person *));
    if (!new_array) {
        perror("calloc failed! {ref: resize_array}\n");
        return false;
    }
    map->bucket_count = new_bucket_count;
    map->bucket_size = 0;
    map->size  = 0;

    for (size_t i = 0; i < old_bucket_count; ++i) {
        if (map->array[i]) {
            Person *p = map->array[i];
            while (p) {
                Person *next = p->next;
                uint32_t idx = hash(p->username) % (uint32_t)new_bucket_count;
                if (!front_add_person(map, new_array, idx, p)) return false;
                p  = next;
            }
        }
    }

    free(map->array);
    map->array = new_array;

    return true;
}



HashMap *hashmap_init(size_t bucket_count) {
    if (bucket_count == 0 || bucket_count >= 1000) {
        fprintf(stderr, "bucket count error! {ref: hashmap_init}\n");
        bucket_count = 10;
    }
    HashMap *map = calloc(1, sizeof(HashMap));
    if (!map) {
        perror("calloc failed! {ref: hashmap_init}\n");
        return NULL;
    }
    map->array = calloc(bucket_count, sizeof(Person *));
    if (!map->array) {
        perror("calloc failed! {ref: hashmap_init}\n");
        return NULL;
    }
    map->bucket_count = bucket_count;
    return map;
}


static Person *person_init(const char *username, Details *details) {
    if (!username || !details) return NULL;
    Person *p = malloc(sizeof(Person));
    if (!p) {
        perror("malloc failed! {ref: person_init}\n");
        return NULL;
    }
    p->username = strdup_safe(username);
    p->details.birth_year = details->birth_year;
    p->details.first_name = details->first_name ? strdup_safe(details->first_name) : NULL;
    p->details.last_name = details->last_name ? strdup_safe(details->last_name) : NULL;
    p->next = NULL;

    return p;
} 


static Person *person_update(Person *p, const char *username, Details *details) {
    if (!username || !details) return NULL;

    p->details.birth_year = details->birth_year;
    p->details.first_name = details->first_name ? str_reheap(p->details.first_name, details->first_name) : NULL;
    p->details.last_name = details->last_name ? str_reheap(p->details.last_name, details->last_name) : NULL;
    // p->next shouldn't be touched in updates
    return p;
} 




bool hashmap_insert(HashMap *map, const char *key, Details details) {
    if (!map || !key) return false;

    if (load_factor(map) >= 0.7) {
        if (!resize_array(map)) return false;
    }

    uint32_t index = hash(key) % (uint32_t)map->bucket_count;
    Person *p = hashmap_find(map, key);
    if (p) {
        if (!person_update(p, key, &details)) return false;
        return true;
    }
    p = person_init(key, &details);
    if (!front_add_person(map, map->array, index, p)) return false;

    return true;
}


static Person *find_slot(Person **array, uint32_t index, const char *key) {
    if (!array) return NULL;

    for (Person *ptr = array[index]; ptr; ptr = ptr->next) {
        if (strcmp(key, ptr->username) == 0) {
            return ptr;
        }
    }
    return NULL;
}



Person *hashmap_find(HashMap *map, const char *key) {
    if (!map || !key) return NULL;

    uint32_t idx = hash(key) % (uint32_t)map->bucket_count;

    return find_slot(map->array, idx, key);
}


static bool person_free(Person *p) {
    if (!p) return false;
    free(p->username);
    free(p->details.first_name);
    free(p->details.last_name);
    p->next = NULL;
    p->details.first_name = NULL;
    p->details.last_name = NULL;
    p->username = NULL;
    p->details.birth_year = 0;
    free(p);

    return true;
}


bool hashmap_remove_entry(HashMap *map, const char *key) {
    if (!map || !key) return NULL;

    uint32_t idx = hash(key) % (uint32_t)map->bucket_count;

    Person *find_slot_ptr = find_slot(map->array, idx, key);
    if (!find_slot_ptr) return false;

    if (find_slot_ptr == map->array[idx]) {
        map->array[idx] = find_slot_ptr->next; 
        person_free(find_slot_ptr);
        return true;
    }


    Person *prev = map->array[idx];
    while (prev->next != find_slot_ptr) {
        prev = prev->next;
    }

    prev->next = find_slot_ptr->next;
    person_free(find_slot_ptr);

    return true;
}


static bool free_linked_list(Person *p) {
    if (!p) {
        return false;
    }
    while (p != NULL) {
        Person *delete_ptr = p;
        p = p->next;
        if (!person_free(delete_ptr)) return false;
    }
    return true;
}


void hashmap_delete(HashMap *map) {
    if (!map) {
        fprintf(stderr, "map is invalid! {ref: hashmap_delete}\n");
        return;
    }

    for (size_t i = 0; i < map->bucket_count; ++i) {
        if (!free_linked_list(map->array[i])) {
            continue;
        };
    }

    free(map->array);
    map->array = NULL;
    free(map);
    map = NULL;
}















































