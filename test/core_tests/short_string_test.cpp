//
// Created by gabriel on 6/7/16.
//


#include <string.h>
#include <iostream>
#include <fstream>

#include "gtest/gtest.h"

#include "list.h"
#include "short_string.h"
#include "cases_config.h"
#include "array.h"

using busca_imd_core::ShortString;
using busca_imd_core::List;
using busca_imd_core::Array;
using busca_imd_core::ultraFastHash;

double dispersionRate(unsigned int size) {
    static const char * cases_path = CASES_DIR FILE_SEPARATOR "words.cases";

    int step = size * 2 / 3;
    Array<uint32_t> target(size, 0);

    std::ifstream cases;
    cases.open(cases_path);
    if(!cases.is_open())
    {
        std::cout << "Não foi possível abrir arquivo " << cases_path << "Programa será terminado!" << std::endl;
        cases.clear( );
        exit(1);
    }

    // Linha do arquivo de teste
    std::string word;
    ShortString ss;
    uint32_t hash;
    int i = 0;

    List<int> filledSpaces;
    while (std::getline(cases, word)) {
        ss = word.c_str();
        hash = ultraFastHash(&ss);

        if (i < step) {
            target[hash % target.size()]++;
        }
        i++;

        if (i== step) {
            int occupancy = 0;
            for (int j = 0; j < target.size(); ++j) {
                if (target[j] >= 1) {
                    occupancy++;
                }
            }
            filledSpaces.add(occupancy);
            target.fill(0);
            i=0;
        }
    }

    cases.close();

    int occupancyMean = 0;

    for(int c : filledSpaces) {
        occupancyMean += c;
    }


    return ((double)occupancyMean) / step / filledSpaces.size();
}

TEST(ShortString, ConstStringConstructor) {

    const char * string = "hello world!";

    ShortString ss(string);

    EXPECT_EQ(ss, string);
    EXPECT_EQ(ss.length(), strlen(string));
}

TEST(ShortString, OtherShortStringConstructor) {

    const char * string = "hello world!";

    ShortString source(string);
    ShortString ss(source);

    EXPECT_EQ(ss.length(), source.length());

    EXPECT_EQ(ss, source);
}

TEST(ShortString, CharListConstructor) {

    const char * string = "hello world!";

    List<char> list;
    for (int i = 0; i < strlen(string); ++i) {
        list.add(string[i]);
    }

    ShortString ss(list);


    EXPECT_EQ(ss.length(), list.size());


    EXPECT_EQ(ss, string);

}


TEST(ShortString, Equals) {
    const char * string = "controle remoto";
    const char * otherString = "televisão";
    ShortString ss(string);
    EXPECT_NE(ss, otherString);
    EXPECT_EQ(ss, string);
}

TEST(ShortString, Hash) {
    ShortString vaso("vaso"), vaso2("vaso"), planta("planta"), pato("pato");

    EXPECT_EQ(busca_imd_core::ultraFastHash(&vaso), busca_imd_core::ultraFastHash(&vaso));
    EXPECT_EQ(busca_imd_core::ultraFastHash(&vaso), busca_imd_core::ultraFastHash(&vaso2));
    EXPECT_NE(busca_imd_core::ultraFastHash(&vaso), busca_imd_core::ultraFastHash(&planta));
    EXPECT_NE(busca_imd_core::ultraFastHash(&vaso), busca_imd_core::ultraFastHash(&pato));
    EXPECT_NE(busca_imd_core::ultraFastHash(&pato), busca_imd_core::ultraFastHash(&planta));
}

TEST(ShortString, HashColision) {


    EXPECT_NEAR(dispersionRate(0x10), 1, 0.30);
    EXPECT_NEAR(dispersionRate(0x20), 1, 0.30);
    EXPECT_NEAR(dispersionRate(0x80), 1, 0.30);
    EXPECT_NEAR(dispersionRate(0x800), 1, 0.30);
 }

