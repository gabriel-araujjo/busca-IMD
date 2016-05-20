//
// Created by gabriel on 5/19/16.
//

#include "iterator.h"

template <typename Element>
Element core::Iterator<Element>::operator*() {
    return nullptr;
}

template <typename Element>
Iterator &core::Iterator<Element>::operator++() {
    return *this;
}

bool core::Iterator::isEnd() {
    return false;
}
