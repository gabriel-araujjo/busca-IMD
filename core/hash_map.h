//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_HASH_MAP_H
#define BUSCA_IMD_HASH_MAP_H

#define HASH_MAP_KEY_NOT_FOUND_EXCEPTION 1

#define HASH_MAP_ENTRIES_MIN_SIZE 8
#define HASH_MAP_ENTRIES_MAX_SIZE 0x80000000
#define HASH_MAP_MAX_LOAD_FACTOR 0.75
#define HASH_MAP_MIN_LOAD_FACTOR 0.25

#include "array.h"
#include "iterator.h"

namespace core {

    template<typename Key, typename Value>
    class HashMap {
    public:
        typedef unsigned int (*HashTp) (const Key &);

        struct Entry {
            Key key;
            Value value;
        };

    private:

        typedef struct InternalEntry {
            Entry entry;
            InternalEntry * next;
        } * ptrEntry;

        class HashMapCursor : public Iterator<HashMap<Key, Value>::Entry>::Cursor {
            const HashMap<Key, Value> * mHashMap;
            unsigned int mCursor;
            unsigned int mCoveredValues;
            ptrEntry mCurrentEntry;
        public:
            /**
             * Constructors/destructor
             */
            HashMapCursor(const HashMap<Key, Value> * hashMap);
            HashMapCursor(HashMapCursor & other);

            virtual bool isNotEnd();
            virtual void next();
            virtual Entry &get();
        };

        Array<ptrEntry> * mEntries;
        int mSize;
        HashTp mCalcHash;

        ptrEntry getEntry(const Key & key, bool forceAddition = false);
        void resizeIfNecessary();
        unsigned int positionOfKey(const Key &key);

    public:
        HashMap(HashTp hashFunction);
        HashMap(const HashMap<Key, Value> & other);
        ~HashMap();

        bool put(Key key, Value value);
        Value get(const Key &key);
        Value remove(const Key &key);
        bool contains(const Key &key);
        int size() const ;

        // TODO: put this method outside
        Iterator<Entry> iterator() const {
            return Iterator<Entry>(new HashMapCursor(this));
        };

        HashMap &operator=(const HashMap & other);
    };


    template <typename Key, typename Value>
    HashMap<Key, Value>::HashMap(HashTp hashFunction) {
        mCalcHash = hashFunction;
        mEntries = new Array<ptrEntry>(HASH_MAP_ENTRIES_MIN_SIZE, nullptr);
        mSize = 0;
    }

    template <typename Key, typename Value>
    HashMap<Key, Value>::HashMap(const HashMap<Key, Value> &other) {
        mCalcHash = other.mCalcHash;
        mEntries = new Array<ptrEntry>(other.mEntries->size(), nullptr);
        for (int i = 0; i < mEntries->size(); ++i) {
            ptrEntry e = (*other.mEntries)[i];
            while (e) {
                ptrEntry entry = new InternalEntry();
                entry->entry.value = e->entry.value;
                entry->entry.key = e->entry.key;
                entry->next = (*mEntries)[i];

                (*mEntries)[i] = entry;
                e = e->next;
            }
        }
        mSize = other.mSize;


    }
    template <typename Key, typename Value>
    HashMap<Key, Value>::~HashMap() {
        ptrEntry entry;
        for (int i = 0; i < mEntries->size(); ++i) {
            while (entry = (*mEntries)[i]) {
                (*mEntries)[i] = entry->next;
                delete entry;
            }
        }
        delete mEntries;
    }




    template <typename Key, typename Value>
    bool HashMap<Key, Value>::put(Key key, Value value) {
        ptrEntry entry = getEntry(key, true);
        entry->entry.value = value;
    }

    template <typename Key, typename Value>
    Value HashMap<Key, Value>::get(const Key &key) {
        if (ptrEntry entry = getEntry(key)) {
            return entry->entry.value;
        };
        throw HASH_MAP_KEY_NOT_FOUND_EXCEPTION;
    }

    template <typename Key, typename Value>
    Value HashMap<Key, Value>::remove(const Key &key) {


        int position = positionOfKey(key);
        ptrEntry entry = (*mEntries)[position];


        if (!entry) {
            throw HASH_MAP_KEY_NOT_FOUND_EXCEPTION;
        }

        if (entry->entry.key == key) {
            (*mEntries)[position] = entry->next;
            Value value = entry->entry.value;
            delete entry;
            mSize--;
            resizeIfNecessary();
            return value;
        }

        ptrEntry prev;

        while (prev = entry, entry = entry->next) {
            if (entry->entry.key == key) {
                prev->next = entry->next;
                Value value = entry->entry.value;
                delete entry;
                mSize--;
                resizeIfNecessary();
                return value;
            }
        }
        throw HASH_MAP_KEY_NOT_FOUND_EXCEPTION;
    }

    template <typename Key, typename Value>
    bool HashMap<Key, Value>::contains(const Key &key) {
        return getEntry(key) != nullptr;
    }

    template <typename Key, typename Value>
    int HashMap<Key, Value>::size() const {
        return mSize;
    }

    template <typename Key, typename Value>
    unsigned int HashMap<Key, Value>::positionOfKey(const Key &key) {
        unsigned int hash = mCalcHash(key);
        return hash & (mEntries->size() -1);
    }

    template <typename Key, typename Value>
    typename HashMap<Key, Value>::ptrEntry HashMap<Key, Value>::getEntry(const Key &key, bool forceAddition) {
        unsigned int position = positionOfKey(key);
        ptrEntry entry = (*mEntries)[position];
        while (entry) {
            if (entry->entry.key == key)
                return entry;
            entry = entry->next;
        }
        if (!entry && forceAddition) {
            entry = new InternalEntry;
            entry->entry.key = key;
            entry->next = (*mEntries)[position];

            (*mEntries)[position] = entry;
            mSize++;
            resizeIfNecessary();
        }

        return entry;
    }

    template <typename Key, typename Value>
    void HashMap<Key, Value>::resizeIfNecessary() {

        float loadFactor = ((float) mSize) / mEntries->size();

        unsigned int newSize = 0;

        if ( loadFactor > HASH_MAP_MAX_LOAD_FACTOR && mEntries->size() < HASH_MAP_ENTRIES_MAX_SIZE) {
            newSize = mEntries->size() * 2;
        } else if (loadFactor < HASH_MAP_MIN_LOAD_FACTOR && mEntries->size() > HASH_MAP_ENTRIES_MIN_SIZE){
            newSize = mEntries->size() / 2;
        }

        if (newSize) {


            Array<ptrEntry> * oldEntries = mEntries;

            mEntries = new Array<ptrEntry>(newSize, nullptr);

            ptrEntry entry;
            for (int i = 0; i < oldEntries->size(); ++i) {
                while (entry = (*oldEntries)[i]) {
                    int position = positionOfKey(entry->entry.key);
                    (*oldEntries)[i] = entry->next;

                    entry->next = (*mEntries)[position];
                    (*mEntries)[position] = entry;
                }
            }



            delete oldEntries;
        }
    }

    template <typename Key, typename Value>
    HashMap<Key, Value>& HashMap<Key, Value>::operator=(const HashMap &other) {
        if (this != &other) {

            mCalcHash = other.mCalcHash;

            ptrEntry entry;
            for (int i = 0; i < mEntries->size(); ++i) {
                while (entry = (*mEntries)[i]) {
                    (*mEntries)[i] = entry->next;
                    delete entry;
                }
            }


            mEntries->resize(other.mEntries->size(), nullptr);


            for (int i = 0; i < mEntries->size(); ++i) {
                ptrEntry e = (*other.mEntries)[i];
                while (e) {

                    ptrEntry newEntry = new InternalEntry();
                    newEntry->entry.value = e->entry.value;
                    newEntry->entry.key = e->entry.key;
                    newEntry->next = (*mEntries)[i];

                    (*mEntries)[i] = newEntry;

                    e = e->next;
                }
            }
            mSize = other.mSize;
        }

        return *this;
    }

    // Cursor

    template <typename Key, typename Value>
    HashMap<Key, Value>::HashMapCursor::HashMapCursor(const HashMap<Key, Value> *hashMap) {
        mHashMap = hashMap;
        mCoveredValues = 0;

        for (mCursor = 0; mCursor < mHashMap->mEntries->size(); mCursor++) {
            while (mCurrentEntry = (*mHashMap->mEntries)[mCursor]) {
                return;
            }
        }

    }

    template <typename Key, typename Value>
    HashMap<Key, Value>::HashMapCursor::HashMapCursor(HashMap<Key, Value>::HashMapCursor & other) {
        mHashMap = other.mHashMap;
        mCursor = other.mCursor;
        mCoveredValues = other.mCoveredValues;
        mCurrentEntry = other.mCurrentEntry;
    }

    template <typename Key, typename Value>
    bool HashMap<Key, Value>::HashMapCursor::isNotEnd() {
        return mCoveredValues < mHashMap->mSize;
    }

    template <typename Key, typename Value>
    void HashMap<Key, Value>::HashMapCursor::next() {
        if (mCoveredValues < mHashMap->mSize) {
            if (mCurrentEntry) {
                mCurrentEntry = mCurrentEntry->next;
                mCursor++;
            }


            if (!mCurrentEntry) {
                for (; mCursor < mHashMap->mEntries->size(); mCursor++) {
                    while (mCurrentEntry = (*mHashMap->mEntries)[mCursor]) {

                        goto end;
                    }
                }
            }

            end:
            mCoveredValues++;
        }
        return;
    }

    template <typename Key, typename Value>
    typename HashMap<Key, Value>::Entry& HashMap<Key, Value>::HashMapCursor::get() {
        return mCurrentEntry->entry;
    }

}

#endif //BUSCA_IMD_HASH_MAP_H
