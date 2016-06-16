//
// Created by gabriel on 5/19/16.
//

#ifndef BUSCA_IMD_LIST_H
#define BUSCA_IMD_LIST_H

#define LIST_INDEX_OF_BOUND_EXCEPTION 1

#include <stdint.h>

#include "iterator.h"

namespace busca_imd_core {

    template <typename Element>
    class List {
    public:
        typedef int (*Comparator) (const Element &, const Element &);
    private:
        typedef class tpNode {
        public:
            tpNode() : prev(nullptr), next(nullptr), element(nullptr) {

            }
            tpNode * prev;
            tpNode * next;
            Element * element;

            void remove() {
                if (!(prev && next))
                    return;
                prev->next = next;
                next->prev = prev;
                prev = nullptr;
                next = nullptr;
            }

            void addAfter(tpNode * node) {
                if (prev || next || !node)
                    return;
                prev = node;
                next = node->next;
                prev->next = this;
                next->prev = this;
            }

            void addBefore(tpNode * node) {
                if (prev || next || !node)
                    return;
                prev = node->prev;
                next = node;
                prev->next = this;
                next->prev = this;
            }
        } * Node;

        class ListCursor : public Iterator<Element>::Cursor {
            const List * list;
            Node cursor;
        public:
            /**
             * Constructors/destructor
             */
            ListCursor(const List<Element> * list, bool end = false);
            ListCursor(ListCursor & other);

            virtual bool isNotEnd();
            virtual void next();
            virtual Element &get();
            virtual bool samePosition(typename Iterator<Element>::Cursor * const other);
        };

        int mSize;
        Node mFirst;
        Node mLast;
        Iterator<Element> * mEndIterator;

        void init();
        Node getNodeAt(int index);
        void mergeSort(Node &first, Node &last, Comparator comparator);
        void merge(Node &start, Node &middle, Node &end, Comparator comparator);

    public:

        /**
         * Contructor / Destructor
         */
        List();
        List(const List<Element> &other);

        ~List();

        bool add(Element element, int index = -1);

        bool remove(Element element);
        void sort(Comparator comparator);
        void intersection(const List<Element> &other, Comparator comparator);
        void join(const List<Element> &other, Comparator);
        Element &get(int index);
        bool contains(Element &element);
        int size()const;
        void clear();
        Iterator<Element> begin() const;
        Iterator<Element>& end() const;

        List & operator=(const List & other);

        template <typename Type>
        friend std::ostream  &operator<<( std::ostream &output,
                                          const List<Type> &list );

        template <typename Type>
        friend std::istream  &operator>>( std::istream &input,
                                          List<Type> &list );

    };


    // Implementation


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

        Iterator<Element> iterator = other.begin();
        while (iterator.isNotEnd()) {
            add(*iterator);
            ++iterator;
        }
    }

    template <typename Element>
    List<Element>& List<Element>::operator=(const List<Element> &other) {
        if (this != &other) {

            clear();

            Node cursor = other.mFirst->next, node = mFirst;

            while (cursor != other.mLast) {
                Node newNode = new tpNode;
                node->next = newNode;
                newNode->prev = node;
                newNode->element = new Element(*(cursor->element));
                node = newNode;
                cursor = cursor->next;
            }
            node->next = mLast;
            mLast->prev = node;
            mSize = other.mSize;
        }
        return *this;
    }

/**
 * Destrutor
 */
    template <typename Element>
    List<Element>::~List() {
        delete mEndIterator;
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
        mFirst = new tpNode;
        mLast = new tpNode;
        mFirst->next = mLast;
        mLast->prev = mFirst;

        mSize = 0;
        mEndIterator = new Iterator<Element>(new ListCursor(this, true));
    }

/**
 * Obtem o iterador da lista
 */
    template <typename Element>
    Iterator<Element> List<Element>::begin() const {
        List<Element>::ListCursor * cursor = new List<Element>::ListCursor(this);
        return Iterator<Element>(cursor);
    }

    template <typename Element>
    Iterator<Element>& List<Element>::end() const {
        return *mEndIterator;
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
    int List<Element>::size()const {
        return mSize;
    }

    template <typename Element>
    void List<Element>::clear() {
        typename busca_imd_core::List<Element>::Node node, cursor = mFirst->next;
        while (cursor != mLast) {
            node = cursor;
            cursor = cursor->next;

            delete node->element;
            delete node;
        }

        mFirst->next = mLast;
        mLast->prev = mFirst;
        mSize = 0;
    }

    template <typename Element>
    Element &List<Element>::get(int index) {
        Node node = getNodeAt(index);
        if (!node) throw LIST_INDEX_OF_BOUND_EXCEPTION;
        return *(node->element);
    }

    template <typename Element>
    bool List<Element>::contains(Element &element){
        for(Element e : *this){
            if(e == element){
                return true;
            }
        }
        return false;
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

    template <typename Element>
    void List<Element>::sort(Comparator comparator) {
        Node first = mFirst->next;
        Node last = mLast;
        mergeSort(first, last, comparator);
    }

    template <typename Element>
    void List<Element>::intersection(const List<Element> &other, Comparator comparator) {
        Node thisCursor = mFirst->next;
        Node thatCursor = other.mFirst->next;

        Node toRemove;
        while(thisCursor != mLast && thatCursor != other.mLast) {
            int result = comparator(*thisCursor->element, *thatCursor->element);
            if (result < 0) {
                toRemove = thisCursor;
                thisCursor = thisCursor->next;
                toRemove->remove();
                mSize--;
            } else if (result == 0) {
                thisCursor = thisCursor->next;
                thatCursor = thatCursor->next;
            } else {
                thatCursor = thatCursor->next;
            }
        }

        while (thisCursor != mLast) {
            toRemove = thisCursor;
            thisCursor = thisCursor->next;
            toRemove->remove();
            mSize--;
        }
    }

    template <typename Element>
    void List<Element>::join(const List<Element> &other, Comparator comparator) {
        Node thisCursor = mFirst->next;
        Node thatCursor = other.mFirst->next;

        Node toAdd;
        while(thisCursor != mLast && thatCursor != other.mLast) {
            int result = comparator(*(thisCursor->element), *(thatCursor->element));
            if (result < 0) {
                thisCursor = thisCursor->next;
            } else if (result == 0) {
                thisCursor = thisCursor->next;
                thatCursor = thatCursor->next;
            } else {
                toAdd = new tpNode();
                toAdd->element = new Element(*(thatCursor->element));
                toAdd->addBefore(thisCursor);
                thatCursor = thatCursor->next;
                mSize++;
            }
        }

        while (thatCursor != other.mLast) {
            toAdd = new tpNode();
            toAdd->element = new Element(*(thatCursor->element));
            toAdd->addBefore(mLast);
            thatCursor = thatCursor->next;
            mSize++;
        }
    }

    template <typename Element>
    void List<Element>::mergeSort(Node &first, Node &last, Comparator comparator) {
        if (first == last || first->next == last) return;

        Node slow, fast;
        slow = fast = first;
        while (fast != last) {
            fast = fast->next;
            if (fast != last) {
                fast = fast->next;
                slow = slow->next;
            }
        }
        mergeSort(first, slow, comparator);
        mergeSort(slow, last, comparator);
        merge(first, slow, last, comparator);
    }


    template <typename Element>
    void List<Element>::merge(Node &start, Node &middle, Node &end, Comparator comparator) {
        Node node;
        Node left = start;
        while (left != middle && middle != end) {
            if (comparator(*middle->element, *left->element) < 0) {
                node = middle;
                middle = middle->next;
                node->remove();
                node->addBefore(left);
                if (left == start) {
                    start = node;
                }
            } else {
                left = left->next;
            }
        }
    }


/******************************************************************
 * ITERADOR DE LISTA
 ******************************************************************/
    template <typename Element>
    List<Element>::ListCursor::ListCursor(const List<Element> *list, bool end)
            : list(list), cursor(end ? list->mLast : list->mFirst->next) {
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

    template <typename Element>
    bool List<Element>::ListCursor::samePosition(typename Iterator<Element>::Cursor * const other) {
        return cursor == ((List<Element>::ListCursor *) other)->cursor;
    }

    template <typename Element>
    std::ostream  &operator<<( std::ostream &output,
                                      const busca_imd_core::List<Element> &list ) {
        uint16_t listSize = (uint16_t) list.mSize;
        output.write((char*) &listSize, sizeof(uint16_t));
        for (auto &&item : list) {
            output << item;
        }
        return output;
    }

    template <typename Element>
    std::istream  &operator>>( std::istream &input,
                                      busca_imd_core::List<Element> &list ) {

        list.clear();


        uint16_t listPosition;
        input.read((char*) & listPosition, sizeof(uint16_t));
        while (listPosition) {
            Element e;
            input >> e;
            list.add(e);
            listPosition--;
        }

        return input;
    }

}

#endif //BUSCA_IMD_LIST_H
