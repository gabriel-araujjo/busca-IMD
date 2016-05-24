#include "gtest/gtest.h"

#include "list.h"

using core::List

class ListFixure : public ::testing::Test {
protected:
    virtual void TearDown() {

    }

    virtual void SetUp() {
    }

public:

    List<int> list;
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



