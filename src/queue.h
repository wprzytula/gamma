#ifndef GAMMA_QUEUE_H
#define GAMMA_QUEUE_H

#include "list.h"


typedef list_t queue_t;


static inline bool queueIsEmpty(queue_t *queue) {
    return listIsEmpty(queue);
}


static inline queue_t* queueNew() {
    return listNew();
}


static inline void queueDelete(queue_t *queue) {
    return listDelete(queue);
}


static inline void* queuePeek(queue_t *queue) {
    return listFirst(queue);
}


static inline void* queuePeekLast(queue_t *queue) {
    return listLast(queue);
}


static inline void queuePush(queue_t *queue, void* value) {
    listAppend(queue, value);
}


static inline void queuePushBeg(queue_t *queue, void* value) {
    listPrepend(queue, value);
}


static inline void* queuePop(queue_t *queue) {
    return listPopFirst(queue);
}


static inline void* queuePopLast(queue_t *queue) {
    return listPopLast(queue);
}


#endif //GAMMA_QUEUE_H