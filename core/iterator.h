//
// Created by gabriel on 5/19/16.
//

#ifndef BUSCA_IMD_ITERATOR_H
#define BUSCA_IMD_ITERATOR_H

namespace core {
    template <typename Element>
    class Iterator {
    public:
        class Cursor {
        public:
            int references = 0;
            virtual Element & get() = 0;
            virtual void next() = 0;
            virtual bool isNotEnd() = 0;
        };

    private:
        Cursor * mCursor;

    public:

        Iterator(Cursor * cursor) {
            mCursor = cursor;
            mCursor->references++;
        }

        Iterator(const Iterator<Element> & other) {
            mCursor = other.mCursor;
            mCursor->references++;
        }

        Element & operator*() {
            return mCursor->get();
        };

        Iterator<Element> & operator++() {
            mCursor->next();
            return *(this);
        };

        Iterator &operator =(const Iterator & other) {
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

        bool isNotEnd() {
            mCursor->isNotEnd();
        };

        ~Iterator() {
            mCursor->references--;
            if (mCursor->references <= 0) {
                delete mCursor;
            }
        }
    };
}


#endif //BUSCA_IMD_ITERATOR_H
