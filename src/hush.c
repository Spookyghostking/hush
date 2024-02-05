#define HUSH_IMPLEMENTATION
#include"hush.h"
#define NOB_IMPLEMENTATION
#include"nob.h"


int main(void) {
    struct hush_map_llb hashmap = {0};
    printf("capacity is: %lli\n", hashmap.capacity);

    hush_set_capacity(&hashmap, 3);
    printf("capacity is: %lli\n", hashmap.capacity);

    hush_set_value(&hashmap, "boi", 1);
    hush_set_value(&hashmap, "sad", -331);
    hush_set_value(&hashmap, "happy", 3);
    hush_show(hashmap);
    hush_set_value(&hashmap, "happy", -3);
    hush_set_value(&hashmap, "sad", 331);
    hush_show(hashmap);

    hush_set_capacity(&hashmap, 50);
    printf("capacity is: %lli\n", hashmap.capacity);
    hush_dsa_show(hush_get_keys(hashmap));

    hush_show(hashmap);

    nob_String_Builder sb = {0};
    if (!nob_read_entire_file("Shakespeare.txt", &sb))
        return 1;
    const char* contents = nob_string_builder_to_cstr(sb);
    printf("%s\n", contents);


    return 0;
}
