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
            struct tpNode* prev;
            struct tpNode* next;
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

        virtual ~List();

        bool add(Element element, int index = -1);

        bool remove(Element element);
        Element get(int index);
        int size();
        Iterator<Element> iterator();


    private:
        static class ListIterator : public Iterator<Element> {
            const List list;
            Node cursor;
        public:
            /**
             * Constructors/destructor
             */
            ListIterator(const List &list);
            ListIterator(ListIterator & other);
            virtual ~ListIterator() { }

            bool isEnd();
            ListIterator &operator++();
            Element operator*();
        };
    };




}


#endif //BUSCA_IMD_LIST_H
