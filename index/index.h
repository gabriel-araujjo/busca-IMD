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
        Index();

    protected:
        virtual bool equals(busca_imd_core::ShortString *const & a, busca_imd_core::ShortString *const & b);
    public:

        Index(Index const&) = delete;
        void operator=(Index const&) = delete;

        // instance of this index
        static Index & getInstance();

        //after read the file and seeing if the file really needs to be added, then, we will add
        //the occurrences of that word in the hashMap
        void addEntry(busca_imd_core::ShortString * filePath, busca_imd_core::ShortString * word, int line, bool forceInsertion = false);


        //remove the occurrences of the word from the hashMap
        //return nullptr
        //internally call Index#remove(busca_imd_core::ShortString * const & word)
        void removeWord(busca_imd_core::ShortString * const & word);


        // Iterate over all words removing files
        void removeFile(busca_imd_core::ShortString * const & filePath);

        //remove the occurrences of the word from the hashMap
        //return nullptr
        FileHashMap* remove(busca_imd_core::ShortString * const & word);

        //get the occurrences of the word from the hashMap
        //it will return all the files that have that word
        FileHashMap* get(busca_imd_core::ShortString * const & word);

        void release();
    };
}


#endif //BUSCA_IMD_INDEX_H
