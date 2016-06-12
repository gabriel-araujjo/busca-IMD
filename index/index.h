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

    //hashMap containing the ocurrences of the word (fileName, list of lines)
    typedef busca_imd_core::HashMap<busca_imd_core::ShortString *, busca_imd_core::List<int>*> FileHashMap;

    //hashMap containing the word index (word, FileHashMap)
    typedef busca_imd_core::HashMap<busca_imd_core::ShortString *, FileHashMap*> WordHashMap;

    class Index : public WordHashMap{

    private:
        typedef busca_imd_core::List<int> OccurrencesList;
        typedef busca_imd_core::ShortString Str;
        Index();

        //get or create a file hash map for word
        FileHashMap * getOrCreateFileMap(Str *const &word);

        //get or create occurrences for word in file
        OccurrencesList * getOrCreateOccurrencesList(Str * filePath, Str *const &word);

        busca_imd_core::HashMap<Str *, uint16_t> mFilePathUsage;

    public:

        // A singleton has neither copy constructor nor assignment operator
        Index(Index const&) = delete;
        void operator=(Index const&) = delete;

        // instance of this index
        static Index & getInstance();

        //after read the file and seeing if the file really needs to be added, then, we will add
        //the occurrences of that word in the hashMap
        void addEntry(Str * filePath, Str * word, int line, bool forceInsertion = false);


        //remove the occurrences of the word from the hashMap
        //return nullptr
        //internally call Index#remove(busca_imd_core::ShortString * const & word)
        void removeWord(Str * const & word);


        // Iterate over all words removing files
        void removeFile(Str * const & filePath);

        //remove the occurrences of the word from the hashMap
        //return aways nullptr
        virtual FileHashMap* remove(Str * const & word);
//
        void release();

        friend std::ostream &operator<<( std::ostream &output,
                                          const busca_imd_index::Index &index );

        friend std::istream &operator>>( std::istream &input,
                                         busca_imd_index::Index &index );
    };
}


#endif //BUSCA_IMD_INDEX_H
