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

#define CONFIG_DIR_NAME ".buscaimd"
#define INDEX_FILE_PATH "index_file_imd"
#define INDEX_WORD_PATH "index_words_imd"

#define DB_FILE_NOT_MODIFIED 0
#define DB_FILE_INSERTION 1
#define DB_FILE_UPDATE 2

#define NATURAL_ORDER 'i'
#define WORDS_COUNT_ORDER 't'
#define ALPHABETICAL_ORDER 'a'

namespace busca_imd_config{
    //this class will read the files inputed by the user, add the properties of this file to the file index and
    //will populate the word hash map

    int searchEntryFileInsertionOrder(busca_imd_index::SearchResult::Entry const & a, busca_imd_index::SearchResult::Entry const & b);

    class Config{
    private:
        bool initialized;
        char * mInfoListFilePath;
        char * mIndexFilePath;
        busca_imd_core::List<FileInfo> mInfoList;
        busca_imd_core::HashMap<busca_imd_core::ShortString*, FileInfo*> mInfoIndex;

        char * getConfigDirPath();

        void internalLoadIndex();

        void internalLoadInfoList();

        void persistIndex();

        void persistInfoList();

        Config();

        void init();

    public:



        Config(Config const&) = delete;
        void operator=(Config const&) = delete;

        int insertOrUpdateFile(busca_imd_core::ShortString filePath);
        void removeFile(busca_imd_core::ShortString filePath);
        busca_imd_core::List<FileInfo> getFiles();

        // façade methods
        static Config &getInstance();

        static void loadIndex();

        static int insertFiles(int argc, char ** argv);

        static int removeFiles(int argc, char ** argv);

        static int listFiles(int argc, char ** argv);

        static busca_imd_index::SearchResultEntryList * orderSearch(busca_imd_index::SearchResult &result, busca_imd_index::SearchParams & params);

        friend int searchEntryFileInsertionOrder(busca_imd_index::SearchResult::Entry const & a, busca_imd_index::SearchResult::Entry const & b);
    };

}

#endif //BUSCA_IMD_CONFIG_H
