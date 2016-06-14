//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_INDEX_H
#define BUSCA_IMD_INDEX_H

/**
 * Somente um índice para todos os arquivos contentdo todas as palavas distintas de todos os arquivos.
 *
 * { #hash table
 *    palavra1 : { #hash table
 *          arquivo1: [ ->lista-> linhas],
 *          arquivo2: [ ->lista-> linhas]
 *    }
 *    ...
 * }
 *
 *
 */

#include <fstream>
#include <stdint.h>

#include "hash_map.h"
#include "list.h"
#include "short_string.h"

namespace busca_imd_index {

    struct SearchParams {
        bool exclusive;
        bool showTime;
        char order;
        busca_imd_core::List<busca_imd_core::ShortString> words;
    };

    //hashMap containing the ocurrences of the word (fileName, list of lines)
    typedef busca_imd_core::HashMap<busca_imd_core::ShortString *, busca_imd_core::List<int>*> FileHashMap;

    //hashMap containing the word index (word, FileHashMap)
    typedef busca_imd_core::HashMap<busca_imd_core::ShortString, FileHashMap*> WordHashMap;

    typedef busca_imd_core::HashMap<busca_imd_core::ShortString, busca_imd_core::List<int>*> SearchResult;

    class Index : public WordHashMap{

    private:
        typedef busca_imd_core::List<int> OccurrencesList;
        typedef busca_imd_core::ShortString Str;

        struct FilePathHolder{
            Str * filePathCopy;
            uint16_t usage;
        };

        Index();

        //get or create a file hash map for word
        FileHashMap * getOrCreateFileMap(Str &word);

        //get or create occurrences for word in file
        OccurrencesList * getOrCreateOccurrencesList(Str * filePath, Str &word);

        busca_imd_core::HashMap<Str *, FilePathHolder*> mFilePathUsage;

    public:

        // A singleton has neither copy constructor nor assignment operator
        Index(Index const&) = delete;
        void operator=(Index const&) = delete;

        // instance of this index
        static Index & getInstance();

        //after read the file and seeing if the file really needs to be added, then, we will add
        //the occurrences of that word in the hashMap
        void addEntry(Str & filePath, Str & word, int line, bool forceInsertion = false);


        // return a map
        // file->lines
        // some files may contain empty list of lines, ignore them
        SearchResult * search(SearchParams &params);

        //remove the occurrences of the word from the hashMap
        //return nullptr
        //internally call Index#remove(busca_imd_core::ShortString * const & word)
        void removeWord(Str const & word);


        // Iterate over all words removing files
        void removeFile(Str & filePath);

        //remove the occurrences of the word from the hashMap
        //return aways nullptr
        virtual FileHashMap* remove(Str const & word);
//
        virtual void clear();
        void release();

        friend std::ostream &operator<<( std::ostream &output,
                                          const busca_imd_index::Index &index );

        friend std::istream &operator>>( std::istream &input,
                                         busca_imd_index::Index &index );
    };
}


#endif //BUSCA_IMD_INDEX_H
