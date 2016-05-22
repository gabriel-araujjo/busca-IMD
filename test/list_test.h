//
// Created by pdr_m on 21/05/2016.
//

#ifndef BUSCA_IMD_LIST_TEST_H
#define BUSCA_IMD_LIST_TEST_H

namespace test{
    class List_test {
        bool passed;

    public:
        List_test();
        List_test(List_test other);

        virtual ~List_test();

        bool start_test();

    private:
        bool test_create();
        bool test_destroy();

        bool test_insertion();

        bool test_remove();

        bool test_get();

        bool test_size();
    };
}



#endif //BUSCA_IMD_LIST_TEST_H
