/**
 * @file occurrence.c
 * @brief Implementation of the Occurrence functions for inverted index
 */

 #include <stdlib.h>
 #include "occurrence.h"
 
 /**
  * Creates a new occurrence with initial position
  */
 Occurrence* CreateOccurrence(int doc_id, int position) {
     Occurrence* occurrence;
     ArrayList* positions;
     
     if (position < 0) {
         return NULL;
     }
     
     // Create positions list
     positions = arraylist_create(5, sizeof(int));
     if (positions == NULL) {
         return NULL;
     }
     
     // Add initial position
     if (!arraylist_add(positions, &position)) {
         arraylist_destroy(positions);
         return NULL;
     }
     
     // Create occurrence
     occurrence = (Occurrence*)malloc(sizeof(Occurrence));
     if (occurrence == NULL) {
         arraylist_destroy(positions);
         return NULL;
     }
     
     // Initialize occurrence
     occurrence->doc_id = doc_id;
     occurrence->positions_list = positions;
     occurrence->next = NULL;
     
     return occurrence;
 }
 
 /**
  * Creates a new occurrence with existing positions list
  */
 Occurrence* CreateOccurrenceWithPositions(int doc_id, ArrayList* positions_list) {
     Occurrence* occurrence;
     
     if (positions_list == NULL) {
         return NULL;
     }
     
     // Create occurrence
     occurrence = (Occurrence*)malloc(sizeof(Occurrence));
     if (occurrence == NULL) {
         return NULL;
     }
     
     // Initialize occurrence
     occurrence->doc_id = doc_id;
     occurrence->positions_list = positions_list;
     occurrence->next = NULL;
     
     return occurrence;
 }
 
 /**
  * Adds a new position to an occurrence
  */
 int AddPositionToOccurrence(Occurrence* occurrence, int position) {
     if (occurrence == NULL || position < 0) {
         return 0;
     }
     
     // Check if positions list exists
     if (occurrence->positions_list == NULL) {
         occurrence->positions_list = arraylist_create(5, sizeof(int));
         if (occurrence->positions_list == NULL) {
             return 0;
         }
     }
     
     return arraylist_add(occurrence->positions_list, &position);
 }
 
 /**
  * Creates a new occurrence list with a single occurrence
  */
 OccurrenceList* CreateOccurrenceList(Occurrence* occurrence) {
     OccurrenceList* list;

     if (occurrence == NULL) {
         return NULL;
     }
     
     // Allocate memory for the list
     list = (OccurrenceList*)malloc(sizeof(OccurrenceList));
     if (list == NULL) {
         return NULL;
     }
     
     // Initialize list with the occurrence
     list->first = occurrence;
     list->last = occurrence;
     list->count = 1;
     
     return list;
 }
 
 /**
  * Creates a new empty occurrence list
  */
 OccurrenceList* CreateEmptyOccurrenceList(void) {
     OccurrenceList* list;
     
     list = (OccurrenceList*)malloc(sizeof(OccurrenceList));
     if (list == NULL) {
         return NULL;
     }
     
     // Initialize as empty list
     list->first = NULL;
     list->last = NULL;
     list->count = 0;
     
     return list;
 }
 
 /**
  * Adds an occurrence to an existing list
  */
 int AddOccurrence(OccurrenceList* list, Occurrence* occurrence) {
     if (list == NULL || occurrence == NULL) {
         return 0;
     }
     
     // If the list is empty, set as first occurrence
     if (list->first == NULL) {
         list->first = occurrence;
         list->last = occurrence;
         list->count = 1;
         return 1;
     }
     
     // Add to the end of the list
     list->last->next = occurrence;
     list->last = occurrence;
     list->count++;
     
     return 1;
 }
 
 /**
  * Finds an occurrence for a specific document in the list
  */
 Occurrence* FindOccurrenceByDocId(const OccurrenceList* list, int doc_id) {
     Occurrence* current;
     
     if (list == NULL) {
         return NULL;
     }
     
     // Traverse the list looking for doc_id
     current = list->first;
     while (current != NULL) {
         if (current->doc_id == doc_id) {
             return current;
         }
         current = current->next;
     }
     
     // Not found
     return NULL;
 }
 
 /**
  * Adds a position to an occurrence for a specific document
  */
 int AddPositionToDocument(OccurrenceList* list, int doc_id, int position) {
     Occurrence* occurrence;
     
     if (list == NULL || position < 0) {
         return 0;
     }
     
     // Find document in the list
     occurrence = FindOccurrenceByDocId(list, doc_id);
     
     // If document exists, add position
     if (occurrence != NULL) {
         return AddPositionToOccurrence(occurrence, position);
     }
     
     // Document doesn't exist, create new occurrence
     occurrence = CreateOccurrence(doc_id, position);
     if (occurrence == NULL) {
         return 0;
     }
     
     // Add new occurrence to the list
     return AddOccurrence(list, occurrence);
 }
 
 /**
  * Gets the count of documents in the occurrence list
  */
 int GetDocumentCount(const OccurrenceList* list) {
     if (list == NULL) {
         return 0;
     }
     
     return list->count;
 }
 
 /**
  * Gets the count of positions for a specific document
  */
 int GetPositionCount(const OccurrenceList* list, int doc_id) {
     Occurrence* occurrence;
     
     if (list == NULL) {
         return 0;
     }
     
     // Find document in the list
     occurrence = FindOccurrenceByDocId(list, doc_id);
     
     // If found, return position count
     if (occurrence != NULL && occurrence->positions_list != NULL) {
         return (int)arraylist_size(occurrence->positions_list);
     }
     
     return 0;
 }
 
 /**
  * Merges two occurrence lists into one
  */
 int MergeOccurrenceLists(OccurrenceList* dest, OccurrenceList* src) {
     if (dest == NULL || src == NULL) {
         return 0;
     }
     
     // If source is empty, nothing to do here, just return
     if (src->first == NULL) {
         return 1;
     }
     
     // If destination is empty, just transfer
     if (dest->first == NULL) {
         dest->first = src->first;
         dest->last = src->last;
         dest->count = src->count;
     } else {
         // Append source to destination
         dest->last->next = src->first;
         dest->last = src->last;
         dest->count += src->count;
     }
     
     // Clear source list without freeing occurrences
     src->first = NULL;
     src->last = NULL;
     src->count = 0;
     
     return 1;
 }
 
 /**
  * Frees all memory associated with an occurrence
  */
 void FreeOccurrence(Occurrence* occurrence) {
     if (occurrence == NULL) {
         return;
     }
     
     // Free the positions list if it exists
     if (occurrence->positions_list != NULL) {
         arraylist_destroy(occurrence->positions_list);
     }
     
     // Free the occurrence itself
     free(occurrence);
 }
 
 /**
  * Frees all memory associated with an occurrence list
  */
 void FreeOccurrenceList(OccurrenceList* list) {
     Occurrence* current;
     Occurrence* next;
     
     if (list == NULL) {
         return;
     }
     
     // Free all occurrences in the list
     current = list->first;
     while (current != NULL) {
         next = current->next;
         FreeOccurrence(current);
         current = next;
     }
     
     // Free the list itself
     free(list);
 }