#include "queue.h"


bool queueIsEmpty(queue_t *queue) {
    return listIsEmpty(queue);
}


queue_t* queueNew(size_t elemSize) {
    return listNew(elemSize);
}


void queueDelete(queue_t *queue) {
    return listDelete(queue);
}


void* queuePeek(queue_t *queue) {
    return listFirst(queue);
}


void* queuePeekLast(queue_t *queue) {
    return listLast(queue);
}


void queuePush(queue_t *queue, void* value) {
    listAppend(queue, value);
}


void queuePushBeg(queue_t *queue, void* value) {
    listPrepend(queue, value);
}


void* queuePop(queue_t *queue) {
    void* popped = queuePeek(queue);
    listRemoveFirst(queue);
    return popped;
}


void* queuePopLast(queue_t *queue) {
    void* popped = queuePeekLast(queue);
    listRemoveLast(queue);
    return popped;
}
