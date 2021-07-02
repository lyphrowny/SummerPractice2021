#include <vector>

#include <gtest/gtest.h>
#include "../fib_heap.h"

class ExtractMin : public testing::Test {
protected:
    Heap_t* heap = nullptr;

    ExtractMin() { heap = initHeap(); }

    ~ExtractMin() { deleteHeap(heap); };

    Heap_t* insertNodes(const std::vector<int> &vals) {
        for (auto val : vals)
            heap = addElem(heap, val);
        return heap;
    }

    void checkLevel(const std::vector<int> &vals, const Node_t* start) {
        auto* curr = start;
        auto val = vals.begin();
        auto rval = vals.rbegin();
        do {
            ASSERT_EQ(curr->key, *val++);
            curr = curr->right;
        } while (curr != start);
        curr = start->left;
        do {
            ASSERT_EQ(curr->key, *rval++);
            curr = curr->left;
        } while (curr->right != start);
    }

    void checkLevelParent(const Node_t* start, const Node_t* parent) {
        auto* curr = start;
        do {
            ASSERT_EQ(curr->parent, parent);
            curr = curr->right;
        } while (curr != start);
    }

    void checkChildren(const Node* start, const std::vector<char>& vals) {
        auto* curr = start;
        auto val = vals.begin();
        do {
            if (*val == 'n')
                ASSERT_EQ(curr->child, nullptr);
            else
                ASSERT_EQ(curr->child->key, static_cast<int>(*val) - static_cast<int>('0'));
            val++;
            curr = curr->right;
        } while (curr != start);
    }

};

TEST_F(ExtractMin, OneElem) {
    heap = addElem(heap, 1);

    ASSERT_EQ(heap->min->key, 1);
    ASSERT_EQ(heap->min->left, heap->min);
    ASSERT_EQ(heap->min->right, heap->min);
    ASSERT_EQ(heap->min->parent, nullptr);
    ASSERT_EQ(heap->min->child, nullptr);
    ASSERT_EQ(heap->min->degree, 0);
    ASSERT_EQ(heap->size, 1);
}

TEST_F(ExtractMin, TwoElems) {
    std::vector<int> elems = {1, 2};
    insertNodes(elems);

    checkLevel(elems, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'n', 'n'});
    ASSERT_EQ(heap->size, elems.size());
}

TEST_F(ExtractMin, ThreeElems) {
    std::vector<int> elems = {3, 1, 2};
    insertNodes(elems);

    checkLevel(elems, heap->min->left); // left, since min is 1, so 1, 2, 3
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'n', 'n', 'n'});
    ASSERT_EQ(heap->size, elems.size());
}

TEST_F(ExtractMin, FourElems) {
    std::vector<int> elems = {4, 2, 3, 1};
    insertNodes(elems);

    checkLevel({4, 2, 1, 3}, heap->min->right->right); // 4, 2, 1, 3
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'n', 'n', 'n', 'n'});
    ASSERT_EQ(heap->size, elems.size());
}
