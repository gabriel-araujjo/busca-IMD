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


        //list of the occurrences of word in in file
        List<int> *occurrencesList = getOrCreateOccurrencesList(filePath, word);


        //if the list doesn't contain the line, we add it
        if (forceInsertion || !occurrencesList->contains(line)) {
            occurrencesList->add(line, -1);
        }
    }

    List<int> *Index::getOrCreateOccurrencesList(ShortString *filePath, ShortString *const &word) {
        //get the hashMap containing all the occurrences of that word on the index file;
        FileHashMap *fileHash = getOrCreateFileMap(word);
        //list of the occurrences of the word in file
        List<int> *occurrencesList;

        try {
            //try to see if the list is nullptr
            occurrencesList = fileHash->get(filePath);
        } catch (int e) {
            //if the list is null, them we will create one
            occurrencesList = new List<int>;
            fileHash->put(filePath, occurrencesList);
        }
        return occurrencesList;
    }

    //get or crete index for word
    FileHashMap *Index::getOrCreateFileMap(ShortString *const &word) {

        //create a hashMap
        FileHashMap *fileHash = nullptr;
        try {
            //try to see if
            fileHash = get(word);
        } catch (int e) {
            fileHash = new FileHashMap(ultraFastHash);
            put(word, fileHash);
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
                List<int> * occurrences = fileHash->remove(filePath);
                delete occurrences;
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

    void Index::release() {
        List<ShortString*> words;
        for (auto entry : *this) {
            words.add(entry.key);
        }

        for (auto word: words) {
            removeWord(word);
        }
    }


}

