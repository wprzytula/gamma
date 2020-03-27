#ifndef GAMMA_QUEUE_H
#define GAMMA_QUEUE_H

#include "list.h"


typedef list_t queue_t;



bool queueIsEmpty(queue_t*);


queue_t* queueNew();


void queueDelete(queue_t*);


holdType queuePeek(queue_t*);


holdType queuePeekLast(queue_t*);


void queuePush(queue_t*, holdType value);


void queuePushBeg(queue_t *queue, holdType value);


holdType queuePop(queue_t*);


holdType queuePopLast(list_t*);


#endif //GAMMA_QUEUE_H
