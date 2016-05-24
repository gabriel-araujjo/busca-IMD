//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_ARRAY_H
#define BUSCA_IMD_ARRAY_H

namespace core {
    template <typename Element>
    class Array {
        Element * mData;
        unsigned int mSize;

    public:
        // Construtores e destrutores

        Array();
        Array(unsigned const int initialCapacity);
        Array(const Array &other);
        Array(const Element[]);
        ~Array();


        unsigned int size();

        void resize(unsigned int newSize);


        // Obtem ou adiciona um elemento a uma determinada posição no array
        // ou throw INDEX_OUT_OF_BOUND_EXCEPTION caso o índice for menor que zero ou maior que size - 1
        Element& operator[](unsigned int position);
        const Element& operator[](unsigned int position) const ;

    };

}


//Implementation

using core::Array;

template <typename Element>
Array<Element>::Array() {
    mData = nullptr;
    mSize = 0;
}

template <typename Element>
Array<Element>::Array(unsigned const int initialCapacity) {
    mData = new Element[initialCapacity];
    mSize = initialCapacity;
}

template <typename Element>
Array<Element>::Array(const Array<Element> &other) {
    mSize = other.mSize;
    mData = new Element[mSize];
    for (int i = 0; i <mSize; ++i) {
        mData[i] = other.mData[i];
    }
}

template <typename Element>
Array<Element>::~Array() {
    delete mData;
    mSize = 0;
}

template <typename Element>
unsigned int Array<Element>::size() {
    return mSize;
}

template <typename Element>
Array<Element>::Array(const Element *const elements) {
    mSize = (sizeof(elements) / sizeof(*elements));
    mData = new Element[mSize];
    for (int i = 0; i <mSize; ++i) {
        mData[i] = elements[i];
    }
}

template <typename Element>
Element &Array<Element>::operator[](unsigned int position) {
    return mData[position];
}

template <typename Element>
const Element &Array<Element>::operator[](unsigned int position) const {
    return mData[position];
}

template <typename Element>
void Array<Element>::resize(unsigned int newSize) {
    if (mData == nullptr)
        mData = new Element[newSize];
    else if (newSize)
        mData = (Element *) realloc(mData, newSize);
    else {
        delete mData;
        mData = nullptr;
    }

    mSize = newSize;
}

#endif //BUSCA_IMD_ARRAY_H
