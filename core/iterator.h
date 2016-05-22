//
// Created by gabriel on 5/19/16.
//

#ifndef BUSCA_IMD_ITERATOR_H
#define BUSCA_IMD_ITERATOR_H

namespace core {
    template <typename Element>
    class Iterator {
    public:
        Element operator*();

        Iterator<Element> & operator++();

        bool isEnd();
    };
}


#endif //BUSCA_IMD_ITERATOR_H
