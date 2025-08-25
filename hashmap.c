#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Person {
    char *key;
    int age;
} Person;

typedef struct HashMap {
    Person *array;
    size_t capacity;
    size_t num_elements;
} HashMap;

HashMap *hashmap_init(int num_of_elements) {
    HashMap *map;
    map->array = calloc(num_of_elements, sizeof(Person));
    if (!map) {
        perror("Malloc Failed!\n");
        return NULL;
    }
    return map;
}

int hash(char *key) {
    return 0;
}