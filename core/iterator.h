//
// Created by gabriel on 5/19/16.
//

#ifndef BUSCA_IMD_ITERATOR_H
#define BUSCA_IMD_ITERATOR_H

namespace core {
    template <typename Element>
    class Iterator {
    public:
        virtual Element operator*() = 0;

        virtual Iterator<Element> & operator++() = 0;

        virtual bool isEnd() = 0;
    };
}


#endif //BUSCA_IMD_ITERATOR_H
