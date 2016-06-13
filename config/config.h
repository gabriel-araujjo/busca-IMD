//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_CONFIG_H
#define BUSCA_IMD_CONFIG_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>

#include "file_info.h"
#include "list.h"
#include "hash_map.h"
#include "short_string.h"
#include "index.h"

#define CONFIG_DIR_NAME "debugimd"
#define INDEX_FILE_PATH "index_file_imd"
#define INDEX_WORD_PATH "index_words_imd"

#define NATURAL_ORDER 'i'
#define WORDS_COUNT_ORDER 't'
#define ALPHABETICAL_ORDER 'a'

namespace busca_imd_config{
    //this class will read the files inputed by the user, add the properties of this file to the file index and
    //will populate the word hash map

    class Config{
    private:

        char * mInfoListFilePath;
        char * mIndexFilePath;
        busca_imd_core::List<FileInfo> mInfoList;

        char * getConfigDirPath();

        void loadIndex();

        void loadInfoList();

        void persistIndex();

        void persistInfoList();

        Config();

    public:

        static Config & getInstance();

        Config(Config const&) = delete;
        void operator=(Config const&) = delete;

        void insertOrUpdateFile(busca_imd_core::ShortString filePath);
        void removeFile(busca_imd_core::ShortString filePath);
        busca_imd_core::List<FileInfo> getFiles();
        void orderFiles(busca_imd_core::List<FileInfo> &files, char order);
    };

}

#endif //BUSCA_IMD_CONFIG_H
