//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_HASH_MAP_H
#define BUSCA_IMD_HASH_MAP_H

#include "array.h"

template<typename Key, typename Value>
typedef struct tpHashMap {
    Array<Key> keys;
    Array<Value> values;
    int size;
} * HashMap;

namespace HashMap {
    HashMap Create();
}

#endif //BUSCA_IMD_HASH_MAP_H
