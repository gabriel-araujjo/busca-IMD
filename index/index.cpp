//
// Created by gabriel on 5/17/16.
//

#include <iosfwd>
#include <iostream>

#include "array.h"
#include "index.h"

using busca_imd_core::Array;
using busca_imd_core::HashMap;
using busca_imd_core::List;
using busca_imd_core::ShortString;
using busca_imd_core::ultraFastHash;

namespace busca_imd_index {

    //Constructor of the Index class
    Index::Index() : WordHashMap(ultraFastHash), mFilePathUsage(ultraFastHash) { }

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
            // increment fileUsages
            try {
                uint16_t usage = mFilePathUsage.get(filePath);
                usage++;
                mFilePathUsage.put(filePath, usage);
            } catch (int e) {
                mFilePathUsage.put(filePath, 1);
            }

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
        try {
            mFilePathUsage.remove(filePath);
            delete filePath;
        } catch (int ignore) { }
    }

    // remove a word of the index
    FileHashMap *Index::remove(ShortString *const &word) {
        FileHashMap *fileHash = WordHashMap::remove(word);
        for (FileHashMap::Entry entry : *fileHash) {
            delete entry.value;
            uint16_t usage = mFilePathUsage.get(entry.key);
            usage--;
            if (usage == 0) {
                mFilePathUsage.remove(entry.key);
                delete entry.key;
            }
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

    std::ostream  &operator<<( std::ostream &output,
                                      const busca_imd_index::Index &index ) {
        HashMap<ShortString *, uint16_t > filePathReverseLookup(ultraFastHash);

        // write all filePath lookup
        uint16_t filePathPosition = (uint16_t) index.mFilePathUsage.size();
        //write filePath lookup size
        output.write((char*) &filePathPosition, sizeof(uint16_t));
        for (auto entry : index.mFilePathUsage) {
            // write file path
            output << *entry.key;
            // track filePath into local lookup
            filePathReverseLookup.put(entry.key, --filePathPosition);
        }

        // write index
        uint16_t wordHashMapSize = (uint16_t) index.size();
        // write hashMap size
        output.write((char*) &wordHashMapSize, sizeof(uint16_t));
        for (auto entry : index) {
            // write word
            output << *entry.key;

            // write occurrences
            uint16_t occurrencesSize = (uint16_t) entry.value->size();
            uint16_t filePosition;
            uint16_t linesSize;
            // write occurrences size
            output.write((char*) &occurrencesSize, sizeof(uint16_t));
            for (auto fileEntry : *entry.value) {
                filePosition = filePathReverseLookup.get(fileEntry.key);
                // write filePth reference from filePathReverseLookup
                output.write((char*) &filePosition, sizeof(uint16_t));

                // writing lines
                linesSize = (uint16_t) fileEntry.value->size();
                output.write((char*) &linesSize, sizeof(uint16_t));
                for (auto line : *fileEntry.value) {
                    output.write((char*) &line, sizeof(uint16_t));
                }
            }
        }
        return output;
    }



    std::istream  &operator>>( std::istream &input,
                               busca_imd_index::Index &index ) {
        index.release();

        //Reading filePathLookup
        uint16_t filePathLookupSize;
        // reading file path lookup size
        input.read((char *) &filePathLookupSize, sizeof(uint16_t));

        Array<ShortString *> filePathLookup(filePathLookupSize);

        //reading file paths
        ShortString * filePath;
        for (;filePathLookupSize; --filePathLookupSize) {
            filePath = new ShortString();
            // reading file path
            input >> *filePath;
            // adding at begin of filePath
            filePathLookup[filePathLookupSize-1] = filePath;
            //creating entry in mFilePathUsage
            index.mFilePathUsage.put(filePath, 0);
        }

        // read index
        uint16_t wordHashMapSize;
        //reading word hash map size
        input.read((char *) &wordHashMapSize, sizeof(uint16_t));

        // reading words
        ShortString * word;
        busca_imd_index::FileHashMap * fileHashMap;
        for (;wordHashMapSize; --wordHashMapSize) {
            word = new ShortString();
            fileHashMap = new busca_imd_index::FileHashMap(ultraFastHash);
            input >> *word;
            index.put(word, fileHashMap);

            // read occurrences
            uint16_t occurrencesSize;
            uint16_t filePosition;
            uint16_t linesSize;
            int line = 0;
            List<int> * occurrences;
            ShortString * file;
            // reading occurrences size
            input.read((char *) &occurrencesSize, sizeof(uint16_t));
            for (;occurrencesSize; --occurrencesSize) {
                // reading file position in lookup
                input.read((char *) &filePosition, sizeof(uint16_t));
                occurrences = new List<int>;

                // reading occurrences
                // reading occurrences size
                input.read((char *) &linesSize, sizeof(uint16_t));
                for(;linesSize;--linesSize) {
                    input.read((char *) &line, sizeof(uint16_t));
                    occurrences->add(line);
                }
                file = filePathLookup[filePosition];
                fileHashMap->put(file, occurrences);
                index.mFilePathUsage.put(file, index.mFilePathUsage.get(file) + (uint16_t)1);
            }
        }

        return input;
    }

}

