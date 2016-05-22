//
// Created by gabriel on 5/17/16.
//

#include "result.h"

search::Result::Result(SortOrder order, bool st)
        : sortOrder(order), showTime(st) {

}

search::Result::Result(const search::Result &other)
        : sortOrder(other.sortOrder), showTime(other.showTime) {

}

search::Result::~Result() {

}

bool search::Result::addEntry(Entry entry) {
    return false;
}

search::Result &search::Result::operator<<(Entry entry) {
    addEntry(entry);
    return *this;
}
