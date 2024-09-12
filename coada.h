/* Antonescu Albert - 314CD */

#pragma once

#include <string.h>
#include <stdlib.h>

typedef struct cnode {
    void *data;
    struct cnode *next;
} cNode;

typedef struct queue {
    cNode *head, *tail;
} *TQueue;

cNode *initNode(void *data) {
    cNode *node = (cNode*)malloc(sizeof(struct cnode));
    node->data = data;
    node->next = NULL;
    return node;
}

cNode *freeNode(cNode *node) {
    if (node)
        free(node);
    return NULL;
}

TQueue initQueue(void *data) {
    TQueue queue = (TQueue)malloc(sizeof(struct queue));
    queue->head = queue->tail = initNode(data);
    return queue;
}

TQueue init() {
    return NULL;
}
int isEmpty(TQueue queue) {
    return queue == NULL || queue->head == NULL;
}

TQueue enqueue(TQueue queue, void *data) {
    cNode *node;
    if (isEmpty(queue)) {
        if (queue == NULL)
            return initQueue(data);
        free(queue);
        return initQueue(data);
    }
    node = initNode(data);
    queue->tail->next = node;
    queue->tail = node;
    return queue;
}
TQueue dequeue(TQueue queue) {
    cNode *tmp;
    if (!isEmpty(queue)) {
        tmp = queue->head;
        queue->head = queue->head->next;
        tmp = freeNode(tmp);
    }
    return queue;
}

void* front(TQueue queue) {
    if (!isEmpty(queue))
        return queue->head->data;
    else
        exit(1);
}

TQueue freeQueue(TQueue queue) {
    while (!isEmpty(queue))
        queue = dequeue(queue);
    if (queue)
        free(queue);
    return NULL;
}