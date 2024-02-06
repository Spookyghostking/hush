#include<assert.h>

#define HUSH_IMPLEMENTATION
#include"hush.h"


int main(void) {
    struct hush_map_llb hashmap = {0};
    hush_set_capacity(&hashmap, 5);
    
    for (size_t i = 0; i < hashmap.capacity; i++) {
        assert(hashmap.items[i] == NULL && "Improper hush_map initialization\n");
    }

    hush_set_value(&hashmap, "city", 2);

    assert(hush_has_key(hashmap, "city") && "Key should already exist\n");
    assert(!hush_has_key(hashmap, "cutie") && "Key should not exist\n");




    return 0;
}
