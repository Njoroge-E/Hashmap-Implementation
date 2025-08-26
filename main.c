// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

void printSlots(SlotState state) {
    switch (state) {
        case EMPTY:
            printf("Empty\n");
            break;
        case OCCUPIED: 
            printf("Occupied\n");
            break;
        case TOMBSTONE:
            printf("Tombstone\n");
            break;
    }
}


int main(void) {
    HashMap *map = hashmap_init(10);

    for (size_t i = 0; i < map->capacity; ++i) {
        printf("Slot %zu:\t", i);
        printSlots(map->array[i].state);
    }
    putchar('\n');

    hashmap_insert(map, "john", (Details){.birth_year=2006, .first_name="John", .last_name="Kangaroo"});
    hashmap_insert(map, "jeff", (Details){.birth_year=2000, .first_name="Jeff", .last_name="Andrews"});
    hashmap_insert(map, "nick", (Details){.birth_year=2002, .first_name="Nick", .last_name="Rhumba"});
    hashmap_insert(map, "mike", (Details){.birth_year=2002, .first_name="Mike", .last_name="Karzonski"});
    hashmap_insert(map, "emma", (Details){.birth_year=2005, .first_name="Emma", .last_name="Mariam"});
    hashmap_insert(map, "alex", (Details){.birth_year=2004, .first_name="Alex", .last_name="Montrean"});
    hashmap_insert(map, "lisa", (Details){.birth_year=2006, .first_name="Lisa", .last_name="Fairy"});
    hashmap_insert(map, "anna", (Details){.birth_year=2002, .first_name="Anna", .last_name="Mickleberry"});

    for (size_t i = 0; i < map->capacity; ++i) {
        printf("Slot %zu:\t", i);
        printSlots(map->array[i].state);
    }
    putchar('\n');

    Person *p = NULL;
    p = hashmap_find(map, "john");
    printf("john %s\n", p ? "found" : "not found");
    p = hashmap_find(map, "linda");
    printf("linda %s\n", p ? "found" : "not found");

    hashmap_remove_entry(map, "nick");
    p = hashmap_find(map, "nick");
    printf("nick %s\n", p ? "found" : "not found");

    for (size_t i = 0; i < map->capacity; ++i) {
        printf("Slot %zu:\t", i);
        printSlots(map->array[i].state);
    }
    putchar('\n');

    hashmap_insert(map, "aztec", (Details){.birth_year=2002, .first_name="Anna", .last_name="Mickleberry"});
    hashmap_insert(map, "michalangelo", (Details){.birth_year=2002, .first_name="Anna", .last_name="Mickleberry"});
    hashmap_insert(map, "lenna", (Details){.birth_year=2002, .first_name="Anna", .last_name="Mickleberry"});
    hashmap_insert(map, "anna", (Details){.birth_year=2002, .first_name="Anastasia", .last_name="Mickleberry"});

    for (size_t i = 0; i < map->capacity; ++i) {
        printf("Slot %zu:\t", i);
        printSlots(map->array[i].state);
    }
    putchar('\n');

    hashmap_delete(map);
    free(map);
    return 0;
}
