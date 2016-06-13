//
// Created by gabriel on 6/12/16.
//

#include <cstdio>
#include <fstream>

#include "gtest/gtest.h"
#include "file_info.h"

using busca_imd_config::FileInfo;

TEST(FileInfo, Serialization) {

    FileInfo info;
    info.totalWords = 5;
    info.filePath = "/home/user/alguma-coisa";
    info.lastModified = 1231231;


    char * tmpFilePath = std::tmpnam(nullptr);
    std::ofstream out(tmpFilePath, std::ios::binary);
    out << info;
    out.flush();
    out.close();

    info.totalWords = 10;
    info.filePath = "/tmp/caminho-errado";
    info.lastModified = 0;

    std::ifstream in(tmpFilePath, std::ios::binary);
    in >> info;

    EXPECT_EQ(info.totalWords, 5);
    EXPECT_EQ(info.filePath, "/home/user/alguma-coisa");
    EXPECT_EQ(info.lastModified, 1231231);

    std::remove(tmpFilePath);
    EXPECT_EQ(0,0);
}

