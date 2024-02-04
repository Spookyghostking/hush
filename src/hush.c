#define HUSH_IMPLEMENTATION
#include"hush.h"


int main(void) {
    struct hush_map_llb hashmap = {0};
    printf("capacity is: %lli\n", hashmap.capacity);

    hush_set_capacity(&hashmap, 3);
    printf("capacity is: %lli\n", hashmap.capacity);

    hush_set_value(&hashmap, "boi", 1);
    hush_show(hashmap);
    hush_set_value(&hashmap, "sad", -331);
    hush_show(hashmap);
    hush_set_value(&hashmap, "happy", 3);
    hush_show(hashmap);
    printf("%d\n", hush_get_value(hashmap, "boi"));
    printf("%d\n", hush_get_value(hashmap, "sad"));
    printf("%d\n", hush_get_value(hashmap, "happy"));

    hush_set_capacity(&hashmap, 50);
    printf("capacity is: %lli\n", hashmap.capacity);
    hush_dsa_show(hush_get_keys(hashmap));

    hush_show(hashmap);


    return 0;
}
