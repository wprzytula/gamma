#ifndef GAMMA_LIST_H
#define GAMMA_LIST_H

#include <stdbool.h>
#include <stdint.h>

#define holdType char


typedef struct listElem listElem;

typedef listElem* sublist;

struct listElem {
    holdType value;
    sublist left;
    sublist right;
    };


typedef struct {
    uint32_t size;
    sublist leftGuard;
    sublist rightGuard;
} list_t;


bool listIsEmpty(list_t*);


list_t* listNew();


void listDelete(list_t*);


holdType listFirst(list_t*);


holdType listLast(list_t*);


holdType listNth(list_t*, uint32_t index);


void listAppend(list_t*, holdType value);


void listPrepend(list_t*, holdType value);


void listInsert(list_t*, uint32_t index, holdType value);


void listRemoveLast(list_t*);


void listRemoveFirst(list_t*);


void listRemoveNth(list_t*, uint32_t index);


void listIterLeft(void (*)(holdType), list_t*);


void listIterRight(void (*)(holdType), list_t*);




#endif //GAMMA_LIST_H
