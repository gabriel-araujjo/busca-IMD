//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_SEARCH_H
#define BUSCA_IMD_SEARCH_H

#include "result.h"

//using search::Result::SortOrder;

namespace search {

    /**
     * This class deliveries a result, given a set of words
     */
    class Search {

    public:
        Result searchAnd(const SortOrder order, const bool showTime, char ** words);
        Result searchOr(const SortOrder order, const bool showTime, char ** words);
    };
}

#endif //BUSCA_IMD_SEARCH_H
