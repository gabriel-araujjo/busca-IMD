//
// Created by gabriel on 5/17/16.
//

#include <nmmintrin.h>
#include <iostream>
#include <sys/types.h>

#include "index.h"
#include "iterator.h"

using core::HashMap;
using core::List;
using core::Iterator;
using core::ShortString;

//just for test
unsigned int HASH( ShortString * const & key);

//Constructor of the Index class
index::Index::Index() : index::WordHashMap(HASH) {

}

//add an entry to the index (path of the file(AKA:name of the file), word to add, line where this word appears)
void index::Index::addEntry(ShortString * filePath, ShortString * word, int line, bool forceInsertion) {

    //get the hashMap containing all the ocurrences of that word on the index file;
    index::FileHashMap* fileHash = get(word);
    //list of the ocurrences of that word containing the lines where that word appears
    List<int>* listOfOcurrences;

    try {
        //try to see if the list is nullptr
        listOfOcurrences = fileHash->get(filePath);
    } catch (int e) {
        //if the list is null, them we will create one
        listOfOcurrences = new List<int>;
    }
    //if the list doesnt containt the occurence of that word in that line, then we will add this to the list,
    //otherwise, we dont need to add since the contains(line) will return true, indicating that this word appears
    //on the file in this line that we are trying to add
    if(forceInsertion || !listOfOcurrences->contains(line)) {
        listOfOcurrences->add(line);
    }
}

//key is the filePath
index::FileHashMap* index::Index::get(ShortString * const &key) {

    //create a hashMap
    index::FileHashMap* fileHash = nullptr;
    try {
        //try to see if
        fileHash = index::WordHashMap::get(key);
    } catch (int e){
        fileHash = new index::FileHashMap(HASH);
        put(key, fileHash);
    }

    return fileHash;
}

index::FileHashMap *index::Index::remove(ShortString * const &key) {
    index::FileHashMap* fileHash = HashMap::remove(key);

    Iterator<index::FileHashMap::Entry> iter = fileHash->iterator();

    //iterating through the lists at the hash
    for(;iter.isNotEnd();++iter){
        //deleting the list
        delete fileHash->remove((*iter).key);
    }

    delete fileHash;

    return nullptr;
}

unsigned int HASH(ShortString * const & key) {
    char * name = key.value;
    uint16_t len = key.length;
    uint32_t hash = 0;

#ifndef __SSE4_2__
    //TODO: Implement murmur
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
    return hash;
}