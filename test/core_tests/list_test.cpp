#include "gtest/gtest.h"

#include "list.h"

#define LIST_SIZE 10

using core::List;


// Ctrl + Shift + F10

TEST(List, Insert) {
    List<int> list;
    int values[] = {0, 1, 2, 3, 5, 6, 7, 8, 9, 10};
    for(int i = 0; i < 10; i++){
        list.add(values[i]);
    }
    list.add(4, 4);
    EXPECT_EQ(list.size(), 11);
    for (int i = 0; i < 11; i++) {
        EXPECT_EQ(list.get(i), i);
    }
}

TEST(List, Size) {
    List<int> list;
    EXPECT_EQ(list.size(), 0);
    list.add(2);
    EXPECT_EQ(list.size(), 1);
    list.add(3);
    EXPECT_EQ(list.size(), 2);
}

TEST(List, Removal) {
    List<int> list;
    for(int i = 0; i < LIST_SIZE; i++){
        list.add(i);

    }
    for (int i = 0; i < LIST_SIZE; ++i) {
        EXPECT_EQ(list.size(), LIST_SIZE - i);
        EXPECT_TRUE(list.remove(i));
    }

    EXPECT_EQ(list.size(), 0);
}

TEST(List, Access) {
    List<int> list;
    for(int i = 0; i < LIST_SIZE; i++){
        list.add(i);

    }
    for (int i = 0; i < LIST_SIZE; ++i) {
        EXPECT_EQ(list.get(i), i);
    }
}

TEST(List, Iterator) {

    EXPECT_EQ(0, 1);

    List<int> list;
    for(int i = 0; i < LIST_SIZE; i++) {
        list.add(i);
    }

//    list.iterator();

//    int count = 0;
//    for (Iterator<int> iter = list.iterator(); iter.isNotEnd(); ++iter) {
//        EXPECT_EQ(*iter, count++);
//        if (count >= LIST_SIZE) break;
//    }
}

