#include "gtest/gtest.h"

#include "array.h"

#define ARRAY_SIZE 10
#define ARRAY_CONTENT {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

using core::Array;

class ArrayFixture : public ::testing::Test {
protected:
    virtual void TearDown() {

    }

    virtual void SetUp() {
        resizedArray.resize(ARRAY_SIZE);
        resizedArray.resize(ARRAY_SIZE * 2);
        resizedArray.resize(ARRAY_SIZE);

        for (int i = 0; i < ARRAY_SIZE; ++i) {
            filledArray[i] = content[i];
        }
    }

public:

    Array<int> emptyArray;
    Array<int> resizedArray;
    Array<int> filledArray = Array<int>(10);

    int content[ARRAY_SIZE] = ARRAY_CONTENT;
};

TEST_F(ArrayFixture, array_size) {
    EXPECT_EQ(emptyArray.size(), 0);
    EXPECT_EQ(resizedArray.size(), ARRAY_SIZE);
    EXPECT_EQ(filledArray.size(), ARRAY_SIZE);
}

TEST_F(ArrayFixture, array_content) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        EXPECT_EQ(filledArray[i], content[i]);
    }
}

