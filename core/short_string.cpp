//
// Created by Pedro Paulo on 31/05/2016.
//
#include <string.h>
#include <locale>
#include <malloc.h>
#include <iostream>
#include <nmmintrin.h>

#include "iterator.h"
#include "short_string.h"

namespace busca_imd_core {

    void ShortString::copy(const char *const & word) {
        mLength = (uint16_t) strlen(word);
        mValue = (char *) realloc(mValue, mLength * sizeof(char));
        strcpy(mValue, word);
    }

    void ShortString::copy(List<char> const & list) {
        mLength = (uint16_t) list.size();
        mValue = (char *) realloc(mValue, mLength * sizeof(char));
        int i = 0;
        for (char c : list) {
            mValue[i++] = c;
        }
    }

    void ShortString::copy(ShortString const & other) {
        if (this == &other)
            return;
        mLength = other.mLength;
        mValue = (char *) realloc(mValue, mLength * sizeof(char));
        strncpy(mValue, other.mValue, mLength);
    }

    ShortString::ShortString(): mValue(nullptr), mLength(0) {
        mLength = 0;
        mValue = nullptr;
    }

    ShortString::ShortString(char const * word) {
        mValue = nullptr;
        copy(word);
    }


    ShortString::ShortString(List<char> const & list) {
        mValue = nullptr;
        copy(list);
    }

    ShortString::ShortString(ShortString const & other) {
        mValue = nullptr;
        copy(other);
    }


    ShortString::~ShortString() {
        if (mValue != nullptr) {
            delete mValue;
            mValue = nullptr;
        }
    }

    ShortString &ShortString::operator=(const char *const word) {
        copy(word);
        return *this;
    }

    ShortString &ShortString::operator=(List<char> const & list) {
        copy(list);
        return *this;
    }

    ShortString &ShortString::operator=(ShortString const & other) {
        copy(other);
        return *this;
    }

    bool ShortString::operator==(const ShortString &ss) const {


        if (&ss == this) {
            return true;
        }


        if (ss.mLength != this->mLength) {
            return false;
        }

        for (int i = 0; i < mLength; i++) {
            if (ss.mValue[i] != mValue[i]) {
                return false;
            }
        }

        return true;
    }

    bool ShortString::operator==(const char *const other) const {
        return (strlen(other) == mLength) && (strncmp(mValue, other, mLength) == 0);
    }

    bool ShortString::operator!=(const ShortString &ss) const {
        return &ss != this && ((ss.mLength != this->mLength) || (strncmp(this->mValue, ss.mValue, mLength) != 0));
    }

    bool ShortString::operator!=(const char *const other) const {
        return (strlen(other) != mLength) || (strncmp(mValue, other, mLength) != 0);
    }

    const uint16_t ShortString::length() const {
        return mLength;
    }

    uint32_t ultraFastHash(ShortString *const & key) {
        return directUltraFastHash(*key);
    }

    uint32_t directUltraFastHash(ShortString const &key) {
        uint8_t * name = (uint8_t *) key.mValue;
        uint16_t len = key.mLength;
        uint32_t hash = 0;

#ifndef __SSE4_2__
        #define ROTL32(x,r) (x << r) | (x >> (32 - r))

    uint32_t k1;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    //----------
    // body
    for(;len >=4; len-=4, name+=4) {
        k1 = *(uint32_t *) name;
        k1 *= c1;
        k1 = ROTL32(k1,15);
        k1 *= c2;

        hash ^= k1;
        hash = ROTL32(hash,13);
        hash = hash * 5 + 0xe6546b64;
    }

    //----------
    // tail

    k1 = 0;

    switch(len & 3) {
        case 3:
            k1 ^= name[2] << 16;
        case 2:
            k1 ^= name[1] << 8;
        case 1:
            k1 ^= name[0];
            k1 *= c1;
            k1 = ROTL32(k1,15);
            k1 *= c2;
            hash ^= k1;
    };

    //----------
    // finalization
    hash ^= len;

    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
#else // __SSE4_2__
        //align first bytes
        for (; len > 0 && ((size_t)name & 0x7UL); len--, name++) {
            hash = _mm_crc32_u8(hash, *name);
        }
#ifndef __x86_64__
        //hash in 32bits systems
    for (;len >= 4; len-=4, name+=4) {
        hash = _mm_crc32_u32(hash, *(uint32_t *) name);
    }
#else // __x86_64__
        // hash in 64bits systems
        for (;len >= 8; len-=8, name+=8) {
            hash = (uint32_t) _mm_crc32_u64(hash, *(uint64_t *) name);
        }
#endif //__x86_64__
        //add last bytes
        for (; len > 0; len--, name++) {
            hash = _mm_crc32_u8(hash, *name);
        }
#endif //__SSE4_2__
        return hash;
    }

    char *ShortString::asCharArray() const {

        char * charToReturn = new char[mLength+1];

        strncpy(charToReturn, mValue, mLength);
        charToReturn[mLength] = '\0';
        return charToReturn;
    }

    int ShortString::compare(ShortString const &ss) const {
        if(this == &ss || mLength == ss.mLength && mLength == 0) return 0;
        if(mLength == 0) {
            return ss.mLength ? -1 : 0;
        }
        if(ss.mLength == 0) {
            return mLength;
        }

        char *thisValue = asCharArray(), *thatValue = ss.asCharArray();
        int value = strcoll(thisValue, thatValue);
        delete thisValue;
        delete thatValue;
        return value;
    }

    std::ostream & operator<<( std::ostream &output,
                               const busca_imd_core::ShortString &ss ) {
        output.write((char*) &ss.mLength, sizeof(uint16_t));
        if (ss.mLength) {
            output.write(ss.mValue, ss.mLength * sizeof(char));
        }
        return output;
    }

    std::istream & operator>>( std::istream &input,
                               busca_imd_core::ShortString &ss ) {

        input.read((char*) &ss.mLength, sizeof(uint16_t));
        if (ss.mLength > 0 ) {
            ss.mValue = (char *) realloc(ss.mValue, ss.mLength * sizeof(char));
            input.read(ss.mValue, ss.length());
        } else {
            delete ss.mValue;
            ss.mValue = nullptr;
        }

        return input;
    }
}






