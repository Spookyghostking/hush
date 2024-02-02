#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct hush_item_llb {
    const char* key;
    struct hush_item_llb* next;
    int value;
};

struct hush_map_llb {
    size_t capacity;
    struct hush_item_llb** items;
};


long long int hush_fnv1a(const char* data);
void hush_set_value(struct hush_map_llb* hashmap, const char* key, int value);
int hush_get_value(struct hush_map_llb hashmap, const char* key);



long long int hush_fnv1a(const char* data) {
    long long int hash = 0xcbf29ce484222325;

    size_t count = strlen(data);
    for (size_t i = 0; i < count; i++) {
        hash = hash ^ data[i];
        hash = hash * 0x100000001b3; 
    }

    return hash;
}

// struct hush_map_llb hush_set_capacity(struct hush_map_llb hashmap, size_t capacity) {
//     return hashmap;
// }

void hush_set_value(struct hush_map_llb* hashmap, const char* key, int value) {
    size_t index = (size_t)(hush_fnv1a(key) % hashmap->capacity);
    struct hush_item_llb new_item = {0};
    new_item.key = key;
    new_item.value = value;
    new_item.next = NULL;
    if (hashmap->items[index] == NULL) {
        hashmap->items[index] = malloc(sizeof(struct hush_item_llb));
        *hashmap->items[index] = new_item;
    } else {
        struct hush_item_llb* collision_item = hashmap->items[index];
        while (collision_item->next != NULL) {
            collision_item = collision_item->next;
        }
        collision_item->next = malloc(sizeof(struct hush_item_llb));
        *collision_item->next = new_item;
    }
}

int hush_get_value(struct hush_map_llb hashmap, const char* key) {
    size_t index = (size_t)(hush_fnv1a(key) % hashmap.capacity);
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

struct hush_item_llb hush_pop_item(struct hush_map_llb* hashmap, const char* key) {
    size_t index = (size_t)(hush_fnv1a(key) % hashmap->capacity);
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

void hush_show(struct hush_map_llb hashmap) {
    for (size_t i = 0; i < hashmap.capacity; i++) {
        struct hush_item_llb* item = hashmap.items[i];
        while (item != NULL) {
            printf("\"%s\": %d", item->key, item->value);
            if (item->next != NULL) 
                printf(", ");
            else
                printf(";\n");
            item = item->next;
        }
    }
}


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

    hush_show(hashmap);


    return 0;
}
