//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_CONFIG_H
#define BUSCA_IMD_CONFIG_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>

#include "list.h"
#include "hash_map.h"
#include "short_string.h"
#include "index.h"

#define CONFIG_DIR_NAME ".busca_imd"

namespace busca_imd_config{
    //this class will read the files inputed by the user, add the properties of this file to the file index and
    //will populate the word hash map
    class Config{
    private:
        typedef struct tpInfos{
            busca_imd_core::ShortString filePath;
            time_t lastModified;
            int totalWords;
            busca_imd_core::HashMap<busca_imd_core::ShortString, void *> mapOfWords;
        } * Infos;

        busca_imd_core::List<Infos> mInfoList;

        busca_imd_core::ShortString * readShortString(std::fstream & fileStream);
        busca_imd_index::FileHashMap * readFileHashMap(std::fstream & fileStream, busca_imd_core::Array<busca_imd_core::ShortString *> & filesLUT);
        uint16_t readUint16(std::fstream & fileStream);
        uint32_t readUint32(std::fstream &fileStream);
        busca_imd_core::Array<busca_imd_core::ShortString *> readFilesPathArray(std::fstream & fileStream);
        busca_imd_core::List<int> * readListOfOccurrences(std::fstream &fileStream);

        void readIndex(busca_imd_core::ShortString indexPath);
        void readInfoList(busca_imd_core::ShortString infoListPath);

        bool needToUpdate(busca_imd_core::ShortString filePath);

        void removeSpecialCharacters(busca_imd_core::ShortString &word);

        Config();

    public:

        static Config & getInstance();

        Config(Config const&) = delete;
        void operator=(Config const&) = delete;

        void insertOrUpdateFile(busca_imd_core::ShortString filePath);
        void removeFile(busca_imd_core::ShortString filePath);
        void listFiles(busca_imd_core::ShortString order);
    };

}

#endif //BUSCA_IMD_CONFIG_H
