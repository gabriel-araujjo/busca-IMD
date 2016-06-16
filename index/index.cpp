//
// Created by gabriel on 5/17/16.
//

#include <iosfwd>
#include <iostream>
#include <cassert>

#include "array.h"
#include "index.h"

using busca_imd_core::Array;
using busca_imd_core::HashMap;
using busca_imd_core::List;
using busca_imd_core::ShortString;
using busca_imd_core::ultraFastHash;
using busca_imd_core::directUltraFastHash;

namespace busca_imd_index {

    int int_comparator(const int & a, const int & b) {
        return a - b;
    }

    //Constructor of the Index class
    Index::Index() : WordHashMap(directUltraFastHash), mFilePathUsage(ultraFastHash) { }

    //add an entry to the index (path of the file(AKA:name of the file), word to add, line where this word appears)
    void Index::addEntry(ShortString &filePath, ShortString &word, int line, bool forceInsertion) {

//        std::cout << "adding entry " << filePath << " -> " << word << " " << line << std::endl;

        //list of the occurrences of word in in file
        List<int> *occurrencesList = getOrCreateOccurrencesList(&filePath, word);


        //if the list doesn't contain the line, we add it at begin
        if (forceInsertion || !occurrencesList->contains(line)) {
            occurrencesList->add(line, -1);
        }
    }

    SearchResult * Index::search(SearchParams &params) {
        SearchResult * results = new SearchResult(busca_imd_core::directUltraFastHash);

        for (auto &&word : params.words) {
            FileHashMap * index;
            try {
                index = get(word);
            } catch (int e) {
                if (params.exclusive) {
                    //exclusive behavior, ia word is out there is no results
                    for (auto &&entry : *results) {
                        delete entry.value;
                    }
                    delete results;
                    return nullptr;
                } else {
                    continue;
                }
            }

            for (FileHashMap::Entry entry : *index) {
                List<int> * list;
                try {
                    list = results->get(*entry.key);
                } catch (int e) {
                    results->put(*entry.key, new List<int>(*entry.value));
                    continue;
                }
                if (params.exclusive) {
                    list->intersection(*entry.value, int_comparator);
                } else {
                    list->join(*entry.value, int_comparator);
                }
            }
        }
        return results;
    }

    List<int> *Index::getOrCreateOccurrencesList(ShortString * filePath, ShortString &word) {
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


            // increment fileUsages
            FilePathHolder * holder;
            try {
                holder = mFilePathUsage.get(filePath);
                holder->usage++;
            } catch (int e) {
                ShortString * filePathCopy = new ShortString(*filePath);
                holder = new FilePathHolder;
                holder->filePathCopy = filePathCopy;
                holder->usage = 1;
                mFilePathUsage.put(filePathCopy, holder);
            }

            fileHash->put(holder->filePathCopy, occurrencesList);

        }
        return occurrencesList;
    }

    //get or crete index for word
    FileHashMap *Index::getOrCreateFileMap(ShortString &word) {

        //create a hashMap
        FileHashMap *fileHash;
        try {
            //try to see if
            fileHash = get(word);
        } catch (int e) {
            fileHash = new FileHashMap(ultraFastHash);
            put(word, fileHash);
        }

        return fileHash;
    }

    void Index::removeWord(ShortString const &word) {


        FileHashMap *fileHash = WordHashMap::remove(word);
        for (FileHashMap::Entry entry : *fileHash) {
            // delele occurrences
            delete entry.value;
            entry.value = nullptr;
            //check file usage

            FilePathHolder * holder;
            holder = mFilePathUsage.get(entry.key);
            holder->usage--;
            if (holder->usage == 0) {
                mFilePathUsage.remove(entry.key);
                delete holder->filePathCopy;
                delete holder;
            }
        }
        delete fileHash;
    }

    void Index::removeFile(ShortString &filePath) {
        FileHashMap * fileHash;
        List<ShortString const> wordsToRemove;
        for (WordHashMap::Entry entry :*this) {
            fileHash = entry.value;
            try {
                List<int> * occurrences = fileHash->remove(&filePath);
                delete occurrences;
            } catch (int ignore) { }
            // Marking this word to remove
            if (!fileHash->size()) {
                wordsToRemove.add(entry.key);
            }
        }
        for (ShortString const word : wordsToRemove) {
            removeWord(word);
        }
        try {
            FilePathHolder * holder = mFilePathUsage.remove(&filePath);
            delete holder->filePathCopy;
            delete holder;
        } catch (int ignore) { }
    }

    // remove a word of the index
    FileHashMap *Index::remove(ShortString const &word) {
        removeWord(word);
        return nullptr;
    }

    Index &Index::getInstance() {
        static Index index;
        return index;
    }

    void Index::clear() {
        release();
    }

    void Index::release() {
        if (!size()) return;
        List<ShortString> words;
        for (auto &&entry : *this) {
            words.add(entry.key);
        }

        for (auto &&word: words) {
            removeWord(word);
        }
    }

    std::ostream  &operator<<( std::ostream &output,
                                      const busca_imd_index::Index &index ) {
        HashMap<ShortString *, uint16_t > filePathReverseLookup(ultraFastHash);

//        std::cout << std::endl;
//        std::cout << "WRITING" << std::endl;

        // write all filePath lookup
        uint16_t filePathPosition = (uint16_t) index.mFilePathUsage.size();
        //write filePath lookup size
//        std::cout << "Lookup Size " << filePathPosition << std::endl;
        output.write((char*) &filePathPosition, sizeof(uint16_t));
        for (auto &&entry : index.mFilePathUsage) {
            // write file path
//            std::cout << *entry.key << " -> " << (filePathPosition - 1) << std::endl;
            output << *entry.key;
            // track filePath into local lookup
            filePathReverseLookup.put(entry.key, --filePathPosition);
        }

//        std::cout << std::endl;
        // write index
        uint16_t wordHashMapSize = (uint16_t) index.size();
        // write hashMap size
//        std::cout << "HashMap Size " << wordHashMapSize << std::endl;
        output.write((char*) &wordHashMapSize, sizeof(uint16_t));
        for (auto &&entry : index) {
            // write word
//            std::cout << "word " << entry.key << std::endl;
            output << entry.key;

            // write occurrences
            uint16_t occurrencesSize = (uint16_t) entry.value->size();
            uint16_t filePosition;
            uint16_t linesSize;
            // write occurrences size
//            std::cout << "\toccurrences size " << occurrencesSize << std::endl;
            output.write((char*) &occurrencesSize, sizeof(uint16_t));
            for (auto &&fileEntry : *entry.value) {
                filePosition = filePathReverseLookup.get(fileEntry.key);
                // write filePth reference from filePathReverseLookup
//                std::cout << "\tfile " << filePosition << " " << *fileEntry.key << std::endl;
                output.write((char*) &filePosition, sizeof(uint16_t));

                // writing lines
                linesSize = (uint16_t) fileEntry.value->size();
//                std::cout << "\tlinhas (" << linesSize << ")";
                output.write((char*) &linesSize, sizeof(uint16_t));
                for (auto &&line : *fileEntry.value) {
//                    std::cout << " " << line;
                    output.write((char*) &line, sizeof(uint16_t));
                }
//                std::cout << std::endl;
            }
        }
//        std::cout << std::endl;
        return output;
    }



    std::istream  &operator>>( std::istream &input,
                               busca_imd_index::Index &index ) {
        index.release();

//        std::cout << std::endl;
//        std::cout << "READING" << std::endl;

        //Reading filePathLookup
        uint16_t filePathLookupSize;
        // reading file path lookup size
        input.read((char *) &filePathLookupSize, sizeof(uint16_t));

        assert(filePathLookupSize > 0);

//        std::cout << "Lookup Size " << filePathLookupSize << std::endl;

        Array<ShortString *> filePathLookup(filePathLookupSize);

        //reading file paths
        ShortString * filePath;
        for (;filePathLookupSize; --filePathLookupSize) {
            filePath = new ShortString();
            filePathLookup[filePathLookupSize -1] = filePath;
            // reading file path
            input >> *filePath;
//            std::cout << *filePathLookup[filePathLookupSize -1] << " -> " << (filePathLookupSize -1) << std::endl;
            busca_imd_index::Index::FilePathHolder * holder = new busca_imd_index::Index::FilePathHolder;
            holder->filePathCopy = filePath;
            holder->usage = 0;
            index.mFilePathUsage.put(filePath, holder);
        }

//        std::cout << std::endl;

        // read index
        uint16_t wordHashMapSize;
        //reading word hash map size
        input.read((char *) &wordHashMapSize, sizeof(uint16_t));

//        std::cout << "HashMap Size " << wordHashMapSize << std::endl;
        // reading words
        ShortString word;
        busca_imd_index::FileHashMap * fileHashMap;
        for (;wordHashMapSize; --wordHashMapSize) {
            fileHashMap = new busca_imd_index::FileHashMap(ultraFastHash);
            input >> word;
//            std::cout << "word " << word << std::endl;
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
//            std::cout << "\toccurrences size " << occurrencesSize << std::endl;
            for (;occurrencesSize; --occurrencesSize) {
                // reading file position in lookup
                input.read((char *) &filePosition, sizeof(uint16_t));
                assert(filePosition < filePathLookup.size());

//                std::cout << "\tfile " << filePosition << " " << *filePathLookup[filePosition] << std::endl;
                occurrences = new List<int>;

                // reading occurrences
                // reading occurrences size
                input.read((char *) &linesSize, sizeof(uint16_t));
//                std::cout << "\tlinhas (" << linesSize << ")";
                for(;linesSize;--linesSize) {
                    input.read((char *) &line, sizeof(uint16_t));
//                    std::cout << " " << line;
                    occurrences->add(line);
                }
//                std::cout << std::endl;
                file = filePathLookup[filePosition];
                fileHashMap->put(file, occurrences);
                index.mFilePathUsage.get(file)->usage++;
            }
        }
//        std::cout << std::endl;
        return input;
    }

}

