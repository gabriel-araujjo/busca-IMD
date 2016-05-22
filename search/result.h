//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_RESULT_H
#define BUSCA_IMD_RESULT_H

#include "entry.h"



namespace search {

    typedef enum SortOrder{ ALPHABETICAL, MATCH_COUNT, NATURAL} ;

    class Result {
        
    private:
        const SortOrder sortOrder;

        const bool showTime;

    public:

        Result(SortOrder sortOrder, bool showTime);
        Result(const Result &other);

        virtual ~Result();

        bool addEntry(Entry entry);

        Result & operator<< (Entry entry);
    };
}


#endif //BUSCA_IMD_RESULT_H
