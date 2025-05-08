/**
 * @file example.c
 * @brief Example usage of the ArrayList implementation
 */

#include <stdio.h>
#include "arraylist.h"

int main() {
    ArrayList* list;
    int i, value, *element;
    
    /* Create a new ArrayList for integers */
    list = arraylist_create(5, sizeof(int));
    if (list == NULL) {
        printf("Failed to create ArrayList\n");
        return 1;
    }
    
    /* Add elements to the ArrayList */
    printf("Adding elements to the ArrayList:\n");
    for (i = 0; i < 10; i++) {
        value = i * 10;
        if (!arraylist_add(list, &value)) {
            printf("Failed to add element %d\n", i);
        } else {
            printf("Added: %d\n", value);
        }
    }
    
    /* Get and print all elements */
    printf("\nElements in the ArrayList:\n");
    for (i = 0; i < arraylist_size(list); i++) {
        element = (int*)arraylist_get(list, i);
        if (element != NULL) {
            printf("Element at index %d: %d\n", i, *element);
        }
    }
    
    /* Insert an element at index 2 */
    value = 999;
    if (arraylist_insert(list, 2, &value)) {
        printf("\nInserted %d at index 2\n", value);
    }
    
    /* Remove an element at index 5 */
    if (arraylist_remove(list, 5)) {
        printf("Removed element at index 5\n");
    }
    
    /* Print the updated ArrayList */
    printf("\nUpdated ArrayList:\n");
    for (i = 0; i < arraylist_size(list); i++) {
        element = (int*)arraylist_get(list, i);
        if (element != NULL) {
            printf("Element at index %d: %d\n", i, *element);
        }
    }
    
    /* Clear the ArrayList */
    arraylist_clear(list);
    printf("\nArrayList cleared, size: %zu\n", arraylist_size(list));
    
    /* Free the ArrayList */
    arraylist_destroy(list);
    printf("ArrayList destroyed\n");
    
    return 0;
}