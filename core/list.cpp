//
// Created by gabriel on 5/19/16.
//

#include "list.h"

using namespace core;

/**
 * Construtor
 */
template <typename Element>
List<Element>::List() {
    init();
}


/**
 * Construtor de copia
 */
template <typename Element>
List<Element>::List(List<Element> &other) {
    init();
    for(Iterator<Element> iterator = other.iterator(); !iterator.isEnd(); ++iterator) {
        add(*iterator);
    }
}

/**
 * Destrutor
 */
template <typename Element>
List<Element>::~List() {
    Node cursor, markedForDeathNode;
    for (markedForDeathNode = mFirst, cursor = mFirst->next; cursor != mLast; markedForDeathNode = cursor, cursor = cursor->next) {
        delete markedForDeathNode->element;
        delete markedForDeathNode;
    }

    delete markedForDeathNode->element;
    delete markedForDeathNode;
}


/**
 * Inicializador das sentnelas e do tamanho
 */
template <typename Element>
void List<Element>::init() {
    mFirst = new struct tpNode;
    mLast = new struct tpNode;
    mFirst->next = mLast;
    mLast->prev = mFirst;
    mSize = 0;
}

/**
 * Obtem o iterador da lista
 */
template <typename Element>
Iterator<Element> List<Element>::iterator() {
    return List::ListIterator(*this);
}


template <typename Element>
bool List<Element>::add(Element element, int index) {
    Node cursor = getNodeAt(index);

    if (!cursor)
        return false;

    Node node = new tpNode;

    // Checagem neurótica para saber se conseguiu criar o nó
    if (!node)
        return false;

    node->element = new Element(element);


    // Checagem neurótica para saber se conseguiu criar o elemento
    if (!node->element) {

        // Deleta o nó alocado para evitar memory leak
        delete node;
        return false;
    }

    // Ajusta os links entre os nós
    node->next = cursor;
    node->prev = cursor->prev;
    cursor->prev = node;
    node->prev->next = node;

    // Incrementa o tamanho da lista
    mSize++;
    return true;
}

template <typename Element>
int List<Element>::size() {
    return mSize;
}

template <typename Element>
Element List<Element>::get(int index) {
    Node node = getNodeAt(index);
    if (!node) throw INDEX_OF_BOUND_EXCEPTION;
    return *(node->element);
}

template <typename Element>
typename List<Element>::Node List<Element>::getNodeAt(int index) {
    Node cursor;

    // retorne se o indice for inválido
    if (index < -mSize || index > mSize) return nullptr;

    // Permite inserir elementos em índices negativos, -1 adiciona no final, -2 adiciona antes do último elemento
    // e assim sucessivamente
    if (index < 0) index = mSize + 1 - index;

    if (index >= mSize / 2) {
        // caso em que o índice está na segunda metade

        cursor = mLast;
        for(int pos = mSize; pos > index; pos--) {
            cursor = cursor->prev;
        };
    } else {

        // caso em que o índice está na primeira metade
        cursor = mFirst->next;
        for (int pos = 0; pos < index; pos++) {
            cursor = cursor->next;
        }
    }
    return cursor;
}

template <typename Element>
bool List<Element>::remove(Element element) {
    // Iterar do início até o final
    for (Node cursor = mFirst->next; cursor != mLast; cursor = cursor->next) {
        if (element == *(cursor->element)) {
            // Encontrou o elemento
            // deleta o ponteiro para o elemento
            delete cursor->element;


            //Ajusta os links entre os nós
            cursor->next->prev = cursor->prev;
            cursor->prev->next = cursor->next;

            // deleta o cursor
            delete cursor;

            // decrementa o tamanho da lista
            mSize--;
            return true;
        }
    };
    return false;
}




/******************************************************************
 * ITERADOR DE LISTA
 ******************************************************************/
template <typename Element>
List<Element>::ListIterator::ListIterator(const List &list) : list(list) {
    cursor = list.mFirst;
}

template <typename Element>
List<Element>::ListIterator::ListIterator(List::ListIterator &other)
        : list(other.list), cursor(other.cursor) {

}

template <typename Element>
bool List<Element>::ListIterator::isEnd() {
    return cursor == list.mLast;
}

template <typename Element>
typename List<Element>::ListIterator &List<Element>::ListIterator::operator++() {
    if (!isEnd()) cursor = cursor->next;
    return *this;
}

template <typename Element>
Element & List<Element>::ListIterator::operator*() {
    return *(cursor->element);
}