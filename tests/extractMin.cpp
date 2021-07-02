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

    void checkChildren(const Node* start, const std::vector<char> &vals) {
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

TEST_F(ExtractMin, 1Elem1Ext) {
    insertNodes({1});
    auto* res = extractMin(heap);

    ASSERT_EQ(res->key, 1);
    ASSERT_EQ(heap->size, 0);
    ASSERT_EQ(heap->min, nullptr);
}

TEST_F(ExtractMin, 1Elem2Ext) {
    insertNodes({1});
    auto* res = extractMin(heap);
    res = extractMin(heap);

    ASSERT_EQ(res, nullptr);
}

TEST_F(ExtractMin, 2Elem1Ext) {
    insertNodes({2, 1});
    auto* res = extractMin(heap);

    ASSERT_EQ(res->key, 1);
    checkLevel({2}, heap->min);
    checkChildren(heap->min, {'n'});
    checkLevelParent(heap->min, nullptr);
}

TEST_F(ExtractMin, 2Elem2Ext) {
    insertNodes({2, 1});
    auto* res = extractMin(heap);
    res = extractMin(heap);

    ASSERT_EQ(heap->min, nullptr);
    ASSERT_EQ(heap->size, 0);
}

TEST_F(ExtractMin, 3Elem1Ext) {
    insertNodes({3, 1, 2});
    auto* res = extractMin(heap);

    // 2
    // |
    // 3

    ASSERT_EQ(heap->size, 2);
    // root level
    checkLevel({2}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'3'});

    // children of the root node
    checkLevel({3}, heap->min->child);
    checkLevelParent(heap->min->child, heap->min);
    checkChildren(heap->min->child, {'n'});
}

TEST_F(ExtractMin, 3Elem2Ext) {
    insertNodes({3, 1, 2});
    auto* res = extractMin(heap); // 1
    res = extractMin(heap); // 2

    ASSERT_EQ(heap->size, 1);
    // root level
    checkLevel({3}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'n'});
}

TEST_F(ExtractMin, 3Elem3Ext) {
    insertNodes({3, 1, 2});
    auto* res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);

    ASSERT_EQ(heap->size, 0);
    ASSERT_EQ(heap->min, nullptr);
}

TEST_F(ExtractMin, 4Elem1Ext) {
    insertNodes({4, 2, 3, 1});
    auto* res = extractMin(heap);

    // 2 - 3
    //     |
    //     4

    ASSERT_EQ(heap->size, 3);
    ASSERT_EQ(heap->min->key, 2);

    checkChildren(heap->min, {'n', '4'});
    checkLevelParent(heap->min, nullptr);
    checkLevel({2, 3}, heap->min);

    checkLevelParent(heap->min->left->child, heap->min->left);
    checkLevel({4}, heap->min->left->child);

    checkChildren(heap->min->left->child, {'n'});
}

TEST_F(ExtractMin, 4Elem2Ext) {
    insertNodes({4, 2, 3, 1});
    auto* res = extractMin(heap);
    res = extractMin(heap);

    // 3
    // |
    // 4

    ASSERT_EQ(heap->size, 2);
    ASSERT_EQ(heap->min->key, 3);

    checkLevel({3}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'4'});

    checkLevel({4}, heap->min->child);
    checkChildren(heap->min->child, {'n'});
    checkLevelParent(heap->min->child, heap->min);
}

TEST_F(ExtractMin, 4Elem3Ext) {
    insertNodes({4, 2, 3, 1});
    auto* res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);

    ASSERT_EQ(heap->size, 1);
    ASSERT_EQ(heap->min->key, 4);

    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'n'});
    checkLevel({4}, heap->min);
}

TEST_F(ExtractMin, 4Elem4Ext) {
    insertNodes({4, 2, 3, 1});
    auto* res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);

    ASSERT_EQ(heap->size, 0);
    ASSERT_EQ(heap->min, nullptr);

}

TEST_F(ExtractMin, 5Elem1Ext) {
    insertNodes({1, 5, 4, 3, 2});
    auto* res = extractMin(heap);

    // 2
    // | \
    // 4 3
    // |
    // 5

    ASSERT_EQ(res->key, 1);
    ASSERT_EQ(heap->min->key, 2);
    ASSERT_EQ(heap->size, 4);

    checkLevel({2}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'4', '3'});

    checkLevel({4, 3}, heap->min->child);
    checkLevelParent(heap->min->child, heap->min);
    checkChildren(heap->min->child, {'5', 'n'});

    checkLevel({5}, heap->min->child->child);
    checkLevelParent(heap->min->child->child, heap->min->child);
    checkChildren(heap->min->child->child, {'n'});
}

TEST_F(ExtractMin, 5Elem2Ext) {
    insertNodes({1, 5, 4, 3, 2});
    auto* res = extractMin(heap);
    res = extractMin(heap);

    // 3 4
    //   |
    //   5

    ASSERT_EQ(res->key, 2);
    ASSERT_EQ(heap->min->key, 3);
    ASSERT_EQ(heap->size, 3);

    checkLevel({3, 4}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'n', '5'});

    checkLevel({5}, heap->min->left->child);
    checkLevelParent(heap->min->left->child, heap->min->left);
    checkChildren(heap->min->left->child, {'n'});
}


TEST_F(ExtractMin, 5Elem3Ext) {
    insertNodes({1, 5, 4, 3, 2});
    auto* res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);

    // 4
    // |
    // 5

    ASSERT_EQ(res->key, 3);
    ASSERT_EQ(heap->min->key, 4);
    ASSERT_EQ(heap->size, 2);

    checkLevel({4}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'5'});

    checkLevel({5}, heap->min->child);
    checkLevelParent(heap->min->child, heap->min);
    checkChildren(heap->min->child, {'n'});
}

TEST_F(ExtractMin, 5Elem4Ext) {
    insertNodes({1, 5, 4, 3, 2});
    auto* res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);

    // 5

    ASSERT_EQ(res->key, 4);
    ASSERT_EQ(heap->min->key, 5);
    ASSERT_EQ(heap->size, 1);

    checkLevel({5}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'n'});
}

TEST_F(ExtractMin, 5Elem5Ext) {
    insertNodes({1, 5, 4, 3, 2});
    auto* res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);

    // 5

    ASSERT_EQ(res->key, 5);
    ASSERT_EQ(heap->min, nullptr);
    ASSERT_EQ(heap->size, 0);
}

TEST_F(ExtractMin, 8Elem1Ext) {
    insertNodes({1, 8, 7, 6, 5, 4, 3, 2});
    auto* res = extractMin(heap);

    // 2    6  8
    // | \  |
    // 4 3  7
    // |
    // 5

    ASSERT_EQ(res->key, 1);
    ASSERT_EQ(heap->min->key, 2);
    ASSERT_EQ(heap->size, 7);

    checkLevel({2, 6, 8}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'4', '7', 'n'});

    checkLevel({4, 3}, heap->min->child);
    checkLevelParent(heap->min->child, heap->min);
    checkChildren(heap->min->child, {'5', 'n'});
    checkChildren(heap->min->right->child, {'n'});
    ASSERT_EQ(heap->min->left->child, nullptr);

    checkLevel({5}, heap->min->child->child);
    checkLevelParent(heap->min->child->child, heap->min->child);
    checkChildren(heap->min->child->child, {'n'});
}

TEST_F(ExtractMin, 8Elem2Ext) {
    insertNodes({1, 8, 7, 6, 5, 4, 3, 2});
    auto* res = extractMin(heap);
    res = extractMin(heap);

    // 4    3
    // | \  |
    // 6 5  8
    // |
    // 7

    ASSERT_EQ(res->key, 2);
    ASSERT_EQ(heap->min->key, 3);
    ASSERT_EQ(heap->size, 6);
    ASSERT_EQ(heap->min->degree, 1);
    ASSERT_EQ(heap->min->right->degree, 2);

    checkLevel({3, 4}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'8', '6'});

    checkLevel({6, 5}, heap->min->right->child);
    checkLevelParent(heap->min->right->child, heap->min->right);
    checkChildren(heap->min->right->child, {'7', 'n'});

    checkLevel({7}, heap->min->right->child->child);
    checkLevelParent(heap->min->right->child->child, heap->min->right->child);
    checkChildren(heap->min->right->child->child, {'n'});

    checkLevel({8}, heap->min->child);
    checkLevelParent(heap->min->child, heap->min);
    checkChildren(heap->min->child, {'n'});
}

TEST_F(ExtractMin, 8Elem3Ext) {
    insertNodes({1, 8, 7, 6, 5, 4, 3, 2});
    auto* res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);

    // 4   8
    // | \
    // 6 5
    // |
    // 7

    ASSERT_EQ(res->key, 3);
    ASSERT_EQ(heap->min->key, 4);
    ASSERT_EQ(heap->size, 5);
    ASSERT_EQ(heap->min->degree, 2);
    ASSERT_EQ(heap->min->right->degree, 0);

    checkLevel({4, 8}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'6', 'n'});

    checkLevel({6, 5}, heap->min->child);
    checkLevelParent(heap->min->child, heap->min);
    checkChildren(heap->min->child, {'7', 'n'});

    checkLevel({7}, heap->min->child->child);
    checkLevelParent(heap->min->child->child, heap->min->child);
    checkChildren(heap->min->child->child, {'n'});
}

TEST_F(ExtractMin, 8Elem4Ext) {
    insertNodes({1, 8, 7, 6, 5, 4, 3, 2});
    auto* res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);
    res = extractMin(heap);

    // 5
    // | \
    // 6 8
    // |
    // 7

    ASSERT_EQ(res->key, 4);
    ASSERT_EQ(heap->min->key, 5);
    ASSERT_EQ(heap->size, 4);
    ASSERT_EQ(heap->min->degree, 2);

    checkLevel({5}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'6'});

    checkLevel({6, 8}, heap->min->child);
    checkLevelParent(heap->min->child, heap->min);
    checkChildren(heap->min->child, {'7', 'n'});

    checkLevel({7}, heap->min->child->child);
    checkLevelParent(heap->min->child->child, heap->min->child);
    checkChildren(heap->min->child->child, {'n'});
}

TEST_F(ExtractMin, 9Elem1Ext) {
    insertNodes({1, 9, 8, 7, 6, 5, 4, 3, 2});
    auto* res = extractMin(heap);

    // 2
    // | \  \
    // 6  3  4
    // |\    |
    // 8 7   5
    // |
    // 9

    ASSERT_EQ(res->key, 1);
    ASSERT_EQ(heap->min->key, 2);
    ASSERT_EQ(heap->size, 8);

    checkLevel({2}, heap->min);
    checkLevelParent(heap->min, nullptr);
    checkChildren(heap->min, {'6', '3', '4'});

    checkLevel({6, 3, 4}, heap->min->child);
    checkLevelParent(heap->min->child, heap->min);
    checkChildren(heap->min->child, {'8', 'n', '5'});
    ASSERT_EQ(heap->min->child->child->left->key, 7);

    checkLevel({8, 7}, heap->min->child->child);
    checkLevelParent(heap->min->child->child, heap->min->child);
    checkChildren(heap->min->child->child, {'9', 'n'});

    checkLevel({5}, heap->min->child->left->child);
    checkLevelParent(heap->min->child->left->child, heap->min->child->left);
    checkChildren(heap->min->child->left->child, {'n'});
}