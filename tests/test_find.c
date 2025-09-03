#include "hashmap.h"
#include <assert.h>
#include <stdlib.h>

int main(void) {
    HashMap *map = hashmap_init(5);

    assert(hashmap_insert(map, "john", (Details){.birth_year=2006, .first_name="John", .last_name="Kangaroo"}));
    
    assert(hashmap_find(map, "john"));
    assert(hashmap_find(map, "nick") == NULL);


    hashmap_delete(map);
    free(map);
    return 0;
}