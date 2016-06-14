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
#include "list.h"

namespace busca_imd_core {

    template <typename Key>
    bool equals(const Key & a, const Key & b) {
        return a == b;
    }

    // Specialization for pointers
    template <typename Key>
    bool equals(Key * const & a, Key *const & b) {
        return a == b || *a == *b;
    }

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

        class HashMapCursor : public Iterator<Entry>::Cursor {
            const HashMap<Key, Value> * mHashMap;
            unsigned int mCursor;
            ptrEntry mCurrentEntry;
        public:
            unsigned int mCoveredValues;
            /**
             * Constructors/destructor
             */
            HashMapCursor(const HashMap<Key, Value> * hashMap, bool end = false);
            HashMapCursor(HashMapCursor & other);

            virtual bool isNotEnd();
            virtual void next();
            virtual Entry &get();
            virtual bool samePosition(typename Iterator<Entry>::Cursor * const other);
        };

        Array<ptrEntry> * mEntries;
        unsigned int mSize;
        HashTp mCalcHash;
        Iterator<Entry> * mEndIterator;
        HashMapCursor * mEndCursor;

        ptrEntry getEntry(const Key & key, bool forceAddition = false);
        void resizeIfNecessary();
        unsigned int positionOfKey(const Key &key);

    public:
        HashMap(HashTp hashFunction);
        HashMap(const HashMap<Key, Value> & other);
        ~HashMap();

        bool put(Key key, Value value);
        virtual Value get(const Key &key);
        virtual Value remove(const Key &key);
        bool contains(const Key &key);
        int size() const ;
        virtual void clear();

        Iterator<Entry> begin() const;
        Iterator<Entry> & end() const;

        HashMap &operator=(const HashMap & other);
    };


    // Implementation

    template <typename Key, typename Value>
    HashMap<Key, Value>::HashMap(HashTp hashFunction) {
        mCalcHash = hashFunction;
        mEntries = new Array<ptrEntry>(HASH_MAP_ENTRIES_MIN_SIZE, nullptr);
        mSize = 0;
        mEndIterator = new Iterator<Entry>(mEndCursor = new HashMapCursor(this, true));
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
        mEndIterator = new Iterator<Entry>(mEndCursor = new HashMapCursor(this, true));
    }
    template <typename Key, typename Value>
    HashMap<Key, Value>::~HashMap() {
        delete mEndIterator;
        clear();
        delete mEntries;
    }


    template <typename Key, typename Value>
    bool HashMap<Key, Value>::put(Key key, Value value) {
        ptrEntry entry = getEntry(key, true);
        entry->entry.value = value;
        return true;
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

        if (equals(entry->entry.key, key)) {
            (*mEntries)[position] = entry->next;
            Value value = entry->entry.value;
            delete entry;
            mEndCursor->mCoveredValues = --mSize;
            resizeIfNecessary();
            return value;
        }

        ptrEntry prev;

        while (prev = entry, entry = entry->next) {
            if (equals(entry->entry.key, key)) {
                prev->next = entry->next;
                Value value = entry->entry.value;
                delete entry;
                mEndCursor->mCoveredValues = --mSize;
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
            if (equals(entry->entry.key, key))
                return entry;
            entry = entry->next;
        }
        if (forceAddition) {
            entry = new InternalEntry;
            entry->entry.key = key;
            entry->next = (*mEntries)[position];

            (*mEntries)[position] = entry;
            mEndCursor->mCoveredValues = ++mSize;
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

            clear();
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

    template <typename Key, typename Value>
    void HashMap<Key, Value>::clear() {
        ptrEntry entry;
        for (int i = 0; i < mEntries->size(); ++i) {
            while (entry = (*mEntries)[i]) {
                (*mEntries)[i] = entry->next;
                delete entry;
            }
        }
        mSize = 0;
    }

    template <typename Key, typename Value>
    Iterator<typename HashMap<Key, Value>::Entry> HashMap<Key, Value>::begin() const {
//        std::cout << "HM cursor mEntries = " << mEntries << std::endl;
        return Iterator<Entry>(new HashMapCursor(this));
    }

    template <typename Key, typename Value>
    Iterator<typename HashMap<Key, Value>::Entry>& HashMap<Key, Value>::end() const {
        return *mEndIterator;
    }

    // Cursor Implementation

    template <typename Key, typename Value>
    HashMap<Key, Value>::HashMapCursor::HashMapCursor(const HashMap<Key, Value> *hashMap, bool end) {
        mHashMap = hashMap;
        if (end) {
            mCoveredValues = mHashMap->mSize;
            return;
        }
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

            mCurrentEntry = mCurrentEntry->next;

            if (!mCurrentEntry) {
                mCursor++;
                for (; mCursor < mHashMap->mEntries->size(); mCursor++) {
                    while (mCurrentEntry = (*mHashMap->mEntries)[mCursor]) {

                        goto end;
                    }
                }
            }

            end:
            mCoveredValues++;
        }
    }

    template <typename Key, typename Value>
    typename HashMap<Key, Value>::Entry& HashMap<Key, Value>::HashMapCursor::get() {
        return mCurrentEntry->entry;
    }

    template <typename Key, typename Value>
    bool HashMap<Key, Value>::HashMapCursor::samePosition(typename Iterator<HashMap<Key, Value>::Entry>::Cursor * const other) {
        return mCoveredValues == ((HashMap<Key, Value>::HashMapCursor *)other)->mCoveredValues;
    };

}

#endif //BUSCA_IMD_HASH_MAP_H
