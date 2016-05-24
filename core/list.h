//
// Created by gabriel on 5/19/16.
//

#ifndef BUSCA_IMD_LIST_H
#define BUSCA_IMD_LIST_H

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
        List(List<Element> &other);

        ~List();

        bool add(Element element, int index = -1);

        bool remove(Element element);
        Element get(int index);
        int size();
        Iterator<Element> iterator();


    private:
        class ListIterator : public Iterator<Element> {
            const List list;
            Node cursor;
        public:
            /**
             * Constructors/destructor
             */
            ListIterator(const List &list);
            ListIterator(ListIterator & other);
            virtual ~ListIterator() { }

            virtual bool isEnd();
            virtual ListIterator &operator++();
            virtual Element &operator*();
        };
    };




}


#endif //BUSCA_IMD_LIST_H
