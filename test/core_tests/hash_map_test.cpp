//
// Created by gabriel on 5/26/16.
//

#include <iostream>

#include "gtest/gtest.h"
#include "hash_map.h"

using busca_imd_core::HashMap;
using busca_imd_core::Iterator;
using std::cout;
using std::endl;

unsigned int intHash(const int & key) {
    return (unsigned int) key;
}

unsigned int intPtrHash(int * const & key) {
    return key ? *key : 0;
}

TEST(HashMap, Put) {
    HashMap<int, int> hashMap(intHash);
    for(int i = 0; i< 10; ++i) {
        hashMap.put(i, i);
        hashMap.put(i, i*10);
    }
    EXPECT_EQ(hashMap.size(), 10);
}

TEST(HashMap, Get) {
    HashMap<int, int> hashMap(intHash);
    for(int i = 0; i< 10; ++i) {
        hashMap.put(i, i);
        hashMap.put(i, i*10);
    }

    for(int i = 0; i< 10; ++i) {
        EXPECT_EQ(hashMap.get(i), i*10);
    }
}

TEST(HashMap, Remove) {
    HashMap<int, int> hashMap(intHash);
    for(int i = 0; i< 10; ++i) {
        hashMap.put(i*8, i*10);
    }

    EXPECT_THROW(hashMap.remove(10), int);

    for(int i = 0; i< 10; ++i) {
        EXPECT_EQ(hashMap.remove(i*8), i*10);
    }

    EXPECT_THROW(hashMap.remove(9), int);

    EXPECT_EQ(hashMap.size(), 0);
}

TEST(HashMap, Iterator) {
    HashMap<int, int> hashMap(intHash);

    // It must not iterate over an empty list
    for (HashMap<int, int>::Entry entry : hashMap) {
        FAIL();
    }

    for (int i = 0; i < 10; ++i) {
        hashMap.put(i,i);
    }


    int count = 0;
    for (HashMap<int, int>::Entry entry : hashMap) {
        EXPECT_EQ(entry.value, count++);
    }
    EXPECT_EQ(count, hashMap.size());
}

TEST(HashMap, CopyConstructor) {
    HashMap<int, int> hashMap1(intHash);
    for (int i = 0; i < 10; ++i) {
        hashMap1.put(i, i * 10);
    }

    HashMap<int, int> hashMap2 = hashMap1;
    EXPECT_EQ(hashMap1.size(), hashMap2.size());
    Iterator<HashMap<int, int>::Entry> i1 = hashMap1.begin(),
            i2 = hashMap2.begin();
    for (; i1.isNotEnd() && i2.isNotEnd(); ++i1, ++i2) {
        HashMap<int, int >::Entry entry1 = *i1,
                                entry2 = *i2;
        EXPECT_EQ(entry1.key, entry2.key);
        EXPECT_EQ(entry1.value, entry2.value);
        EXPECT_NE(&entry1, &entry2);
    }


    i1 = hashMap1.begin();
    i2 = hashMap1.begin();

    for (; i1.isNotEnd() && i2.isNotEnd(); ++i1, ++i2) {
        EXPECT_EQ((*i1).key, (*i2).key);
        EXPECT_EQ((*i1).value, (*i2).value);
        EXPECT_EQ(&(*i1), &(*i2));
    }
}

TEST(HashMap, AssignmentOperator) {
    HashMap<int, int> hashMap1(intHash), hashMap2(intHash);
    for (int i = 0; i < 10; ++i) {
        hashMap1.put(i, i * 10);
    }

    hashMap2 = hashMap1;
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(hashMap1.get(i), hashMap2.get(i));
    }

    hashMap1.put(0, -1);
    hashMap2.put(1, -1);

    EXPECT_EQ(hashMap1.get(0), -1);
    EXPECT_EQ(hashMap2.get(0), 0);
    EXPECT_EQ(hashMap1.get(1), 10);
    EXPECT_EQ(hashMap2.get(1), -1);
}


TEST(HashMap, DuplicateKeyInsertion) {
    HashMap<int, int> simpleHashMap(intHash);
    HashMap<int *, int> pointerHashMap(intPtrHash);

    int i1 = 1, i2 = 1;
    simpleHashMap.put(i1, i1);
    simpleHashMap.put(i2, i2);

    pointerHashMap.put(&i1, i1);
    pointerHashMap.put(&i2, i2);

    EXPECT_EQ(simpleHashMap.size(), 1);
    EXPECT_EQ(pointerHashMap.size(), 1);
}

TEST(HashMap, Clear) {
    HashMap<int, int> hashMap(intHash);
    for(int i = 0; i< 10; ++i) {
        hashMap.put(i, i);
        hashMap.put(i, i*10);
    }
    EXPECT_EQ(hashMap.size(), 10);
    hashMap.clear();
    EXPECT_EQ(hashMap.size(), 0);
    for(int i = 0; i< 10; ++i) {
        hashMap.put(i+10, i+10);
        hashMap.put(i+10, i*10);
    }
    for (int i = 0; i < 10; i++) {
        EXPECT_THROW(hashMap.get(i), int);
        EXPECT_EQ(hashMap.get(i+10), i*10);
    }
}


