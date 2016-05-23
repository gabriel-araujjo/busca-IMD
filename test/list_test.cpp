//
// Created by pdr_m on 21/05/2016.
//

#include <iostream>

#include "list_test.h"
#include "../core/list.h"
#include "../core/list.cpp" //C:/Users/pdr_m/ClionProjects/busca-IMD/test/list_test.cpp:51: undefined reference to `core::List<int>::List()'
                            //verificar isso (por algum motivo que não sei, se eu tirar esse include, ele dá o erro acima)

using namespace std;
using namespace core;
using namespace test;

List_test::List_test() {
    passed = true;
}

List_test::List_test(List_test &other) {}

List_test::~List_test() {}

void List_test::start_test() {

    clog << "STARTING tests on the list..." << endl;

    if(passed)
        passed = test_insertion();

    if(passed)
        passed = test_remove();

    if(passed)
        passed = test_get();

    if(passed){
        clog << "FINISHED tests on the list without errors." << endl;
    }else{
        clog << "FINISHED tests on the list with errors." << endl;
    }

}

bool List_test::test_insertion() {
    clog << "\tSTARTING insertion tests on the list..." << endl;
    List<int> test;

    int array_test[10] = {0,1,2,3,4,5,6,7,8,9};

    for(int i = 0; i < 10; i++){
        test.add(array_test[i], (i+1));//está sempre caindo no primeiro caso do list.cpp:114:getNodeAt(int index)
                                       //retornando nullptr para o cursor, consequentemente, não adicionando o elemento
    }
    if(test.size() != 10){
        cerr << "\t\tERROR - The size was not increment correctly" << endl;
        cerr << "\t\t\tThe size expected was 10, instead, found " << test.size() << endl;

        clog << "\tFINISHED insertion tests on the list with errors." << endl;

        return false;
    }
    clog << "\tFINISHED insertion tests on the list without errors" << endl;
    return true;
}

bool List_test::test_remove() {
    clog << "\tSTARTING remove tests on the list..." << endl;
    List<int> test;

    int array_test[10] = {0,1,2,3,4,5,6,7,8,9};

    for(int i = 0; i < 10; i++){
        test.remove(array_test[i]);
    }
    if(test.size() != 0){
        cerr << "\t\tERROR - The size was not increment correctly" << endl;
        cerr << "\t\t\tThe size expected was 0, instead, found " << test.size() << endl;

        clog << "\tFINISHED remove tests on the list with errors." << endl;

        return false;
    }
    clog << "\tFINISHED remove tests on the list without errors" << endl;
    return true;
}

bool List_test::test_get() {
    clog << "\tSTARTING get tests on the list..." << endl;
    List<int> test;

    int array_test[10] = {0,1,2,3,4,5,6,7,8,9};

    for(int i = 0; i < 10; i++){
        test.add(array_test[i], (i + 1));
    }

    for(int i = 0; i < 10; i++){
        if(test.get(i + 1) != array_test[i]){
            cerr << "\t\tERROR - The returned element wasn't the right one" << endl;
            cerr << "\t\t\tThe element expected was "<< array_test[i] <<", instead, found " << test.get(i + 1) << endl;

            clog << "\tFINISHED get tests on the list with errors." << endl;

            return false;
        };
    }
    clog << "\tFINISHED get tests on the list without errors" << endl;
    return true;
}

























