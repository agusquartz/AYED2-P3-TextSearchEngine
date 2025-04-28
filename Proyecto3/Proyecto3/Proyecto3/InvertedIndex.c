/**
 * Represents a single occurrence of a word in a document
 */
 typedef struct _Occurrence {
    int doc_id;
    int position;
    struct _Occurrence* next;
} Occurrence;

/**
 * List of occurrences for a single word
 */
typedef struct _OccurrenceList {
    Occurrence* first;
    Occurrence* last; // For fast append
    int count;
} OccurrenceList;

/**
 * Creates a new occurrence
 * 
 * @param id Document identifier
 * @param position The position of the word in the document
 * @return A pointer to the new occurrence or NULL if memory allocation fails
 */
 Occurrence* CreateOccurrence(int id, int position) {
    Occurrence* occurrence = (Occurrence*)malloc(sizeof(Occurrence));
    if (NULL == occurrence) return NULL;
    
    occurrence->doc_id = id;
    if (occurrence->document == NULL) {
        free(occurrence);
        return NULL;
    }
    
    occurrence->position = position;
    occurrence->next = NULL;
    
    return occurrence;
}

/**
 * Creates a new occurrence list with a single occurrence
 * 
 * @param occurrence The first occurrence to add to the list
 * @return A pointer to the new list or NULL if memory allocation fails
 */
OccurrenceList* CreateOccurrenceList(Occurrence* occurrence) {
    OccurrenceList* list = (OccurrenceList*)malloc(sizeof(OccurrenceList));
    if (NULL == list) return NULL;
    
    list->first = occurrence;
    list->last = occurrence;
    list->count = 1;
    
    return list;
}

/**
 * Adds an occurrence to an existing list
 * 
 * @param list The list to append to
 * @param occurrence The occurrence to add
 */
void AddOccurrence(OccurrenceList* list, Occurrence* occurrence) {
    if (NULL == list || NULL == occurrence) return;

    list->last->next = occurrence;
    list->last = occurrence;
    list->count++;
}

/**
 * Frees all memory associated with an occurrence list
 * 
 * @param list The list to "free"
 */
void FreeOccurrenceList(OccurrenceList* list) {
    if (NULL == list) return;
    
    Occurrence* current = list->first;
    while (current != NULL) {
        Occurrence* next = current->next;

        free(current);

        current = next;
    }
    
    free(list);
}