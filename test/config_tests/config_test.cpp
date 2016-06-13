//
// Created by Pedro Paulo on 30/05/2016.
//
#include "gtest/gtest.h"

#include "cases_config.h"
#include "short_string.h"
#include "config_file_utils.h"
#include "config.h"

using busca_imd_core::ShortString;
using busca_imd_config::Config;


TEST(Config, Sanity) {
    busca_imd_config::Config::getInstance();
    SUCCEED();
}

TEST(Config, insertFile){
    const char * filePath_charArr = CASES_DIR FILE_SEPARATOR "portugues_com_acento.txt";
    ShortString filePath(filePath_charArr);

    busca_imd_config::Config::getInstance().insertOrUpdateFile(filePath);
    SUCCEED();
}




