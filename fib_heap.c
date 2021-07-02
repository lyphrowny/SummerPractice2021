#include <stdio.h>
#include <malloc.h>

#include "fib_heap.h"


Heap_t* initHeap() {
    Heap_t* heap = (Heap_t*) malloc(sizeof(Heap_t));
    if (heap != NULL) {
        heap->min = NULL;
        heap->size = 0;
    }
    return heap;
}

Node_t* _newNode(int key) {
    Node_t* node = (Node_t*) malloc(sizeof(Node_t));
    if (node != NULL) {
        node->left = NULL;
        node->right = NULL;
        node->child = NULL;
        node->parent = NULL;
        node->key = key;
        node->degree = 0;
        node->isMarked = 0;
    }
    return node;
}

void _insertNode(Node_t* a, Node_t* b, Node_t* c) {
    b->left = a;
    a->right = b;

    b->right = c;
    c->left = b;
}

void _removeNode(Node_t* a, Node_t* b, Node_t* c) {
    a->right = c;
    c->left = a;
}

Heap_t* addElem(Heap_t* heap, int key) {
    Node_t* node = _newNode(key);
    if (heap == NULL)
        heap = initHeap();
    if (heap->min == NULL && heap->size == 0) { // two conditions are redundant, but
        heap->min = node;
        node->left = node;
        node->right = node;
    } else {
        _insertNode(heap->min, node, heap->min->right);
        if (key < heap->min->key)
            heap->min = node;
    }
    heap->size++;
    return heap;
}

void _heapLink(Heap_t* heap, Node_t* a, Node_t* b) {
    _removeNode(a->left, a, a->right); // remove from the roots' list
    if (b->child)
        _insertNode(b->child, a, b->child->right); // make the child (to the right of the child ptr)
    else
        _insertNode(a, a, a); // if node had no children, this one becomes a one-noded list

    // TODO move up, change conditions
    a->parent = b;
    b->child = a;
    b->degree++;
}

int _log2(int num) {
    int res = 1;
    while (num >>= 1) res++;
    return res;
}

void _consolidate(Heap_t* heap) {
    int max_degree = _log2(heap->size);
    Node_t** degrees = (Node_t**) calloc(max_degree, sizeof(Node_t*));
    Node_t* start = heap->min, * curr = heap->min;
    do {
        int degree = curr->degree;
        Node_t* next = curr->right;
        while (degrees[degree] != NULL) {
            Node_t* node = degrees[degree];
            if (curr->key > node->key) {
                Node_t* tmp = curr;
                curr = node;
                node = tmp;
            }
            if (node == start)
                start = next->left; // if node == start, start gets removed from root list
            if (node == next)
                next = next->right; // if node == next, next gets removed from root list
            _heapLink(heap, node, curr);
            degrees[degree++] = NULL;
        }
        degrees[degree] = curr;
        curr = next;
    } while (curr != start);
    heap->min = NULL;

    for (int i = 0; i < max_degree; i++)
        if (degrees[i] != NULL && (heap->min == NULL || degrees[i]->key < heap->min->key))
            heap->min = degrees[i];
}

Node_t* extractMin(Heap_t* heap) {
    if (heap == NULL || heap->min == NULL)
        return NULL;
    Node_t* min = heap->min, * child = min->child, * next_child, * start = child;
    if (child != NULL) {
        do {
            next_child = child->right;
            _insertNode(min, child, min->right);
            child->parent = NULL;
            child = next_child;
        } while (child != start);
    }
    _removeNode(min->left, min, min->right);

    if (min == min->right) // one and only node
        heap->min = NULL;
    else {
        heap->min = heap->min->right; // moving the min to the next node
        _consolidate(heap);
    }
    heap->size--;
    return min;
}

void _deleteNode(Node_t* node) {
    if (node == NULL)
        return;
    Node_t* curr = node->child;
    if (curr != NULL) {
        Node_t* start = curr;
        do {
            _deleteNode(curr->child);
            curr = curr->right;
        } while (curr != start);
    }
    free(curr);
}

void deleteHeap(Heap_t* heap) {
    if (heap == NULL)
        return;
    Node_t* curr = heap->min;
    if (curr != NULL) {
        Node_t* start = curr;
        do {
            _deleteNode(curr);
            curr = curr->right;
        } while (curr != start);
    }
}