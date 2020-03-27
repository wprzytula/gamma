#include <stdlib.h>
#include "list.h"
#include <assert.h>


bool listIsEmpty(list_t *list) {
    return list->leftGuard->right == list->rightGuard;
}


static listElem* listCreateElem(holdType value) {
    listElem *newElem = malloc(sizeof(listElem));
    ///TODO: error handling
    newElem->value = value;
    return newElem;
}


list_t* listNew() {
    sublist leftGuard = listCreateElem('\v');
    sublist rightGuard = listCreateElem('\v');
    *leftGuard = (listElem){.left = leftGuard, .right = rightGuard};
    *rightGuard = (listElem){.left = leftGuard, .right = rightGuard};
    list_t *newList = malloc(sizeof(list_t));
    *newList = (list_t){.leftGuard = leftGuard, .rightGuard = rightGuard};
    return newList;
}


void listDelete(list_t *list) {
    listElem *sentencedElem;
    sublist currentElem = list->leftGuard;
    do {
        sentencedElem = currentElem;
        currentElem = currentElem->right;
        free(sentencedElem);
    } while (sentencedElem != list->rightGuard);

    free(list);
}



holdType listFirst(list_t *list) {
    assert(!listIsEmpty(list));
    return list->leftGuard->right->value;
}


holdType listLast(list_t *list) {
    assert(!listIsEmpty(list));
    return list->rightGuard->left->value;
}


static listElem* listNthElem(list_t *list, uint32_t index) {
    assert(!listIsEmpty(list));
    sublist elemPtr;
    if (index * 2 > list->size) {
        elemPtr = list->rightGuard->left;
        for (uint32_t i = list->size - 1; i > index; --i) {
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


holdType listNth(list_t *list, uint32_t index) {
    return listNthElem(list, index)->value;
}


void listAppend(list_t *list, holdType value) {
    listElem *newElem = listCreateElem(value);
    newElem->left = list->rightGuard->left;
    newElem->right = list->rightGuard;
    list->rightGuard->left->right = newElem;
    list->rightGuard->left = newElem;
}


void listPrepend(list_t *list, holdType value) {
    listElem *newElem = listCreateElem(value);
    newElem->right = list->leftGuard->right;
    newElem->left = list->leftGuard;
    list->leftGuard->right->left = newElem;
    list->leftGuard->right = newElem;
}


void listInsert(list_t *list, uint32_t index, holdType value) {
    listElem *placeAfter = listNthElem(list, index);
    listElem *placeBefore = placeAfter->left;
    listElem *newElem = listCreateElem(value);

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



void listIterLeft(void (*fun)(char), list_t *list) {
    listElem *currentElem = list->leftGuard->right;
    while (currentElem != list->rightGuard) {
        fun(currentElem->value);
        currentElem = currentElem->right;
    }
}



void listIterRight(void (*fun)(char), list_t *list) {
    listElem *currentElem = list->rightGuard->left;
    while (currentElem != list->leftGuard) {
        fun(currentElem->value);
        currentElem = currentElem->left;
    }
}
