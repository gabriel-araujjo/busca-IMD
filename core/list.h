//
// Created by gabriel on 5/19/16.
//

#ifndef BUSCA_IMD_LIST_H
#define BUSCA_IMD_LIST_H

#define INDEX_OF_BOUND_EXCEPTION

#include "iterator.h"

using core::Iterator;

namespace core {

    template <typename Element>
    class List {

    private:
        typedef struct tpNode {
            tpNode * prev;
            tpNode * next;
            Element * element;
        } * Node;

        class ListCursor : public Iterator<Element>::Cursor {
            const List * list;
            Node cursor;
        public:
            /**
             * Constructors/destructor
             */
            ListCursor(const List<Element> * list);
            ListCursor(ListCursor & other);

            virtual bool isNotEnd();
            virtual void next();
            virtual Element &get();
        };

        int mSize;
        Node mFirst;
        Node mLast;

        void init();
        Node getNodeAt(int index);

    public:

        /**
         * Contructor / Destructor
         */
        List();
        List(const List<Element> &other);

        ~List();

        bool add(Element element, int index = -1);

        bool remove(Element element);
        Element get(int index);
        int size();
        Iterator<Element> iterator() const ;


    private:

    };
}

// Implementation

using core::List;

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
List<Element>::List(const List<Element> & other) {
    init();

    Iterator<Element> iterator = other.iterator();
    while (iterator.isNotEnd()) {
        add(*iterator);
        ++iterator;
    }
}

/**
 * Destrutor
 */
template <typename Element>
List<Element>::~List() {
    Node markedForDeathNode, cursor = mFirst;

    do {
        markedForDeathNode = cursor;
        cursor = cursor->next;

        delete markedForDeathNode->element;
        delete markedForDeathNode;
    } while (cursor);
}


/**
 * Inicializador das sentnelas e do tamanho
 */
template <typename Element>
void List<Element>::init() {
    mFirst = new struct tpNode({nullptr, nullptr, nullptr});
    mLast = new struct tpNode({nullptr, nullptr, nullptr});
    mFirst->next = mLast;
    mLast->prev = mFirst;

    mSize = 0;
}

/**
 * Obtem o iterador da lista
 */
template <typename Element>
Iterator<Element> List<Element>::iterator() const {
    List<Element>::ListCursor * cursor = new List<Element>::ListCursor(this);
    return Iterator<Element>(cursor);
}


template <typename Element>
bool List<Element>::add(Element element, int index) {
    Node cursor = getNodeAt(index);

    if (!cursor)
        return false;

    Node node = new tpNode;
    node->element = new Element(element);

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
    if (index + 1 < -mSize || index > mSize) return nullptr;

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
List<Element>::ListCursor::ListCursor(const List<Element> *list)
        : list(list), cursor(list->mFirst->next) {
}

template <typename Element>
List<Element>::ListCursor::ListCursor(List<Element>::ListCursor &other)
        : list(other.list), cursor(other.cursor) {
}

template <typename Element>
bool List<Element>::ListCursor::isNotEnd() {
    return cursor->next != nullptr;
}

template <typename Element>
void List<Element>::ListCursor::next() {
    if (cursor->next) cursor = cursor->next;
}


template <typename Element>
Element & List<Element>::ListCursor::get() {
    return *(cursor->element);
}


#endif //BUSCA_IMD_LIST_H
