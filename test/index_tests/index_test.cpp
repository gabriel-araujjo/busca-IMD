//
// Created by gabriel on 6/9/16.
//

#include "gtest/gtest.h"
#include "cases_config.h"
#include "list.h"
#include "short_string.h"
#include "hash_map.h"
#include <sstream>
#include <fstream>
#include "index.h"


using busca_imd_core::HashMap;
using busca_imd_core::List;
using busca_imd_core::ShortString;
using busca_imd_index::Index;
using busca_imd_core::ultraFastHash;


void printIndex() {
    std::cout << std::endl << "printIndex" << std::endl;

    for (auto entry : Index::getInstance()) {
        std::cout << *entry.key << "-> [";

        for (auto entry2 : *entry.value) {
            std::cout << *entry2.key << " -> [";

            for (auto line : *entry2.value) {
                std::cout << " " << line;
            }
            std::cout << "], ";
        }

        std::cout << "]" << std::endl;
    }
}


class IndexTest : public  ::testing::Test {


protected:
    HashMap<ShortString*, List<ShortString*>> * map = nullptr;

    virtual void SetUp() {

        map = new HashMap<ShortString*, List<ShortString*>>(ultraFastHash);

        static const char * cases_path = CASES_DIR PATH_SEPARATOR "index.cases";

        std::ifstream cases;
        cases.open(cases_path);
        if(!cases.is_open())
        {
            std::cout << std::endl << "Não foi possível abrir arquivo " << cases_path << "Programa será terminado!" << std::endl;
            cases.clear( );
            FAIL();
        }

        std::string line, file, word;

        while (std::getline(cases, line)) {
            std::stringstream lineStream(line);

            lineStream >> file;
            List<ShortString*> words;
            while (!lineStream.eof()) {
                lineStream >> word;
                words.add(new ShortString(word.c_str()));
            }
            map->put(new ShortString(file.c_str()), words);
        }

        cases.close();
    }

    virtual void TearDown() {
        List<ShortString*> files;
        for (auto entry : *map) {
            files.add(entry.key);
            for (auto word : entry.value) {
                delete word;
            }
        }
        for (auto file : files) {
            delete file;
        }
        delete map;
    }
};

TEST_F(IndexTest, Sanity) {
    if (map == nullptr) {
        FAIL();
    }

    std::cout << std::endl << "map keys =" << std::endl;
    for (auto entry : *map) {
        std::cout << std::endl << *entry.key << "-> [";
        for (auto word : entry.value) {
            std::cout << ", " << *word;
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}


TEST_F(IndexTest, Addition) {
    for (auto entry : *map) {
        for (auto word : entry.value) {
            int line = 1;
            Index::getInstance().addEntry(entry.key, word, line);
            EXPECT_TRUE(Index::getInstance().get(word)->size() > 0);
            EXPECT_NO_THROW(Index::getInstance().get(word)->get(entry.key));
            EXPECT_TRUE(Index::getInstance().get(word)->get(entry.key)->contains(line));
        }
    }

    printIndex();
}

TEST_F(IndexTest, Removing) {
    std::cout << std::endl << Index::getInstance().size() << std::endl;
    EXPECT_TRUE(Index::getInstance().size() > 0);

    printIndex();

//    for (auto entry : *map) {
//        Index::getInstance().removeFile(entry.key);
//    }

//    printIndex();

    EXPECT_EQ(Index::getInstance().size(), 10);
}
