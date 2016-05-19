//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_RESULT_H
#define BUSCA_IMD_RESULT_H

#include "entry.h"

enum SortOrder {
    ALPHABETICAL,
    MATCH_COUNT,
    NATURAL
};

typedef struct Result {
    SortOrder sortOrder;
    bool showTime;
};

Result createResult(SortOrder sortOrder, bool showTime);

bool addEntry(Result result, Entry entry);

#endif //BUSCA_IMD_RESULT_H
