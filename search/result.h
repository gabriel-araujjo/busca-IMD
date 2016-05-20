//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_RESULT_H
#define BUSCA_IMD_RESULT_H

#include "entry.h"

namespace search {

    class Result {

    public:

        static enum SortOrder { ALPHABETICAL, MATCH_COUNT, NATURAL};

    private:
        const SortOrder sortOrder;

        const bool showTime;

    public:

        Result(SortOrder sortOrder, bool showTime);
        Result(Result other);

        virtual ~Result();

        bool addEntry(Entry entry);

        Result & operator<< (Entry entry);
    };
}


#endif //BUSCA_IMD_RESULT_H
