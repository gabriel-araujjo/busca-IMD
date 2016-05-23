//
// Created by pdr_m on 21/05/2016.
//

#ifndef BUSCA_IMD_LIST_TEST_H
#define BUSCA_IMD_LIST_TEST_H

namespace test{
    /*
     * Class for test the List class
     */

    class List_test {
        bool passed;

    public:
        List_test();
        List_test(List_test &other);

        virtual ~List_test();

        void start_test();

    private:
        //Esses foram os testes escolhidos, pois são as únicas classes public da classe list
        static bool test_insertion();
        static bool test_remove();

        static bool test_get();

    };
}

#endif //BUSCA_IMD_LIST_TEST_H
