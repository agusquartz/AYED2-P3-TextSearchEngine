/**
 * @file occurrence_example.c
 * @brief Example usage of the Occurrence structure for inverted index
 */

#include <stdio.h>
#include "occurrence.h"

/**
 * Helper function to print all positions for a document
 */
void PrintPositions(const ArrayList* positions_list) {
    size_t i;
    int* position;
    
    if (positions_list == NULL) {
        printf("No positions available\n");
        return;
    }
    
    printf("Positions: ");
    for (i = 0; i < arraylist_size(positions_list); i++) {
        position = (int*)arraylist_get(positions_list, i);
        if (position != NULL) {
            printf("%d ", *position);
        }
    }
    printf("\n");
}

/**
 * Helper function to print all occurrences in a list
 */
void PrintOccurrenceList(const OccurrenceList* list) {
    Occurrence* current;
    
    if (list == NULL) {
        printf("List is NULL\n");
        return;
    }
    
    if (list->count == 0) {
        printf("List is empty\n");
        return;
    }
    
    printf("Occurrence List (Total Documents: %d):\n", list->count);
    current = list->first;
    while (current != NULL) {
        printf("Document ID: %d - ", current->doc_id);
        PrintPositions(current->positions_list);
        current = current->next;
    }
    printf("\n");
}

int main() {
    OccurrenceList* list;
    Occurrence* occurrence;
    
    printf("Creating occurrence list for word 'example'...\n");
    
    /* Create first occurrence with one position */
    occurrence = CreateOccurrence(1, 5);
    if (occurrence == NULL) {
        printf("Failed to create occurrence\n");
        return 1;
    }
    
    /* Create list with first occurrence */
    list = CreateOccurrenceList(occurrence);
    if (list == NULL) {
        printf("Failed to create occurrence list\n");
        FreeOccurrence(occurrence);
        return 1;
    }
    
    /* Add more positions to document 1 */
    AddPositionToDocument(list, 1, 10);
    AddPositionToDocument(list, 1, 15);
    
    /* Add positions for document 2 */
    AddPositionToDocument(list, 2, 3);
    AddPositionToDocument(list, 2, 7);
    AddPositionToDocument(list, 2, 12);
    
    /* Add positions for document 3 */
    AddPositionToDocument(list, 3, 1);
    
    /* Print the occurrence list */
    PrintOccurrenceList(list);
    
    /* Find a specific document and add more positions */
    occurrence = FindOccurrenceByDocId(list, 2);
    if (occurrence != NULL) {
        printf("Adding more positions to document 2...\n");
        AddPositionToOccurrence(occurrence, 20);
        AddPositionToOccurrence(occurrence, 25);
    }
    
    /* Print updated list */
    PrintOccurrenceList(list);
    
    /* Print statistics */
    printf("Statistics:\n");
    printf("Total documents: %d\n", GetDocumentCount(list));
    printf("Positions in document 1: %d\n", GetPositionCount(list, 1));
    printf("Positions in document 2: %d\n", GetPositionCount(list, 2));
    printf("Positions in document 3: %d\n", GetPositionCount(list, 3));
    printf("Positions in document 4: %d\n", GetPositionCount(list, 4));
    
    /* Create a second list for merging */
    OccurrenceList* list2 = CreateEmptyOccurrenceList();
    if (list2 != NULL) {
        printf("\nCreating second list for word 'test'...\n");
        AddPositionToDocument(list2, 4, 2);
        AddPositionToDocument(list2, 4, 8);
        AddPositionToDocument(list2, 5, 5);
        
        PrintOccurrenceList(list2);
        
        /* Merge lists */
        printf("Merging lists...\n");
        MergeOccurrenceLists(list, list2);
        
        /* Print merged list */
        PrintOccurrenceList(list);
        
        /* Free second list (now empty) */
        FreeOccurrenceList(list2);
    }
    
    /* Free all memory */
    FreeOccurrenceList(list);
    printf("Memory freed\n");
    
    return 0;
}