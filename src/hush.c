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

    nob_String_Builder file_contents = {0};
    const char* file_path = "Shakespeare.txt";
    if (!nob_read_entire_file(file_path, &file_contents)) {
        nob_log(NOB_ERROR, "Could not read file: %s\n", file_path);
        return 1;
    }

    nob_log(NOB_INFO, "The file %s is %lli bytes long.", file_path, file_contents.count);
    
    char* offset = file_contents.items;
    size_t cursor = 0;
    while(offset + cursor < file_contents.items + file_contents.count) {
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
                word[cursor] = '\0';
                const char* string = (const char*) word;
                
                if (!hush_has_key(hashmap, string)) {
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
    nob_log(NOB_INFO, "Number of valid words found is: %d\n", sum);

    fclose(outf);


    return 0;
}
