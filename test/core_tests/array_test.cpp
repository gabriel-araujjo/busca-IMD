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
    Array<int> array(10, 0);

    for (int i = 0; i < 10; i++) {
        array[i] = (i + 30) * 23 % 11;
    }

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(array[i], (i + 30) * 23 % 11);
    }
}

TEST(Array, FillWithConstructor) {
    Array<int> array(10, 0);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(array[i], 0);
    }
}

TEST(Array, AssignmentOperator) {
    Array<int> array(4, 1);
    Array<int> array2(5);

    array2 = array;
    EXPECT_NE(&array, &array2);
    EXPECT_EQ(array.size(), array2.size());
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(array[i], array2[i]);
    }
}

