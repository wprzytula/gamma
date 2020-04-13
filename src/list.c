/** @file
 * Implementacja listy przechowującej wskaźniki jako wartości.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 27.03.2020
 */

#include "list.h"
#include <string.h>


bool list_in(list_t *list, void *value) {
    list_elem_t *currentElem = list->left_guard->right;
    while (currentElem != list->right_guard) {
        if (currentElem->value == value)
            return true;
        else
            currentElem = currentElem->right;
    }
    return false;
}

/** @brief Tworzy nowy element listy.
 * Alokuje pamięć na nowy element listy.
 * Inicjuje ten element i przypisuje mu wartość @p value.
 * @param value  - wartość nowego elementu listy
 * @return Wskaźnik na utworzony element lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
static list_elem_t* listCreateElem(void *value) {
    list_elem_t *newElem = malloc(sizeof(list_elem_t));
    if (!newElem)
        return NULL;
    newElem->value = value;
    return newElem;
}


list_t *list_new() {
    list_elem_t *leftGuard = listCreateElem(NULL);
    list_elem_t *rightGuard = listCreateElem(NULL);

    leftGuard->left = leftGuard;
    leftGuard->right = rightGuard;
    rightGuard->left = leftGuard;
    rightGuard->right = rightGuard;

    list_t *newList = malloc(sizeof(list_t));
    *newList = (list_t){.left_guard = leftGuard, .right_guard = rightGuard,
                        .logical_size = 0};

    return newList;
}


void list_clear(list_t *list) {
    if (!list)
        return;
    list_elem_t *sentencedElem;
    list_elem_t *currentElem = list->left_guard->right;

    while (currentElem != list->right_guard) {
        sentencedElem = currentElem;
        currentElem = currentElem->right;
        free(sentencedElem);
    }

    list->logical_size = 0;
    list->left_guard->right = list->right_guard;
    list->right_guard->left = list->left_guard;
}


void list_delete(list_t *list) {
    list_clear(list);
    free(list->left_guard);
    free(list->right_guard);
    free(list);
}


void* list_first(list_t *list) {
    assert(!list_is_empty(list));
    return list->left_guard->right->value;
}


void* list_last(list_t *list) {
    assert(!list_is_empty(list));
    return list->right_guard->left->value;
}


void list_append(list_t *list, void *value) {
    assert(list);
    list_elem_t *newElem = listCreateElem(value);
    newElem->left = list->right_guard->left;
    newElem->right = list->right_guard;
    list->right_guard->left->right = newElem;
    list->right_guard->left = newElem;
    ++list->logical_size;
}


void list_prepend(list_t *list, void *value) {
    assert(list);
    list_elem_t *newElem = listCreateElem(value);
    newElem->right = list->left_guard->right;
    newElem->left = list->left_guard;
    list->left_guard->right->left = newElem;
    list->left_guard->right = newElem;
    ++list->logical_size;
}


void list_remove_last(list_t *list) {
    assert(!list_is_empty(list));
    list_elem_t *sentencedElem = list->right_guard->left;

    list->right_guard->left->left->right = list->right_guard;
    list->right_guard->left = list->right_guard->left->left;
    free(sentencedElem);
    --list->logical_size;
}


void list_remove_first(list_t *list) {
    assert(!list_is_empty(list));
    list_elem_t *sentencedElem = list->left_guard->right;

    list->left_guard->right->right->left = list->left_guard;
    list->left_guard->right = list->left_guard->right->right;
    free(sentencedElem);
    --list->logical_size;
}


void* list_pop_first(list_t *list) {
    void *value = list_first(list);
    list_remove_first(list);
    --list->logical_size;
    return value;
}


void* list_pop_last(list_t *list) {
    void *value = list_last(list);
    list_remove_last(list);
    --list->logical_size;
    return value;
}


void list_iter_kamikaze(void (*fun)(void*, void*), list_t *list,
                        void *another_param) {
    assert(list);
    assert(fun);
    list_elem_t *incoming_kamikaze = list->left_guard->right;
    while (incoming_kamikaze != list->right_guard) {
        fun(another_param, incoming_kamikaze->value);
        incoming_kamikaze = incoming_kamikaze->right;
        free(incoming_kamikaze->left);
    }
    free(list->left_guard);
    free(list->right_guard);
    free(list);
}
