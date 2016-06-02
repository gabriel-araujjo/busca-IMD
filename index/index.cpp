//
// Created by gabriel on 5/17/16.
//

#include <nmmintrin.h>
#include <iostream>
#include <sys/types.h>

#include "index.h"

unsigned int hash(unsigned char * name, uint16_t len) {
    uint32_t hash = 0;

#ifndef __SSE4_2__

#else
    //align first bytes
    for (; len > 0 && ((size_t)name & 0x7UL); len--, name++) {
        hash = _mm_crc32_u8(hash, *name);
    }
#ifndef __x86_64__
    //hash in 32bits systems
    for (;len >= 4; len-=4, name+=4) {
        hash = _mm_crc32_u32(hash, *(uint32_t *) name);
    }
#else
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
}