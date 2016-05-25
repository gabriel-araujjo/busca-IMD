//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_HASH_MAP_H
#define BUSCA_IMD_HASH_MAP_H

#include "array.h"
#include "iterator.h"

namespace core {

    template<typename Key, typename Value>
    class HashMap {
    public:
        typedef struct Entry {
            Key * key;
            Value * value;
            int frequency;
            Entry * next;
        } * ptrEntry;

    private:
        Array<ptrEntry> mEntries;
        int mSize;

        void resizeIfNecessary();

    public:
        HashMap();
        HashMap(const HashMap & other);
        ~HashMap();

        bool put(Key &key, Value &value);
        bool get(Key &key);
        Value remove(Key &key);
        int size();

        Iterator<Entry> iterator();

    };
}

#endif //BUSCA_IMD_HASH_MAP_H
