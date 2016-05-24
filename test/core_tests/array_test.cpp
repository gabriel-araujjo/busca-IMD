#include "gtest/gtest.h"

#include "array.h"

using core::Array;

TEST(Array, Size) {
    Array<int> array;
    EXPECT_EQ(array.size(), 0);

    array.resize(10);
    EXPECT_EQ(array.size(), 10);

    array.resize(5);
    EXPECT_EQ(array.size(), 5);

    array.resize(0);
    EXPECT_EQ(array.size(), 0);
}

TEST(Array, Content) {
    Array<int> array(10);

    for (int i = 0; i < 10; i++) {
        array[i] = (i + 30) * 23 % 11;
    }

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(array[i], (i + 30) * 23 % 11);
    }
}

