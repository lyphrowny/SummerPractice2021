#include <gtest/gtest.h>

// if .h -> linker issues
#include "../fib_heap.c"
//#include <hash_table.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
