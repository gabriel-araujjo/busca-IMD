//
// Created by pdr_m on 21/05/2016.
//

#include "list_test.h"
#include "../core/list.h"

test::List_test::List_test() {
    passed = true;
}

test::List_test::List_test(List_test other) {

}

test::List_test::~List_test() {

}

bool test::List_test::start_test() {

    passed = test_create();

    if(passed)
        passed = test_insertion();

    if(passed)
        passed = test_get();

    if(passed)
        passed = test_remove();

    if(passed)
        passed = test_size();

    if(passed)
        passed = test_destroy();

    return passed;
}

bool test::List_test::test_create() {
    return false;
}

bool test::List_test::test_destroy() {
    return false;
}

bool test::List_test::test_insertion() {
    return false;
}

bool test::List_test::test_remove() {
    return false;
}

bool test::List_test::test_get() {
    return false;
}

bool test::List_test::test_size() {
    return false;
}

























