/**
 * @file occurrence.h
 * @brief Occurrence structure for inverted index implementation
 */

 #ifndef OCCURRENCE_H
 #define OCCURRENCE_H
 
 #include <stdlib.h>
 #include "ArrayList/arraylist.h"
 
 /**
  * Represents a single occurrence of a word in a document
  */
 typedef struct _Occurrence {
     int doc_id;
     ArrayList* positions_list;  // List of positions in the document
     struct _Occurrence* next;
 } Occurrence;
 
 /**
  * List of occurrences for a single word
  */
 typedef struct _OccurrenceList {
     Occurrence* first;
     Occurrence* last;  // For fast append
     int count;
 } OccurrenceList;
 
 /**
  * Creates a new occurrence
  * 
  * @param doc_id Document identifier
  * @param position Initial position of the word in the document
  * @return A pointer to the new occurrence or NULL if memory allocation fails
  */
 Occurrence* CreateOccurrence(int doc_id, int position);
 
 /**
  * Creates a new occurrence with an existing list of positions
  * 
  * @param doc_id Document identifier
  * @param positions_list Existing ArrayList of positions
  * @return A pointer to the new occurrence or NULL if memory allocation fails
  */
 Occurrence* CreateOccurrenceWithPositions(int doc_id, ArrayList* positions_list);
 
 /**
  * Adds a new position to an occurrence
  * 
  * @param occurrence The occurrence to update
  * @param position The position to add
  * @return 1 if successful, 0 if failed
  */
 int AddPositionToOccurrence(Occurrence* occurrence, int position);
 
 /**
  * Creates a new occurrence list with a single occurrence
  * 
  * @param occurrence The first occurrence to add to the list
  * @return A pointer to the new list or NULL if memory allocation fails
  */
 OccurrenceList* CreateOccurrenceList(Occurrence* occurrence);
 
 /**
  * Creates a new empty occurrence list
  * 
  * @return A pointer to the new empty list or NULL if memory allocation fails
  */
 OccurrenceList* CreateEmptyOccurrenceList(void);
 
 /**
  * Adds an occurrence to an existing list
  * 
  * @param list The list to append to
  * @param occurrence The occurrence to add
  * @return 1 if successful, 0 if parameters are NULL
  */
 int AddOccurrence(OccurrenceList* list, Occurrence* occurrence);
 
 /**
  * Finds an occurrence for a specific document in the list
  * 
  * @param list The list to search in
  * @param doc_id The document ID to find
  * @return The occurrence if found, NULL otherwise
  */
 Occurrence* FindOccurrenceByDocId(const OccurrenceList* list, int doc_id);
 
 /**
  * Adds a position to an occurrence for a specific document
  * If the document doesn't exist in the list yet, creates a new occurrence
  * 
  * @param list The list to update
  * @param doc_id The document ID
  * @param position The position to add
  * @return 1 if successful, 0 if failed
  */
 int AddPositionToDocument(OccurrenceList* list, int doc_id, int position);
 
 /**
  * Gets the count of documents in the occurrence list
  * 
  * @param list The list to count
  * @return The number of documents (occurrences)
  */
 int GetDocumentCount(const OccurrenceList* list);
 
 /**
  * Gets the count of positions for a specific document
  * 
  * @param list The list to search in
  * @param doc_id The document ID to find
  * @return The number of positions in the document, 0 if not found
  */
 int GetPositionCount(const OccurrenceList* list, int doc_id);
 
 /**
  * Merges two occurrence lists into one
  * The second list is appended to the first list
  * 
  * @param dest The destination list
  * @param src The source list (will be empty after the operation)
  * @return 1 if successful, 0 if parameters are NULL
  */
 int MergeOccurrenceLists(OccurrenceList* dest, OccurrenceList* src);
 
 /**
  * Frees all memory associated with an occurrence
  * 
  * @param occurrence The occurrence to free
  */
 void FreeOccurrence(Occurrence* occurrence);
 
 /**
  * Frees all memory associated with an occurrence list
  * 
  * @param list The list to free
  */
 void FreeOccurrenceList(OccurrenceList* list);
 
 #endif // OCCURRENCE_H