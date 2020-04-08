#include "list.h"
#include <string.h>
#include <assert.h>
//#include <stdio.h>


bool listIsEmpty(list_t *list) {
    return list->leftGuard->right == list->rightGuard;
}


bool listIn(list_t *list, void *value) {
    listElem *currentElem = list->leftGuard->right;
    while (currentElem != list->rightGuard) {
        if (currentElem->value == value)
            return true;
        else
            currentElem = currentElem->right;
    }
    return false;
}


static listElem* listCreateElem(void *value) {
    listElem *newElem = malloc(sizeof(listElem));
    ///TODO: error handling
    if (value)
        newElem->value = value;
    else
        newElem->value = NULL;
    return newElem;
}


list_t *listNew() {
    sublist leftGuard = listCreateElem(NULL);
    sublist rightGuard = listCreateElem(NULL);

    leftGuard->left = leftGuard;
    leftGuard->right = rightGuard;
    rightGuard->left = leftGuard;
    rightGuard->right = rightGuard;

    list_t *newList = malloc(sizeof(list_t));
    *newList = (list_t){.leftGuard = leftGuard, .rightGuard = rightGuard,
                        .logicalSize = 0};

    return newList;
}


void listClear(list_t *list) {
    listElem *sentencedElem;
    sublist currentElem = list->leftGuard->right;

    while (currentElem != list->rightGuard) {
        sentencedElem = currentElem;
        currentElem = currentElem->right;
        free(sentencedElem);
    }

    list->logicalSize = 0;
    list->leftGuard->right = list->rightGuard;
    list->rightGuard->left = list->leftGuard;
}



void listDelete(list_t *list) {
    listClear(list);
    free(list->leftGuard);
    free(list->rightGuard);
    free(list);
}



void* listFirst(list_t *list) {
    assert(!listIsEmpty(list));
    assert(list->leftGuard->right->value);
    return list->leftGuard->right->value;
}


void* listLast(list_t *list) {
    assert(!listIsEmpty(list));
    assert(list->leftGuard->right->value);
    return list->rightGuard->left->value;
}


static listElem* listNthElem(list_t *list, uint32_t index) {
    assert(!listIsEmpty(list));
    sublist elemPtr;
    if (index * 2 > list->logicalSize) {
        elemPtr = list->rightGuard->left;
        for (uint32_t i = list->logicalSize - 1; i > index; --i) {
            elemPtr = elemPtr->left;
        }
    }
    else {
        elemPtr = list->leftGuard->right;
        for (uint32_t i = 0; i < index; ++i) {
            elemPtr = elemPtr->right;
        }
    }
    return elemPtr;
}


void* listNth(list_t *list, uint32_t index) {
    return listNthElem(list, index)->value;
}


void listAppend(list_t *list, void *value) {
    listElem *newElem = listCreateElem(value);
    newElem->left = list->rightGuard->left;
    newElem->right = list->rightGuard;
    list->rightGuard->left->right = newElem;
    list->rightGuard->left = newElem;
    ++list->logicalSize;
}


void listPrepend(list_t *list, void *value) {
    listElem *newElem = listCreateElem(value);
    newElem->right = list->leftGuard->right;
    newElem->left = list->leftGuard;
    list->leftGuard->right->left = newElem;
    list->leftGuard->right = newElem;
    ++list->logicalSize;
}


void listInsert(list_t *list, uint32_t index, void *value) {
    listElem *placeAfter = listNthElem(list, index);
    listElem *placeBefore = placeAfter->left;
    listElem *newElem = listCreateElem(value);

    placeBefore->right = newElem;
    placeAfter->left = newElem;
    newElem->right = placeAfter;
    newElem->left = placeAfter;
    ++list->logicalSize;
}


void listRemoveLast(list_t *list) {
    assert(!listIsEmpty(list));
    listElem *sentencedElem = list->rightGuard->left;

    list->rightGuard->left->left->right = list->rightGuard;
    list->rightGuard->left = list->rightGuard->left->left;
    free(sentencedElem);
    --list->logicalSize;
}


void listRemoveFirst(list_t *list) {
    assert(!listIsEmpty(list));
    listElem *sentencedElem = list->leftGuard->right;

    list->leftGuard->right->right->left = list->leftGuard;
    list->leftGuard->right = list->leftGuard->right->right;
    free(sentencedElem);
    --list->logicalSize;
}


void listRemoveNth(list_t *list, uint32_t index) {
    assert(!listIsEmpty(list));
    listElem *sentencedElem = listNthElem (list, index);

    sentencedElem->left->right = sentencedElem->right;
    sentencedElem->right->left = sentencedElem->left;
    free(sentencedElem);
    --list->logicalSize;
}


void* listPopFirst(list_t *list) {
    void *value = listFirst(list);
    listRemoveFirst(list);
    --list->logicalSize;
    return value;
}


void* listPopLast(list_t *list) {
    void *value = listLast(list);
    listRemoveLast(list);
    --list->logicalSize;
    return value;
}


void listIterLeft(void (*fun)(void*), list_t *list) {
    listElem *currentElem = list->leftGuard->right;
    while (currentElem != list->rightGuard) {
        fun(currentElem->value);
        currentElem = currentElem->right;
    }
}


void listIterRight(void (*fun)(void*), list_t *list) {
    listElem *currentElem = list->rightGuard->left;
    while (currentElem != list->leftGuard) {
        fun(currentElem->value);
        currentElem = currentElem->left;
    }
}


void listIterKamikaze(void (*fun)(void*, void*), list_t *list,
                      void *anotherParam) {
    sublist incomingKamikaze = list->leftGuard->right;
    while (incomingKamikaze != list->rightGuard) {
        fun(anotherParam, incomingKamikaze->value);
        incomingKamikaze = incomingKamikaze->right;
        free(incomingKamikaze->left);
    }
    free(list->leftGuard);
    free(list->rightGuard);
    free(list);
}
