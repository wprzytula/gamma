#ifndef GAMMA_QUEUE_H
#define GAMMA_QUEUE_H

#include "list.h"


typedef list_t queue_t;


inline bool queueIsEmpty(queue_t *queue) {
    return listIsEmpty(queue);
}


inline queue_t* queueNew() {
    return listNew();
}


inline void queueDelete(queue_t *queue) {
    return listDelete(queue);
}


inline void* queuePeek(queue_t *queue) {
    return listFirst(queue);
}


inline void* queuePeekLast(queue_t *queue) {
    return listLast(queue);
}


inline void queuePush(queue_t *queue, void* value) {
    listAppend(queue, value);
}


inline void queuePushBeg(queue_t *queue, void* value) {
    listPrepend(queue, value);
}


inline void* queuePop(queue_t *queue) {
    return listPopFirst(queue);
}


inline void* queuePopLast(queue_t *queue) {
    return listPopLast(queue);
}


#endif //GAMMA_QUEUE_H
