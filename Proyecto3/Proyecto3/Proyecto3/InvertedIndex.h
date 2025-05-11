#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include "HashTable.h"
#include "Occurrence/occurrence.h"
#include <stdio.h>

#define MAX_OPEN_FILES 5
#define WORD_MIN_LENGTH 4
#define CONTEXT_WINDOW 100  // max chars between words

// For search results: document id and line range
typedef struct _printData {
    int doc_id;
    int first_occurrence_line;
    int last_occurrence_line;
} printData;

// Main index structure
typedef struct _InvertedIndex {
    HashTable table;                    // maps word -> OccurrenceList*
    FILE* opened_files[MAX_OPEN_FILES];  // raw FILE* handles
    int last_file_index;                 // index of most recently added file
} InvertedIndex;

// Initialize a new inverted index
InvertedIndex* II_Create();
// Clean up and free all memory
void II_Destroy(InvertedIndex* idx);

// Load a file into the index; returns file ID or -1 on error
int II_LoadFile(InvertedIndex* idx, const char* fileName);

// Search for an array of words; returns array of printData and sets out_count
printData* II_Search(InvertedIndex* idx, char* words[], int word_count, int* out_count);

#endif