#include "list.h"

bool listIsEmpty(list_t *list) {
    return list->leftGuard->right == list->rightGuard;
}


static listElem* listCreateElem(size_t elemSize, void *value) {
    listElem *newElem = malloc(sizeof(listElem));
    newElem->value = malloc(elemSize);
    ///TODO: error handling
    if (value)
        memcpy(newElem->value, value, elemSize);
    else
        newElem->value = NULL;
    return newElem;
}


list_t* listNew(size_t elemSize, freer_t freer) {
    sublist leftGuard = listCreateElem(elemSize, NULL);
    sublist rightGuard = listCreateElem(elemSize, NULL);

    leftGuard->left = leftGuard;
    leftGuard->right = rightGuard;
    rightGuard->left = leftGuard;
    rightGuard->right = rightGuard;

    list_t *newList = malloc(sizeof(list_t));
    *newList = (list_t){.leftGuard = leftGuard, .rightGuard = rightGuard,
                        .freer = freer, .logicalSize = 0, .elemSize = elemSize};

    return newList;
}


void listDelete(list_t *list) {
    listElem *sentencedElem;
    sublist currentElem = list->leftGuard;
    do {
        sentencedElem = currentElem;
        currentElem = currentElem->right;
        if (list->freer)
            list->freer(sentencedElem->value);
        free(sentencedElem);
    } while (sentencedElem != list->rightGuard);

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
    listElem *newElem = listCreateElem(list->elemSize, value);
    newElem->left = list->rightGuard->left;
    newElem->right = list->rightGuard;
    list->rightGuard->left->right = newElem;
    list->rightGuard->left = newElem;
}


void listPrepend(list_t *list, void *value) {
    listElem *newElem = listCreateElem(list->elemSize, value);
    newElem->right = list->leftGuard->right;
    newElem->left = list->leftGuard;
    list->leftGuard->right->left = newElem;
    list->leftGuard->right = newElem;
}


void listInsert(list_t *list, uint32_t index, void *value) {
    listElem *placeAfter = listNthElem(list, index);
    listElem *placeBefore = placeAfter->left;
    listElem *newElem = listCreateElem(list->elemSize, value);

    placeBefore->right = newElem;
    placeAfter->left = newElem;
    newElem->right = placeAfter;
    newElem->left = placeAfter;
}


void listRemoveLast(list_t *list) {
    assert(!listIsEmpty(list));
    listElem *sentencedElem = list->rightGuard->left;

    list->rightGuard->left->left->right = list->rightGuard;
    list->rightGuard->left = list->rightGuard->left->left;
    free(sentencedElem);
}


void listRemoveFirst(list_t *list) {
    assert(!listIsEmpty(list));
    listElem *sentencedElem = list->leftGuard->right;

    list->leftGuard->right->right->left = list->leftGuard;
    list->leftGuard->right = list->leftGuard->right->right;
    free(sentencedElem);
}


void listRemoveNth(list_t *list, uint32_t index) {
    assert(!listIsEmpty(list));
    listElem *sentencedElem = listNthElem (list, index);

    sentencedElem->left->right = sentencedElem->right;
    sentencedElem->right->left = sentencedElem->left;
    free(sentencedElem);

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
