#include <stdio.h>

#include "fib_heap.h"

int main() {
    Heap_t* heap = initHeap();
    int elems[] = {5, 1, 3, 2, 4, 0, 7, 6};
    for (int i = 0; i < sizeof(elems) / sizeof(elems[0]); i++)
        addElem(heap, elems[i]);
    Node_t* start = heap->min, * curr = start;
    do {
        printf("%d ", curr->key);
        curr = curr->right;
    } while (curr != start);
    printf("\n");
    fflush(stdout);

    for (int i = 0; i < sizeof(elems) / sizeof(elems[0]); i++) {
        Node_t* res = extractMin(heap);
        if (res->key != i)
            printf("Oops, shouldn't have happened\n");
        else
            printf("min: %d, curr: %d\n", res->key, i);
        fflush(stdout);
    }
    return 0;
}
