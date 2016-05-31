//
// Created by gabriel on 5/17/16.
//

#include "index.h"
#include "iterator.h"

using core::HashMap;
using core::List;
using core::Iterator;

//just for test
unsigned int HASH(const short_string & key){
    return 0;
}

//Constructor of the Index class
index::Index::Index() : WordHashMap(HASH) {

}

//add an entry to the index (path of the file(AKA:name of the file), word to add, line where this word appears)
void index::Index::addEntry(short_string filePath, short_string word, int line, bool forceInsertion) {

    //get the hashMap containing all the ocurrences of that word on the index file;
    FileHashMap* fileHash = get(word);
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
FileHashMap* index::Index::get(const short_string &key) {

    //create a hashMap
    FileHashMap* fileHash = nullptr;
    try {
        //try to see if
        fileHash = WordHashMap::get(key);
    } catch (int e){
        fileHash = new FileHashMap(HASH);
        put(key, fileHash);
    }

    return fileHash;
}

FileHashMap *index::Index::remove(const short_string &key) {
    FileHashMap* fileHash = HashMap::remove(key);

    Iterator<FileHashMap::Entry> iter = fileHash->iterator();

    //iterating through the lists at the hash
    for(;iter.isNotEnd();++iter){
        //deleting the list
        delete fileHash->remove((*iter).key);
    }

    delete fileHash;

    return nullptr;
}
