/**
 * @file arraylist.h
 * @brief Header file for a dynamic array (ArrayList) implementation in C90
 */

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdlib.h>

/**
 * @struct ArrayList
 * @brief A dynamic array implementation that resizes automatically
 *
 * @param data Pointer to the array of elements
 * @param size Current number of elements in the array
 * @param capacity Maximum capacity of the array before resizing
 * @param element_size Size of each element in bytes
 */
typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    size_t element_size;
} ArrayList;

/**
 * @brief Initialize a new ArrayList
 *
 * @param initial_capacity The initial capacity of the array
 * @param element_size Size of each element in bytes
 * @return A pointer to the newly created ArrayList, or NULL if allocation failed
 */
ArrayList* arraylist_create(size_t initial_capacity, size_t element_size);

/**
 * @brief Free the memory used by the ArrayList
 *
 * @param list The ArrayList to free
 */
void arraylist_destroy(ArrayList* list);

/**
 * @brief Add an element to the end of the ArrayList
 *
 * @param list The ArrayList to add to
 * @param element Pointer to the element to add
 * @return 1 if successful, 0 if failed
 */
int arraylist_add(ArrayList* list, const void* element);

/**
 * @brief Get the element at the specified index
 *
 * @param list The ArrayList to get from
 * @param index The index of the element to get
 * @return Pointer to the element, or NULL if index is out of bounds
 */
void* arraylist_get(const ArrayList* list, size_t index);

/**
 * @brief Set the element at the specified index
 *
 * @param list The ArrayList to set in
 * @param index The index of the element to set
 * @param element Pointer to the element to set
 * @return 1 if successful, 0 if index is out of bounds
 */
int arraylist_set(ArrayList* list, size_t index, const void* element);

/**
 * @brief Insert an element at the specified index
 *
 * @param list The ArrayList to insert into
 * @param index The index to insert at
 * @param element Pointer to the element to insert
 * @return 1 if successful, 0 if failed or index is out of bounds
 */
int arraylist_insert(ArrayList* list, size_t index, const void* element);

/**
 * @brief Remove the element at the specified index
 *
 * @param list The ArrayList to remove from
 * @param index The index of the element to remove
 * @return 1 if successful, 0 if index is out of bounds
 */
int arraylist_remove(ArrayList* list, size_t index);

/**
 * @brief Get the current size of the ArrayList
 *
 * @param list The ArrayList to get the size of
 * @return The number of elements in the ArrayList
 */
size_t arraylist_size(const ArrayList* list);

/**
 * @brief Check if the ArrayList is empty
 *
 * @param list The ArrayList to check
 * @return 1 if empty, 0 if not empty
 */
int arraylist_is_empty(const ArrayList* list);

/**
 * @brief Clear all elements from the ArrayList
 *
 * @param list The ArrayList to clear
 */
void arraylist_clear(ArrayList* list);

/**
 * @brief Ensure the ArrayList has enough capacity
 *
 * @param list The ArrayList to resize
 * @param min_capacity The minimum capacity required
 * @return 1 if successful, 0 if allocation failed
 */
int arraylist_ensure_capacity(ArrayList* list, size_t min_capacity);

/**
 * @brief Trim the capacity of the ArrayList to its current size
 *
 * @param list The ArrayList to trim
 * @return 1 if successful, 0 if allocation failed
 */
int arraylist_trim_to_size(ArrayList* list);

typedef int (*Comparator)(const void* a, const void* b);
void arraylist_sort(ArrayList* list, Comparator cmp);

#endif /* ARRAYLIST_H */