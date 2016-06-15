#include "gtest/gtest.h"

#include "list.h"

#define LIST_SIZE 10

using busca_imd_core::List;
using busca_imd_core::Iterator;

struct bigStruct {
    long long int a;
    long long int b;
    long long int c;
    long long int d;
    long long int e;
    long long int f;
    long long int g;
};

int int_comparator (const int & a, const int & b) {
    return a - b;
}

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

TEST(List, Stress) {

    List<bigStruct> list;

    std::cout << std::endl <<  "sizeof bigStruct" << sizeof(bigStruct) << std::endl;

    for (unsigned int i = 0; i < 10000; i++) {
        bigStruct a;
        list.add(a);
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

    EXPECT_FALSE(list.remove(0));

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

    List<int> list;


    // Não pode iterar numa lista vazia
    for (Iterator<int> iter = list.begin(); iter.isNotEnd();) {
        FAIL();
    }

    for(int i = 0; i < LIST_SIZE; i++) {
        list.add(i);
    }

    int count = 0;
    for (Iterator<int> iter = list.begin(); iter.isNotEnd(); ++iter) {

        EXPECT_EQ(*iter, count++);
        if (count > LIST_SIZE) {
            //O iterator estorou o tamanho da lista
            FAIL();
        }
    }
}

TEST(List, CopyConstructor) {
    List<int> list1;
    for (int i = 0; i < LIST_SIZE; ++i) {
        list1.add(i);
    }

    List<int> list2 = list1;
    EXPECT_EQ(list1.size(), list2.size());
    Iterator<int> i1 = list1.begin(),
        i2 = list2.begin();
    for (; i1.isNotEnd() && i2.isNotEnd(); ++i1, ++i2) {
        EXPECT_EQ(*i1, *i2);
        EXPECT_NE(&(*i1), &(*i2));
    }

    i1 = list1.begin();
}

TEST(List, AssignmentOperator) {
    List<int> list1, list2;
    for (int i = 0; i < LIST_SIZE; ++i) {
        list1.add(i);
    }

    list2 = list1;

    EXPECT_EQ(list1.size(), list2.size());

    for (int i = 0; i < LIST_SIZE; ++i) {
        EXPECT_EQ(list1.get(i), list2.get(i));
    }


    list2.add(LIST_SIZE, 0);

    for (int i = 0; i < LIST_SIZE; ++i) {
        EXPECT_EQ(list1.get(i), list2.get(i+1));
    }

    EXPECT_EQ(list2.get(0), LIST_SIZE);
}

TEST(List, Clear) {
    List<int> list;
    for(int i = 0; i< 10; ++i) {
        list.add(i);
    }
    EXPECT_EQ(list.size(), 10);
    list.clear();
    EXPECT_EQ(list.size(), 0);
    for (int el : list) {
        FAIL();
    }
    for(int i = 0; i< 10; ++i) {
        list.add(i);
    }
    EXPECT_EQ(list.size(), 10);
    int i = 0;
    for (int el : list) {
        EXPECT_EQ(el, i++);
    }
    EXPECT_EQ(i, 10);
}

TEST(List, Ordering) {
    List<int> list;
    list.add(20);
    list.add(3);
    list.add(2);
    list.add(1);
    list.add(6);
    list.add(5);
    list.add(4);

    int sizeBeforeSorting = list.size();
    list.sort(int_comparator);
    bool firstElement = true;
    int prevElement;
    for (int el : list) {
        if (firstElement) {
            firstElement = false;
            prevElement = el;
            continue;
        }
        EXPECT_GE(el, prevElement);
        prevElement = el;
    }
    EXPECT_EQ(list.size(), sizeBeforeSorting);
}