//
// Created by Pedro Paulo on 31/05/2016.
//
#define UTF8_0BYTE 0x00
#define UTF8_1BYTE 0x80
#define UTF8_2BYTES 0xC0
#define UTF8_3BYTES 0xE0
#define UTF8_4BYTES 0xF0
#define UTF8_5BYTES 0xF8

#define SHORT_STRING_MIN_SIZE 16

#include <string.h>
#include <streambuf>
#include <ostream>
#include <istream>
#include <locale>
#include <iostream>
#include <nmmintrin.h>

#include "iterator.h"
#include "short_string.h"

namespace busca_imd_core {

    struct membuf : std::streambuf {
        membuf(char* begin, char* end) {
            this->setg(begin, begin, end);
        }
    };

    void roundUpToPowerOfTwo(uint16_t &i) {

//        std::cout << "change " << i ;


        if (i + 1 < SHORT_STRING_MIN_SIZE)
            i = SHORT_STRING_MIN_SIZE;

        // "Smear" the high-order bit all the way to the right.
        i |= i >>  1;
        i |= i >>  2;
        i |= i >>  4;
        i |= i >>  8;

        i++;

//        std::cout << " to " << i << std::endl;
    }

    void ShortString::copy(const char *const & word) {
        mLength = (uint16_t) strlen(word);
        if (!mLength) {
            delete [] mValue;
            mValue = nullptr;
            mAllocatedSize = 0;
            return;
        }
        uint16_t i = mLength;
        roundUpToPowerOfTwo(i);
        if (i != mAllocatedSize) {
            if (mValue) {
                delete[] mValue;
                mValue = nullptr;
            }
            mValue = new char[i];
        }
        mAllocatedSize = i;
        strcpy(mValue, word);
    }

    void ShortString::copy(List<char> const & list) {
        mLength = (uint16_t) list.size();
        if (!mLength) {
            delete [] mValue;
            mValue = nullptr;
            mAllocatedSize = 0;
            return;
        }
        uint16_t  i = mLength;
        roundUpToPowerOfTwo(i);
        if (i != mAllocatedSize) {
            if (mValue) {
                delete[] mValue;
                mValue = nullptr;
            }
            mValue = new char[i];
        }
        mAllocatedSize = i;
        i = 0;
        for (char c : list) {
            mValue[i++] = c;
        }
    }

    void ShortString::copy(ShortString const & other) {
        if (this == &other)
            return;
        mLength = other.mLength;
        if (!mLength) {
            delete [] mValue;
            mValue = nullptr;
            mAllocatedSize = 0;
            return;
        }
        uint16_t i = other.mAllocatedSize;
        roundUpToPowerOfTwo(i);
        if (i != mAllocatedSize) {
            if (mValue) {
                delete[] mValue;
                mValue = nullptr;
            }
            mValue = new char[i];
        }
        mAllocatedSize = i;
        strncpy(mValue, other.mValue, mLength);
    }

    ShortString::ShortString(): mValue(nullptr), mLength(0), mAllocatedSize(0) {
    }

    ShortString::ShortString(char const * word):mValue(nullptr), mLength(0), mAllocatedSize(0) {
        copy(word);
    }


    ShortString::ShortString(List<char> const & list): mValue(nullptr), mLength(0), mAllocatedSize(0) {
        copy(list);
    }

    ShortString::ShortString(ShortString const & other) : mValue(nullptr), mLength(0), mAllocatedSize(0) {
        copy(other);
    }


    ShortString::~ShortString() {
        if (mValue != nullptr) {
            delete[] mValue;
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

    char *ShortString::asCharArray(char * target) const {
        if (!target){
            target = new char[mAllocatedSize];
        }

        strncpy(target, mValue, mLength);
        target[mLength] = '\0';
        return target;
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
        delete[] thisValue;
        delete[] thatValue;
        return value;
    }

    ShortString ShortString::toUpperCase() const {
        ShortString s;
        uint32_t character;
        uint8_t charSize, i;
        char * bytes;
        uint16_t read = 0;
        membuf inputSrc(mValue, mValue + mLength);
        std::istream in(&inputSrc);

        s.mLength = mLength;
        s.mAllocatedSize = mAllocatedSize;
        s.mValue = new char[s.mAllocatedSize];

        while (read < mLength) {

            charSize = readUtf8Char(in, character);
            read+=charSize;
            if (isAlphaCharacter(character)) {
                busca_imd_core::toUpperCase(character);
            }

            bytes = (char *) & character;
            for (i = 0; i < charSize; i++) {
                s.mValue[read - charSize + i] = bytes[i];
            }
        }

        return s;
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

        uint16_t i = ss.mLength;

        if (ss.mLength > 0) {
            busca_imd_core::roundUpToPowerOfTwo(i);
            if (i != ss.mAllocatedSize) {
                if (ss.mValue) {
                    delete[] ss.mValue;
                    ss.mValue = nullptr;
                }
                ss.mValue = new char[i];
                ss.mAllocatedSize = i;
            }
            input.read(ss.mValue, ss.length());
        } else {
            delete[] ss.mValue;
            ss.mValue = nullptr;
        }

        return input;
    }

    uint8_t readUtf8Char(std::istream &input, uint32_t &dest) {
        uint8_t size = 0;
        dest = 0;
        char * letter = (char *) &dest;
        input.read(letter, 1);
        if ((*letter & UTF8_1BYTE) == UTF8_0BYTE) {
            size = 1;
        } else if ((*letter & UTF8_3BYTES) == UTF8_2BYTES) {
            size = 2;
        } else if ((*letter & UTF8_4BYTES) == UTF8_3BYTES) {
            size = 3;
        } else if ((*letter & UTF8_5BYTES) == UTF8_4BYTES) {
            size = 4;
        }

        for (int i = 1; i < size; i++) {
            input.read(&letter[i], 1);
        }

        return size;
    }

    void toUpperCase(uint32_t &a) {
        if ((a & 0xa0c3) == 0xa0c3 ) {
            a &= 0x9fc3;
        }
        if (a >= 'a' && a <= 'z') {
            a &= 0x5F;
        }
    }

    bool isAlphaCharacter(uint32_t a) {
        return (a & 0xc3) == 0xc3 || (a >= 'a' && a <= 'z') || (a >= 'A' && a <='Z');
    }

}






