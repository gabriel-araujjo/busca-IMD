//
// Created by gabriel on 5/19/16.
//

#ifndef BUSCA_IMD_ITERATOR_H
#define BUSCA_IMD_ITERATOR_H

namespace busca_imd_core {
    template <typename Element>
    class Iterator {
    public:
        class Cursor {
        public:
            int references = 0;
            virtual Element & get() = 0;
            virtual void next() = 0;
            virtual bool isNotEnd() = 0;
            virtual bool samePosition(Cursor *const other) = 0;
        };

    private:
        Cursor * mCursor;

    public:

        Iterator(Cursor * cursor);

        Iterator(const Iterator<Element> & other);

        Element & operator*();

        Iterator<Element> & operator++();

        Iterator & operator=(const Iterator & other);

        bool operator!=(const Iterator & other);

        bool isNotEnd();

        ~Iterator();
    };


    // Implementation

    template <typename Element>
    Iterator<Element>::~Iterator() {
        mCursor->references--;
        if (mCursor->references <= 0) {
            delete mCursor;
        }
    }

    template <typename Element>
    bool Iterator<Element>::isNotEnd() {
        return mCursor->isNotEnd();
    }

    template <typename Element>
    Iterator<Element> &Iterator<Element>::operator=(const Iterator<Element> & other) {
        if (this != & other) {
            mCursor->references--;
            if (mCursor->references <= 0) {
                delete mCursor;
            }
            mCursor = other.mCursor;
            mCursor->references++;
        }
        return *this;
    }

    template <typename Element>
    bool Iterator<Element>::operator!=(const Iterator<Element> & other) {
        return !mCursor->samePosition(other.mCursor);
    }

    template <typename Element>
    Iterator<Element> &Iterator<Element>::operator++() {
        mCursor->next();
        return *(this);
    }

    template <typename Element>
    Element &Iterator<Element>::operator*() {
        return mCursor->get();
    }

    template <typename Element>
    Iterator<Element>::Iterator(const Iterator<Element> & other) {
        mCursor = other.mCursor;
        mCursor->references++;
    }

    template <typename Element>
    Iterator<Element>::Iterator(Iterator<Element>::Cursor * cursor) {
        mCursor = cursor;
        mCursor->references++;
    }
}


#endif //BUSCA_IMD_ITERATOR_H
