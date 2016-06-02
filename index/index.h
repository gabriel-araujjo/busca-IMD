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

namespace index {

    //hashMap containing the ocurrences of the word (fileName, list of lines)
    typedef core::HashMap<core::ShortString *, core::List<int>*> FileHashMap;
    //hashMap containing the word index (word, FileHashMap)
    typedef core::HashMap<core::ShortString *, FileHashMap*> WordHashMap;

    class Index : public WordHashMap{

    private:
    public:
        Index();

        //after read the file and seeing if the file realy needs to be added, then, we will add
        //the ocurrences of that word in the hashMap
        void addEntry(core::ShortString * filePath, core::ShortString * word, int line, bool forceInsertion = false);

        //remove the ocurrences of the word from the hashMap
        //it will return all the files that have that word
        FileHashMap* remove(core::ShortString * const & key);

        //get the ocurrences of the word from the hashMap
        //it will return all the files that have that word
        FileHashMap* get(core::ShortString * const & key);
    };
}


#endif //BUSCA_IMD_INDEX_H
