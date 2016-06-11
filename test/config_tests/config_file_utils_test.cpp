//
// Created by Pedro Paulo on 11/06/2016.
//
#include "gtest/gtest.h"

#include "config.h"
#include "config_file_utils.h"



TEST(ConfigFileUtils, getHomeDir){

    char * myDir = busca_imd_config::getHomeDir();

    EXPECT_NE(myDir, nullptr);
    EXPECT_GT(strlen(myDir), 0);

    delete myDir;
}


TEST(ConfigFileUtils, dirExists){
    char * myDirReal = busca_imd_config::getHomeDir();
    const char * myDirFake = "FAKE_DIRECTORY_123";
    EXPECT_TRUE(busca_imd_config::dirExists(myDirReal));
    EXPECT_FALSE(busca_imd_config::dirExists(myDirFake));
    delete myDirReal;
}

TEST(ConfigFileUtils, fileExists){
    char * myDir = busca_imd_config::getHomeDir();
    EXPECT_TRUE(busca_imd_config::fileExists(myDir));
    delete myDir;
}

TEST(ConfigFileUtils, createHiddenDir){

    char * myDir = busca_imd_config::getHomeDir();

    busca_imd_config::joinPath(myDir, CONFIG_DIR_NAME);

    EXPECT_FALSE(busca_imd_config::createHiddenDir(myDir));

    delete myDir;
}


