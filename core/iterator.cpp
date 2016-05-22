//
// Created by gabriel on 5/19/16.
//

#include "iterator.h"

template <typename Element>
Element core::Iterator<Element>::operator*() {
    return nullptr;
}

template <typename Element>
core::Iterator<Element> &core::Iterator<Element>::operator++() {
    return *this;
}

template <typename Element>
bool core::Iterator<Element>::isEnd() {
    return false;
}
