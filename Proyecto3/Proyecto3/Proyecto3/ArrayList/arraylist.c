/**
 * @file arraylist.c
 * @brief Implementation of the ArrayList functions
 */

#include <string.h>
#include "arraylist.h"

/* Default initial capacity if not specified */
#define DEFAULT_CAPACITY 10

/* Growth factor when resizing */
#define GROWTH_FACTOR 2

/**
 * @brief Create a new ArrayList with the specified capacity
 */
ArrayList* arraylist_create(size_t initial_capacity, size_t element_size) {
    ArrayList* list;
    
    /* Validate parameters */
    if (element_size == 0) {
        return NULL;
    }
    
    /* Use default capacity if 0 was provided */
    if (initial_capacity == 0) {
        initial_capacity = DEFAULT_CAPACITY;
    }
    
    /* Allocate memory for the ArrayList structure */
    list = (ArrayList*)malloc(sizeof(ArrayList));
    if (list == NULL) {
        return NULL;
    }
    
    /* Allocate memory for the data array */
    list->data = malloc(initial_capacity * element_size);
    if (list->data == NULL) {
        free(list);
        return NULL;
    }
    
    /* Initialize the structure */
    list->size = 0;
    list->capacity = initial_capacity;
    list->element_size = element_size;
    
    return list;
}

/**
 * @brief Free the memory used by the ArrayList
 */
void arraylist_destroy(ArrayList* list) {
    if (list == NULL) {
        return;
    }
    
    /* Free the data array if it exists */
    if (list->data != NULL) {
        free(list->data);
    }
    
    /* Free the ArrayList structure itself */
    free(list);
}

/**
 * @brief Add an element to the end of the ArrayList
 */
int arraylist_add(ArrayList* list, const void* element) {
    /* Validate parameters */
    if (list == NULL || element == NULL) {
        return 0;
    }
    
    /* Check if we need to resize the array */
    if (list->size >= list->capacity) {
        if (!arraylist_ensure_capacity(list, list->capacity * GROWTH_FACTOR)) {
            return 0;
        }
    }
    
    /* Copy the element to the end of the array */
    memcpy(
        (char*)list->data + (list->size * list->element_size),
        element,
        list->element_size
    );
    
    /* Increment the size */
    list->size++;
    
    return 1;
}

/**
 * @brief Get the element at the specified index
 */
void* arraylist_get(const ArrayList* list, size_t index) {
    /* Validate parameters */
    if (list == NULL || index >= list->size) {
        return NULL;
    }
    
    /* Calculate the address of the element */
    return (char*)list->data + (index * list->element_size);
}

/**
 * @brief Set the element at the specified index
 */
int arraylist_set(ArrayList* list, size_t index, const void* element) {
    /* Validate parameters */
    if (list == NULL || element == NULL || index >= list->size) {
        return 0;
    }
    
    /* Copy the element to the specified position */
    memcpy(
        (char*)list->data + (index * list->element_size),
        element,
        list->element_size
    );
    
    return 1;
}

/**
 * @brief Insert an element at the specified index
 */
int arraylist_insert(ArrayList* list, size_t index, const void* element) {
    size_t move_size;
    
    /* Validate parameters */
    if (list == NULL || element == NULL || index > list->size) {
        return 0;
    }
    
    /* If inserting at the end, use add function */
    if (index == list->size) {
        return arraylist_add(list, element);
    }
    
    /* Check if we need to resize the array */
    if (list->size >= list->capacity) {
        if (!arraylist_ensure_capacity(list, list->capacity * GROWTH_FACTOR)) {
            return 0;
        }
    }
    
    /* Calculate the number of elements to move */
    move_size = (list->size - index) * list->element_size;
    
    /* Shift elements to make space for the new element */
    memmove(
        (char*)list->data + ((index + 1) * list->element_size),
        (char*)list->data + (index * list->element_size),
        move_size
    );
    
    /* Copy the new element to the specified position */
    memcpy(
        (char*)list->data + (index * list->element_size),
        element,
        list->element_size
    );
    
    /* Increment the size */
    list->size++;
    
    return 1;
}

/**
 * @brief Remove the element at the specified index
 */
int arraylist_remove(ArrayList* list, size_t index) {
    size_t move_size;
    
    /* Validate parameters */
    if (list == NULL || index >= list->size) {
        return 0;
    }
    
    /* Calculate the number of elements to move */
    move_size = (list->size - index - 1) * list->element_size;
    
    /* Shift elements to fill the gap */
    if (move_size > 0) {
        memmove(
            (char*)list->data + (index * list->element_size),
            (char*)list->data + ((index + 1) * list->element_size),
            move_size
        );
    }
    
    /* Decrement the size */
    list->size--;
    
    return 1;
}

/**
 * @brief Get the current size of the ArrayList
 */
size_t arraylist_size(const ArrayList* list) {
    if (list == NULL) {
        return 0;
    }
    
    return list->size;
}

/**
 * @brief Check if the ArrayList is empty
 */
int arraylist_is_empty(const ArrayList* list) {
    if (list == NULL) {
        return 1;
    }
    
    return list->size == 0;
}

/**
 * @brief Clear all elements from the ArrayList
 */
void arraylist_clear(ArrayList* list) {
    if (list == NULL) {
        return;
    }
    
    /* Reset the size to 0 */
    list->size = 0;
}

/**
 * @brief Ensure the ArrayList has enough capacity
 */
int arraylist_ensure_capacity(ArrayList* list, size_t min_capacity) {
    void* new_data;
    
    /* Validate parameters */
    if (list == NULL || min_capacity <= list->capacity) {
        return 1; /* Already has enough capacity */
    }
    
    /* Allocate new memory */
    new_data = realloc(list->data, min_capacity * list->element_size);
    if (new_data == NULL) {
        return 0;
    }
    
    /* Update the list with the new data and capacity */
    list->data = new_data;
    list->capacity = min_capacity;
    
    return 1;
}

/**
 * @brief Trim the capacity of the ArrayList to its current size
 */
int arraylist_trim_to_size(ArrayList* list) {
    void* new_data;
    size_t new_capacity;
    
    /* Validate parameters */
    if (list == NULL || list->size == list->capacity) {
        return 1; /* Already trimmed */
    }
    
    /* Calculate the new capacity */
    new_capacity = list->size > 0 ? list->size : DEFAULT_CAPACITY;
    
    /* Allocate new memory */
    new_data = realloc(list->data, new_capacity * list->element_size);
    if (new_data == NULL) {
        return 0;
    }
    
    /* Update the list with the new data and capacity */
    list->data = new_data;
    list->capacity = new_capacity;
    
    return 1;
}