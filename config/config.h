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
#include "iterator.h"
#include "short_string.h"
#include "index.h"


namespace config{
    //this class will read the files inputed by the user, add the properties of this file to the file index and
    //will populate the word hash map
    class Config{
    private:
        typedef struct tpInfos{
            core::ShortString filePath;
            time_t lastMofified;
            int totalWords;
            core::HashMap<core::ShortString, void *> mapOfWords;
        } * Infos;

        index::Index mIndex;
        core::List<Infos> mInfoList;

        core::ShortString * readShortString(std::fstream & fileStream);
        index::FileHashMap * readFileHashMap(std::fstream & fileStream, core::Array<core::ShortString *> & filesLUT);
        uint16_t readUint16(std::fstream & fileStream);
        uint32_t readUint32(std::fstream &fileStream);
        core::Array<core::ShortString *> readFilesPathArray(std::fstream & fileStream);
        core::List<int> * readListOfOcurrences(std::fstream & fileStream);

        void readIndex(core::ShortString indexPath);
        void readInfoList(core::ShortString infoListPath);

        bool needToUpdate(core::ShortString filePath);
        core::ShortString removeSpecialCharacters(core::ShortString word);

        Config(){};

    public:

        static Config & getInstance();

        Config(Config const&) = delete;
        void operator=(Config const&) = delete;

        void insertOrUpdateFile(core::ShortString filePath);
        void removeFile(core::ShortString filePath);
        void listFiles(core::ShortString order);
    };

}

#endif //BUSCA_IMD_CONFIG_H
