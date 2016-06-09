//
// Created by Pedro Paulo on 30/05/2016.
//
#include "gtest/gtest.h"

#include "config.h"

using namespace busca_imd_config;

TEST(FILE, insertFile){
Config file;

file.insertOrUpdateFile((char *) "C:\\TESTE\\LoremIpsumSmall.txt");
}

TEST(FILE, removeFile){

}




