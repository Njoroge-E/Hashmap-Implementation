#include "hashmap.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


int main(void) {
    HashMap *map = hashmap_init(5);

    printf("total bukets: %zu\n", map->bucket_count);

    assert(hashmap_insert(map, "john", (Details){.birth_year=2006, .first_name="John", .last_name="Kangaroo"}));
    assert(hashmap_insert(map, "jeff", (Details){.birth_year=2000, .first_name="Jeff", .last_name="Andrews"}));
    assert(hashmap_insert(map, "nick", (Details){.birth_year=2002, .first_name="Nick", .last_name="Rhumba"}));
    assert(hashmap_insert(map, "mike", (Details){.birth_year=2002, .first_name="Mike", .last_name="Karzonski"}));
    assert(hashmap_insert(map, "emma", (Details){.birth_year=2005, .first_name="Emma", .last_name="Mariam"}));
    assert(hashmap_insert(map, "alex", (Details){.birth_year=2004, .first_name="Alex", .last_name="Montrean"}));
    assert(hashmap_insert(map, "lisa", (Details){.birth_year=2006, .first_name="Lisa", .last_name="Fairy"}));
    assert(hashmap_insert(map, "anna", (Details){.birth_year=2002, .first_name="Anna", .last_name="Mickleberry"}));
    assert(hashmap_insert(map, "anna", (Details){.birth_year=2002, .first_name="Analisa", .last_name="Mickleberry"}));

    printf("buckets used: %zu\n", map->bucket_size);
    printf("total bukets: %zu\n", map->bucket_count);
    
    assert(hashmap_insert(map, "aztec", (Details){.birth_year=2002, .first_name="Anna", .last_name="Mickleberry"}));
    assert(hashmap_insert(map, "michalangelo", (Details){.birth_year=2002, .first_name="Anna", .last_name="Mickleberry"}));
    assert(hashmap_insert(map, "lenna", (Details){.birth_year=2002, .first_name="Anna", .last_name="Mickleberry"}));
    assert(hashmap_insert(map, "anna", (Details){.birth_year=2002, .first_name="Anastasia", .last_name="Mickleberry"}));

    printf("buckets used: %zu\n", map->bucket_size);
    printf("total bukets: %zu\n", map->bucket_count);


    hashmap_delete(map);
    return 0;
}