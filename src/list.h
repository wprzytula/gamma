#ifndef GAMMA_LIST_H
#define GAMMA_LIST_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct listElem listElem;

typedef listElem* sublist;

struct listElem {
    void *value;
    sublist left;
    sublist right;
    };


typedef struct {
    uint32_t logicalSize;
    sublist leftGuard;
    sublist rightGuard;
} list_t;


bool listIsEmpty(list_t*);


bool listIn(list_t*, void *value);


list_t* listNew();


void listClear(list_t*);


void listDelete(list_t*);


void* listFirst(list_t*);


void* listLast(list_t*);


void* listNth(list_t*, uint32_t index);


void listAppend(list_t*, void *value);


void listPrepend(list_t*, void *value);


void listInsert(list_t*, uint32_t index, void *value);


void listRemoveLast(list_t*);


void listRemoveFirst(list_t*);


void listRemoveNth(list_t*, uint32_t index);


void* listPopFirst(list_t*);


void* listPopLast(list_t*);


void listIterLeft(void (*)(void*), list_t*);


void listIterRight(void (*)(void*), list_t*);


void listIterKamikaze(void (*)(void*, void*), list_t*, void*);


#endif //GAMMA_LIST_H
