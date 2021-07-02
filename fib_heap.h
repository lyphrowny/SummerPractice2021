#pragma once

typedef struct Node {
    struct Node* left, * right, * child, * parent;
    int key, degree;
    int isMarked;
} Node_t;

typedef struct Heap {
    int size;
    Node_t* min;
} Heap_t;

Heap_t* initHeap();
Heap_t* addElem(Heap_t* heap, int key);
Node_t* extractMin(Heap_t* heap);
void deleteHeap(Heap_t* heap);