#include "queue.h"


bool queueIsEmpty(queue_t *queue) {
    return listIsEmpty(queue);
}


queue_t* queueNew() {
    return listNew();
}


void queueDelete(queue_t *queue) {
    return listDelete(queue);
}


holdType queuePeek(queue_t *queue) {
    return listFirst(queue);
}


holdType queuePeekLast(queue_t *queue) {
    return listLast(queue);
}


void queuePush(queue_t *queue, holdType value) {
    listAppend(queue, value);
}


void queuePushBeg(queue_t *queue, holdType value) {
    listPrepend(queue, value);
}


holdType queuePop(queue_t *queue) {
    holdType popped = queuePeek(queue);
    listRemoveFirst(queue);
    return popped;
}


holdType queuePopLast(queue_t *queue) {
    holdType popped = queuePeekLast(queue);
    listRemoveLast(queue);
    return popped;
}
