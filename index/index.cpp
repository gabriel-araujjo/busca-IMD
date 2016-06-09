//
// Created by gabriel on 5/17/16.
//

#include "index.h"

using busca_imd_core::HashMap;
using busca_imd_core::List;
using busca_imd_core::ShortString;
using busca_imd_core::ultraFastHash;

//Constructor of the Index class
busca_imd_index::Index::Index() : busca_imd_index::WordHashMap(ultraFastHash) {

}

//add an entry to the index (path of the file(AKA:name of the file), word to add, line where this word appears)
void busca_imd_index::Index::addEntry(ShortString * filePath, ShortString * word, int line, bool forceInsertion) {

    //get the hashMap containing all the ocurrences of that word on the index file;
    busca_imd_index::FileHashMap* fileHash = get(word);
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
busca_imd_index::FileHashMap* busca_imd_index::Index::get(ShortString * const &key) {

    //create a hashMap
    busca_imd_index::FileHashMap* fileHash = nullptr;
    try {
        //try to see if
        fileHash = busca_imd_index::WordHashMap::get(key);
    } catch (int e){
        fileHash = new busca_imd_index::FileHashMap(ultraFastHash);
        put(key, fileHash);
    }

    return fileHash;
}

busca_imd_index::FileHashMap *busca_imd_index::Index::remove(ShortString * const &key) {
    busca_imd_index::FileHashMap* fileHash = HashMap::remove(key);


    //iterating through the lists at the hash
    for(busca_imd_index::FileHashMap::Entry entry : *fileHash){
        //deleting the list
        delete fileHash->remove(entry.key);
    }

    delete fileHash;

    return nullptr;
}