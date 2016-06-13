//
// Created by gabriel on 6/9/16.
//

#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>

#include "gtest/gtest.h"
#include "cases_config.h"
#include "list.h"
#include "short_string.h"
#include "hash_map.h"
#include "index.h"


using busca_imd_core::HashMap;
using busca_imd_core::List;
using busca_imd_core::ShortString;
using busca_imd_index::Index;
using busca_imd_core::directUltraFastHash;

class IndexTest : public  ::testing::Test {


protected:

    static HashMap<ShortString, List<ShortString>> * & getMap() {
        static HashMap<ShortString, List<ShortString>> * indexMap(nullptr);
        return indexMap;
    }

    virtual void SetUp() {

        if (IndexTest::getMap()) {
            // the map has already been created
            return;
        }

        if (Index::getInstance().size()){
            FAIL();
        }

        IndexTest::getMap() = new HashMap<ShortString, List<ShortString>>(directUltraFastHash);

        static const char * cases_path = CASES_DIR FILE_SEPARATOR "index.cases";

        std::ifstream cases;
        cases.open(cases_path);
        if(!cases.is_open())
        {
            std::cout << std::endl << "Can't open file " << cases_path << "Test fail!" << std::endl;
            FAIL();
        }

        std::string line, file, word;

        while (std::getline(cases, line)) {
            std::stringstream lineStream(line);

            lineStream >> file;
            List<ShortString> words;
            while (!lineStream.eof()) {
                lineStream >> word;
                words.add(word.c_str());
            }
            IndexTest::getMap()->put(file.c_str(), words);
        }

        cases.close();
    }
};

TEST_F(IndexTest, Sanity) {
    std::cout << std::endl << "Sanity test" << std::endl;
    if (IndexTest::getMap() == nullptr) {
        FAIL();
    }
}


TEST_F(IndexTest, Addition) {
    std::cout << std::endl << "Addition Test" << std::endl;
    for (auto entry : *IndexTest::getMap()) {
        for (auto word : entry.value) {
            int line = 1;
            Index::getInstance().addEntry(entry.key, word, line);
            EXPECT_TRUE(Index::getInstance().get(word)->size() > 0);
            EXPECT_NO_THROW(Index::getInstance().get(word)->get(&entry.key));
            EXPECT_TRUE(Index::getInstance().get(word)->get(&entry.key)->contains(line));
        }
    }
}

TEST_F(IndexTest, Removing) {

    if (Index::getInstance().size() == 0) {
        for (auto entry : *IndexTest::getMap()) {
            for (auto word : entry.value) {
                int line = 0;
                Index::getInstance().addEntry(entry.key, word, line);
            }
        }
    }

    std::cout << std::endl << "Removing Test" << std::endl;

    EXPECT_TRUE(Index::getInstance().size() > 0);

    for (auto entry : *IndexTest::getMap()) {
        Index::getInstance().removeFile(entry.key);

        // No words can contain this file
        for (auto wordHashEntry : Index::getInstance()) {
            EXPECT_FALSE(wordHashEntry.value->contains(&entry.key));
        }
    }

    EXPECT_EQ(Index::getInstance().size(), 0);
}

TEST_F(IndexTest, Release) {
    std::cout << std::endl << "Release Test" << std::endl;

    if (Index::getInstance().size() == 0) {
        for (auto entry : *IndexTest::getMap()) {
            for (auto word : entry.value) {
                int line = 1;
                Index::getInstance().addEntry(entry.key, word, line);
            }
        }
    }
    EXPECT_TRUE(Index::getInstance().size() > 0);

    Index::getInstance().release();

    EXPECT_EQ(Index::getInstance().size(), 0);
}

TEST_F(IndexTest, Serialization) {
    std::cout << std::endl << "Serialization Test" << std::endl;
    if (Index::getInstance().size() == 0) {
        for (auto entry : *IndexTest::getMap()) {
            for (auto word : entry.value) {
                int line = 1;
                Index::getInstance().addEntry(entry.key, word, line);
            }
        }
    }
    EXPECT_TRUE(Index::getInstance().size() > 0);

    char * tmpFilePath = std::tmpnam(nullptr);

    std::ofstream out;
    out.open(tmpFilePath, std::ios::binary);
    out << Index::getInstance();
    out.flush();
    out.close();
    EXPECT_TRUE(Index::getInstance().size() > 0);
    Index::getInstance().release();

    std::ifstream in;
    in.open(tmpFilePath, std::ios::binary);
    in >> Index::getInstance();
    in.close();


    EXPECT_TRUE(Index::getInstance().size() > 0);

    out.open(tmpFilePath, std::ios::binary);
    out << Index::getInstance();
    out.flush();
    out.close();

    EXPECT_TRUE(Index::getInstance().size() > 0);
    Index::getInstance().release();
    EXPECT_EQ(Index::getInstance().size(), 0);
    std::remove(tmpFilePath);
}
