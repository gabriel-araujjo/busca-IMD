//
// Created by Pedro Paulo on 30/05/2016.
//
#include "gtest/gtest.h"

#include "config.h"

using namespace config;

TEST(FILE, insertFile){
    File file;

//    file.insertFile((char*)"C:\\Users\\Pedro Paulo\\Desktop\\Arquivos de teste\\LoremIpsumSmall.txt");
//    file.insertFile((char*)"C:\\Users\\Pedro Paulo\\Desktop\\Arquivos de teste\\LoremIpsum.txt");
//    file.insertFile((char*)"C:\\Users\\Pedro Paulo\\Desktop\\Arquivos de teste\\LoremIpsumBig.txt");
    file.insertFile((char*)"C:\\TESTE\\LoremIpsumSmall.txt");
}

TEST(FILE, removeFile){
    File file;

    file.removeFile((char*)"C:\\TESTE\\LoremIpsumSmall.txt");
}




