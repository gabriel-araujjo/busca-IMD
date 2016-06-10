//
// Created by gabriel on 5/17/16.
//

#include "index.h"

using busca_imd_core::HashMap;
using busca_imd_core::List;
using busca_imd_core::ShortString;
using busca_imd_core::ultraFastHash;

namespace busca_imd_index {

    //Constructor of the Index class
    Index::Index() : WordHashMap(ultraFastHash) { }

    //add an entry to the index (path of the file(AKA:name of the file), word to add, line where this word appears)
    void Index::addEntry(ShortString *filePath, ShortString *word, int line, bool forceInsertion) {

        //get the hashMap containing all the ocurrences of that word on the index file;
        FileHashMap *fileHash = get(word);
        //list of the ocurrences of that word containing the lines where that word appears
        List<int> *listOfOcurrences;

        try {
            //try to see if the list is nullptr
            listOfOcurrences = fileHash->get(filePath);
        } catch (int e) {
            //if the list is null, them we will create one
            listOfOcurrences = new List<int>;
            fileHash->put(filePath, listOfOcurrences);
        }
        //if the list doesnt containt the occurence of that word in that line, then we will add this to the list,
        //otherwise, we dont need to add since the contains(line) will return true, indicating that this word appears
        //on the file in this line that we are trying to add
        if (forceInsertion || !listOfOcurrences->contains(line)) {
            listOfOcurrences->add(line, -1);
        }
    }

    //get a word of the index
    FileHashMap *Index::get(ShortString *const &word) {

        //create a hashMap
        FileHashMap *fileHash = nullptr;
        try {
            //try to see if
            fileHash = WordHashMap::get(word);
        } catch (int e) {
            fileHash = new FileHashMap(ultraFastHash);
            WordHashMap::put(word, fileHash);
        }

        return fileHash;
    }

    void Index::removeWord(ShortString *const &word) {
        remove(word);
    }

    void Index::removeFile(ShortString *const &filePath) {
        FileHashMap * fileHash;
        List<ShortString * const> wordsToRemove;
        for (WordHashMap::Entry entry :*this) {
            fileHash = entry.value;
            try {
                List<int> * occurences = fileHash->remove(filePath);
                delete occurences;
            } catch (int ignore) { }
            // Marking this word to remove
            if (!fileHash->size()) {
                wordsToRemove.add(entry.key);
            }
        }
        for (ShortString * const word : wordsToRemove) {
            removeWord(word);
        }
    }

    // remove a word of the index
    FileHashMap *Index::remove(ShortString *const &word) {
        FileHashMap *fileHash = WordHashMap::remove(word);
        for (FileHashMap::Entry entry : *fileHash) {
            delete entry.value;
        }
        delete fileHash;

        return nullptr;
    }

    Index &Index::getInstance() {
        static Index index;
        return index;
    }

    bool Index::equals(busca_imd_core::ShortString * const & a, busca_imd_core::ShortString *const & b) {
        return *a == *b;
    }

}

