#define HUSH_IMPLEMENTATION
#include"hush.h"
#define NOB_IMPLEMENTATION
#include"nob.h"

bool is_alphabetic(char c) {
    if ((65 <= c && c <= 90) || (97 <= c && c <= 122))
        return true;
    return false;
}

int main(void) {
    struct hush_map_llb hashmap = {0};
    hush_set_capacity(&hashmap, 10000);

    nob_String_Builder sb = {0};
    if (!nob_read_entire_file("Shakespeare.txt", &sb))
        return 1;
    
    char* offset = sb.items;
    size_t cursor = 0;
    //int loop_count_debug_thing = 0;
    while(offset + cursor < sb.items + sb.count) {
        if (is_alphabetic(*offset)) {
            char* current_char = offset + cursor;
            bool cc_is_alpha = is_alphabetic(*current_char);
            bool cc_is_apostrophe = is_alphabetic(*(current_char - 1)) && *current_char == '\'';
            bool cc_is_hyphen = is_alphabetic(*(current_char - 1)) && *current_char == '-' && is_alphabetic(*(current_char + 1));
            if (cc_is_alpha || cc_is_hyphen || cc_is_apostrophe) {
                cursor += 1;
                continue;
            } else {
                char* word = malloc(cursor + 1);
                for (size_t i = 0; i < cursor; i++) {
                    if (65 <= offset[i] && offset[i] <= 90) 
                        word[i] = offset[i] + 32;
                    else
                        word[i] = offset[i];
                }
                word[cursor] = '\0';                                            // this may or may not cause a segfault - will have to check
                const char* string = (const char*) word;
                
                //Some debug stuff
                //size_t index = hush_get_index(hashmap, string);
                //struct hush_item_llb* item = hashmap.items[index];
                //while (item != NULL) {
                //    fprintf(outf, "Comparing \"%s\", \"%s\": ", item->key, string);
                //    if (nob_is_equal_Cstr(item->key, string)) {
                //        fprintf(outf, "equal\n");
                //        break;
                //    } else {
                //        fprintf(outf, "not equal\n");
                //    }
                //    item = item->next;
                //}
                
                if (!hush_has_key(hashmap, string)) {
                    //fprintf(outf, "Adding new word: %s\n\n", string);
                    //loop_count_debug_thing += 1;
                    //if (loop_count_debug_thing >= 100) {
                    //    exit(-30);
                    //}
                    hush_set_value(&hashmap, string, 1);
                } else {
                    int wordcount = hush_get_value(hashmap, string);
                    hush_set_value(&hashmap, string, wordcount + 1);
                }
                
                offset += cursor;
                cursor = 0;
                free(word);
            }
        } else {
            offset += 1;
        }
    }

    if (cursor > 0) {
        offset[cursor] = '\0';
        const char* string = (const char*) offset;
        if (!hush_has_key(hashmap, string)) {
            hush_set_value(&hashmap, string, 1);
        } else {
            int wordcount = hush_get_value(hashmap, string);
            hush_set_value(&hashmap, string, wordcount + 1);
        }
        offset += cursor;
        cursor = 0;
    }

    FILE* outf = fopen("out.txt", "wb");

    hush_dynamic_string_array key_list = hush_get_keys(hashmap);
    int sum = 0;
    for (size_t i = 0; i < key_list.count; i++) {
        const char* key = key_list.items[i];
        int wordcount = hush_get_value(hashmap, key);
        fprintf(outf, "\"%s\": %d\n", key, wordcount);
        sum += wordcount;
    }
    printf("Number of valid words found is: %d\n", sum);

    fclose(outf);


    return 0;
}
