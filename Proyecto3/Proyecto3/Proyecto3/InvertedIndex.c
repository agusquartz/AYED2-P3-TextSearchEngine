#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "InvertedIndex.h"
#include "HashTable.h"
#include "ArrayList/arraylist.h"
#include "Occurrence/occurrence.h"
#include "FileManager.h"  // provides open_file, print_lines, find_line_by_position



int compare_positions(const void* a, const void* b) {
    long pa = (long)(uintptr_t)a;
    long pb = (long)(uintptr_t)b;
    return (pa < pb) ? -1 : (pa > pb);
}

static void add_word_occurrence(InvertedIndex* idx, const char* word, int file_id, long position) {
    void* val = NULL;
    if (HTContains(idx->table, (char*)word)) {
        HTGet(idx->table, (char*)word, &val);
        OccurrenceList* list = (OccurrenceList*)val;
        AddPositionToDocument(list, file_id, position);
    } else {
        // create new occurrence list
        OccurrenceList* list = malloc(sizeof(OccurrenceList));
        list->first = NULL;
        list->last = NULL;
        list->count = 0;
        // first occurrence
        Occurrence* occ = malloc(sizeof(Occurrence));
        occ->doc_id = file_id;
        occ->positions_list = arraylist_create(11, sizeof(int));
        arraylist_add(occ->positions_list, (void*)(uintptr_t)position);
        occ->next = NULL;
        list->first = occ;
        list->last = occ;
        list->count = 1;
        // insert into hashtable
        char* key = strdup(word);
        HTPut(idx->table, key, list);
    }
}

InvertedIndex* II_Create() {
    InvertedIndex* idx = malloc(sizeof(InvertedIndex));
    idx->table = HTCreate(1024);
    for (int i = 0; i < MAX_OPEN_FILES; ++i) idx->opened_files[i] = NULL;
    idx->last_file_index = -1;
    return idx;
}

void II_Destroy(InvertedIndex* idx) {
    if (!idx) return;

    for (int i = 0; i < idx->table->cap; ++i) {
        Celda* cell = idx->table->arr[i];
        if (cell != NULL && cell != TOMBSTONE) {
            FreeOccurrenceList((OccurrenceList*)cell->valor);
        }
    }

    HTDestroy(idx->table);

    for (int f = 0; f <= idx->last_file_index; ++f) {
        if (idx->opened_files[f]) {
            fclose(idx->opened_files[f]);
        }
    }

    free(idx);
}

int II_LoadFile(InvertedIndex* idx, const char* fileName) {
    if (idx->last_file_index + 1 >= MAX_OPEN_FILES) return -1;
    FILE* f = open_file(fileName);
    if (!f) return -1;
    int id = ++idx->last_file_index;
    idx->opened_files[id] = f;

    long pos = 0;
    long word_start = -1;
    int ch;
    while ((ch = fgetc(f)) != EOF) {
        if (isalpha(ch)) {
            if (word_start < 0) word_start = pos;
        } else {
            if (word_start >= 0 && pos - word_start >= WORD_MIN_LENGTH) {
                long len = pos - word_start;
                char* word = malloc(len + 1);
                fseek(f, word_start, SEEK_SET);
                fread(word, 1, len, f);
                word[len] = '\0';
                for (long i = 0; i < len; ++i) word[i] = tolower((unsigned char)word[i]);
                add_word_occurrence(idx, word, id, word_start);
                free(word);
                fseek(f, pos, SEEK_SET);
            }
            word_start = -1;
        }
        pos++;
    }
    return id;
}

printData* II_Search(InvertedIndex* idx, char* words[], int word_count, int* out_count) {
    // retrieve lists
    OccurrenceList** lists = malloc(sizeof(*lists) * word_count);
    for (int i = 0; i < word_count; ++i) {
        void* val = NULL;
        if (HTContains(idx->table, words[i])) HTGet(idx->table, words[i], &val);
        lists[i] = (OccurrenceList*)val;
    }
    // prepare results
    printData* results = malloc(sizeof(printData) * (idx->last_file_index + 1));
    int res_count = 0;

    for (int doc = 0; doc <= idx->last_file_index; ++doc) {
        ArrayList* all_pos = arraylist_create(11, sizeof(int));
        for (int w = 0; w < word_count; ++w) {
            Occurrence* cur = lists[w] ? lists[w]->first : NULL;
            while (cur) {
                if (cur->doc_id == doc) {
                    for (int j = 0; j < arraylist_size(cur->positions_list); ++j) {
                        long p = (long)(uintptr_t)arraylist_get(cur->positions_list, j);
                        arraylist_add(all_pos, (void*)(uintptr_t)p);
                    }
                }
                cur = cur->next;
            }
        }
        if (arraylist_size(all_pos) < word_count) { arraylist_destroy(all_pos); continue; }
        arraylist_sort(all_pos, compare_positions);
        int start = 0;
        long first_line = -1, last_line = -1;
        for (int end = 0; end < arraylist_size(all_pos); ++end) {
            while ((long)(uintptr_t)arraylist_get(all_pos, end) - (long)(uintptr_t)arraylist_get(all_pos, start) > CONTEXT_WINDOW) start++;
            if (end - start + 1 >= word_count) {
                long p0 = (long)(uintptr_t)arraylist_get(all_pos, start);
                long p1 = (long)(uintptr_t)arraylist_get(all_pos, end);
                first_line = find_line_by_position(idx->opened_files[doc], p0);
                last_line  = find_line_by_position(idx->opened_files[doc], p1);
                break;
            }
        }
        if (first_line > 0) {
            results[res_count++] = (printData){ doc, first_line, last_line };
        }
        arraylist_destroy(all_pos);
    }
    free(lists);
    *out_count = res_count;
    return results;
}
