#ifndef GAMMA_QUEUE_H
#define GAMMA_QUEUE_H

#include "list.h"


typedef list_t queue_t;



bool queueIsEmpty(queue_t*);


queue_t* queueNew(size_t elemSize);


void queueDelete(queue_t*);


void* queuePeek(queue_t*);


void* queuePeekLast(queue_t*);


void queuePush(queue_t*, void* value);


void queuePushBeg(queue_t *queue, void* value);


void* queuePop(queue_t*);


void* queuePopLast(list_t*);


#endif //GAMMA_QUEUE_H
