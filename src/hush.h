#ifndef HUSH_H_
#define HUSH_H_
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct hush_item_llb{
    const char* key;
    struct hush_item_llb* next;
    int value;
};

struct hush_map_llb {
    size_t capacity;
    struct hush_item_llb** items;
};


#define DSA_INIT_CAP 64
typedef struct {
    const char** items;
    size_t capacity;
    size_t count;
} hush_dynamic_string_array;


long long int hush_fnv1a(const char* data);
size_t hush_get_index(const char* key);
void hush_set_value(struct hush_map_llb* hashmap, const char* key, int value);
bool hush_has_key(struct hush_map_llb hashmap, const char* key);
int hush_get_value(struct hush_map_llb hashmap, const char* key);
const char* hush_get_key(struct hush_map_llb hashmap, int value);
hush_dynamic_string_array hush_get_keys(struct hush_map_llb hashmap);
struct hush_item_llb hush_pop_item(struct hush_map_llb* hashmap, const char* key);
void hush_set_capacity(struct hush_map_llb* hashmap, size_t capacity);
void hush_show(struct hush_map_llb hashmap);

void hush_dsa_append(hush_dynamic_string_array* dsa, const char* string);
void hush_dsa_show(hush_dynamic_string_array dsa);



//#define HUSH_IMPLEMENTATION
#ifdef HUSH_IMPLEMENTATION



long long int hush_fnv1a(const char* data) {
    long long int hash = 0xcbf29ce484222325;

    size_t count = strlen(data);
    for (size_t i = 0; i < count; i++) {
        hash = hash ^ data[i];
        hash = hash * 0x100000001b3; 
    }

    return hash;
}

size_t hush_get_index(struct hush_map_llb hashmap, const char* key) {
    return (size_t)(hush_fnv1a(key)) % hashmap->capacity;
}

// struct hush_map_llb hush_set_capacity(struct hush_map_llb hashmap, size_t capacity) {
//     return hashmap;
// }

void hush_set_value(struct hush_map_llb* hashmap, const char* key, int value) {
    size_t index = hush_get_index(*hashmap, key);
    struct hush_item_llb new_item = {0};
    new_item.key = key;
    new_item.value = value;
    new_item.next = NULL;
    if (hashmap->items[index] == NULL) {
        hashmap->items[index] = malloc(sizeof(struct hush_item_llb));
        *hashmap->items[index] = new_item;
    } else {
        struct hush_item_llb* collision_item = hashmap->items[index];
        while (collision_item != NULL) {
            if (collision_item->key == key) {
                collision_item->value = value;
                return;
            }
            collision_item = collision_item->next;
        }
        collision_item = hashmap->items[index];
        while (collision_item->next != NULL) {
            collision_item = collision_item->next;
        }
        collision_item->next = malloc(sizeof(struct hush_item_llb));
        *collision_item->next = new_item;
    }
}

bool hush_has_key(struct hush_map_llb hashmap, const char* key) {
    size_t index = hush_get_index(hashmap, key);
    struct hush_item_llb* item = hashmap.items[index];
    while (item != NULL) {
        if (item->key == key)
            return true;
        item = item->next;
    }
    return false;
}

int hush_get_value(struct hush_map_llb hashmap, const char* key) {
    size_t index = hush_get_index(hashmap, key);
    struct hush_item_llb* item = hashmap.items[index];
    while (item != NULL) {
        if (item->key == key) {
            return item->value;
        }
        item = item ->next;
    }
    fprintf(stderr, "[ERROR] Out of bounds, key does not exist in hashtable.\n");
    exit(1);
}

const char* hush_get_key(struct hush_map_llb hashmap, int value) {
    struct hush_item_llb* item = NULL;
    for (size_t i = 0; i < hashmap.capacity; i++) {
        if (hashmap.items[i] == NULL)
            continue;
        item = hashmap.items[i];
        while (item != NULL) {
            if (item->value == value)
                return item->key;
            item = item->next;
        }
    }
    fprintf(stderr, "[ERROR] Value doesn't exist in hashtable.");
    exit(1);
}

hush_dynamic_string_array hush_get_keys(struct hush_map_llb hashmap) {
    hush_dynamic_string_array dsa = {0};
    for (size_t i = 0; i < hashmap.capacity; i++) {
        struct hush_item_llb* item = hashmap.items[i];
        while (item != NULL) {
            hush_dsa_append(&dsa, item->key);
            item = item->next;
        }
    }
    return dsa;
}

struct hush_item_llb hush_pop_item(struct hush_map_llb* hashmap, const char* key) {
    size_t index = hush_get_index(*hashmap, key);
    struct hush_item_llb* item = hashmap->items[index];
    if (item->key == key) {
        hashmap->items[index] = item->next;
        item->next = NULL;
    } else {
        struct hush_item_llb* prev_item = item;
        while (item != NULL) {
            item = prev_item->next;
            if (item->key == key) {
                break;
            }
            prev_item = item;
        }
        if (item == NULL) {
            fprintf(stderr, "[ERROR] Out of bounds, key does not exist in hashtable.\n");
            exit(1);
        }
        prev_item->next = item->next;
        item->next = NULL;
    }
    struct hush_item_llb item_return = *item;
    free(item);
    return item_return;
}

void hush_set_capacity(struct hush_map_llb* hashmap, size_t capacity) {
    struct hush_item_llb** items = hashmap->items;
    size_t item_count = hashmap->capacity;
    hashmap->capacity = capacity;
    hashmap->items = malloc(hashmap->capacity * sizeof(struct hush_item_llb*));
    for (size_t i = 0; i < hashmap->capacity; i++) {
        hashmap->items[i] = NULL;
    }
    for (size_t i = 0; i < item_count; i++) {
        if (items[i] == NULL) {
            continue;
        }
        while (items[i] != NULL) {
            struct hush_item_llb* item = items[i];
            hush_set_value(hashmap, item->key, item->value);
            items[i] = item->next;
            item->next = NULL;
            free(item);
        }
    }

    for (size_t i = 0; i < item_count; i++) {
        free(items[i]);
    }
    free(items);
}

void hush_show(struct hush_map_llb hashmap) { // maybe change this later so that it doesn't add a tab at the end... Would need to make a list of keys
    hush_dynamic_string_array keys = hush_get_keys(hashmap);

    printf("{\n\t");
    for (size_t i = 0; i < keys.count; i++) {
        const char* key = keys.items[i];
        int value = hush_get_value(hashmap, keys.items[i]);
        printf("\"%s\": %d", key, value);
        if (i == keys.count  - 1)
            printf("\n");
        else
            printf(",\n\t");
    }
    printf("}\n");
}

void hush_dsa_append(hush_dynamic_string_array* dsa, const char* string) {
    if (dsa->count >= dsa->capacity) {
        dsa->capacity = dsa->capacity == 0 ? DSA_INIT_CAP : dsa->capacity * 2;
        dsa->items = realloc(dsa->items, dsa->capacity * sizeof(string));
        assert(dsa->items != NULL && "RAM is full :(\n");
    }
    dsa->items[dsa->count] = string;
    dsa->count += 1;
}

void hush_dsa_show(hush_dynamic_string_array dsa) {
    printf("[ ");
    for (size_t i = 0; i < dsa.count; i++) {
        printf("\"%s\"", dsa.items[i]);
        if (i != dsa.count - 1) 
            printf(", ");
        else
            printf(" ");
    }
    printf("]\n");
}


#endif
#endif
