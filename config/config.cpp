//
// Created by gabriel on 5/17/16.
//


#include <stdlib.h>
#include <fstream>

#include "index.h"
#include "config_file_utils.h"
#include "config.h"


using busca_imd_core::Array;
using busca_imd_core::List;
using busca_imd_core::ShortString;
using busca_imd_index::Index;

namespace busca_imd_config {

    int wordsCountOrder(FileInfo const &a, FileInfo const &b) {
        return a.totalWords - b.totalWords;
    }

    int alphabeticalOrder(FileInfo const & a, FileInfo const & b) {
        return a.filePath.compare(b.filePath);
    }

    Config::Config() : initialized(false){
        std::setlocale(LC_COLLATE, "pt_BR.UTF-8");
        char *cfgDir = getConfigDirPath();
        mIndexFilePath = joinPath(cfgDir, INDEX_WORD_PATH);
        mInfoListFilePath = joinPath(cfgDir, INDEX_FILE_PATH);
        delete []cfgDir;
    }

    void Config::init(bool loadOnlyIndex) {
        if (!initialized) {
            initialized = true;
            internalLoadIndex();

            if (loadOnlyIndex) return;

            internalLoadInfoList();
        }
    }

    char * Config::getConfigDirPath() {

        char * home = getHomeDir();
        char * config_dir = joinPath(home, CONFIG_DIR_NAME);

        delete []home;

        if(!dirExists(config_dir)){
            createHiddenDir(config_dir);
        }

        return config_dir;
    }

    void Config::internalLoadIndex() {
        //If index file does not exist, don't read it
        if (!fileExists(mIndexFilePath)) return;
        try {
            std::ifstream in;
            in.open(mIndexFilePath, std::ios::in | std::ios::binary);
            if (!in.is_open()) {
                return;
            }
            in >> Index::getInstance();
        } catch (std::ifstream::failure e) {
            Index::getInstance().release();
        }
    }

    void Config::internalLoadInfoList() {
        if (!fileExists(mInfoListFilePath)) return;
        try {
            std::ifstream in;
            in.open(mInfoListFilePath, std::ios::in);
            if (!in.is_open()) {
                return;
            }
            in >> mInfoList;
        } catch (std::ifstream::failure e) {
            mInfoList.clear();
            Index::getInstance().release();
        }
    }

    void Config::persistIndex() {
        try {
            std::fstream out;
//            std::cout << std::endl << "trying persist " << mIndexFilePath << std::endl;
            out.open(mIndexFilePath, std::ios::trunc | std::ios::out);
            if (!out.is_open()) {
//                std::cout << std::endl << "can't persist index file" << std::endl;
                return;
            }
//            std::cout << "persisting index into file " << mIndexFilePath << std::endl;
            out << Index::getInstance();
            out.flush();
        } catch (std::fstream::failure e) {
//            std::cout << std::endl << "can't persist index file" << std::endl;
        }
    }

    void Config::persistInfoList() {
        try {
            std::fstream out;
            out.open(mInfoListFilePath, std::ios::trunc | std::ios::out);
            if (!out.is_open()) {
//                std::cout << std::endl << "can't persist index file" << std::endl;
                return;
            }
            out << mInfoList;
            out.flush();
        } catch (std::fstream::failure e) {
//            std::cout << std::endl << "can't persist index file" << std::endl;
        }
    }

    void Config::insertOrUpdateFile(busca_imd_core::ShortString filePath) {
        for (FileInfo info : mInfoList) {

            if (info.filePath == filePath) {
                if (info.updateIndex()) {
                    mInfoList.remove(info);
                    mInfoList.add(info);
                    persistIndex();
                    persistInfoList();
                }
                return;
            }
        }

//        std::cout << "adding " << filePath << std::endl;
        FileInfo info(filePath);
        mInfoList.add(info);

        persistIndex();
        persistInfoList();
    }

    void Config::removeFile(busca_imd_core::ShortString filePath) {
        for (FileInfo info : mInfoList) {
//            std::cout << "checkinf " << filePath << " against " << info.filePath << std::endl;
            if (info.filePath == filePath) {
                Index::getInstance().removeFile(filePath);
                mInfoList.remove(info);
                persistIndex();
                persistInfoList();
                return;
            }
        }
        throw FILE_NOT_FOUND;
    }

    busca_imd_core::List<FileInfo> Config::getFiles() {
        return mInfoList;
    }

    Config &Config::getInstance(bool loadOnlyIndex) {
        static Config config;
        config.init(loadOnlyIndex);
        return config;
    }

    void Config::loadIndex() {
        getInstance(true);
    }

    int Config::insertFiles(int argc, char **argv) {
        //insert a new file on the search base
        ShortString shortString;
        for (int i = 2; i < argc; i++) {
            char *file = realpath(argv[i], nullptr);
            std::cout << std::endl <<" File = " << file << std::endl;
            if (!file) continue;
            shortString = file;
            try {
                Config::getInstance().insertOrUpdateFile(shortString);
                std::cout << ">> Arquivo \"" << file << "\" inserido/atualizado." << std::endl;
            } catch (int fileNotFound) {
                std::cout << ">> Arquivo \"" << file << "\" não encontrado." << std::endl;
                exit(1);
            }
            //insert one or more files to the search base
        }//if it has zero files to insert, print a message explaining that the "-i" argument need at least
        //one file directory
    }

    int Config::removeFiles(int argc, char **argv) {
        //remove a file from the search base
        ShortString shortString;
        for (int i = 2; i < argc; i++) {
            char *file = realpath(argv[i], nullptr);
            shortString = file;
            try {
                Config::getInstance().removeFile(shortString);
                std::cout << ">> Arquivo \"" << file << "\" removido." << std::endl;
            } catch (int fileNotFound) {
                std::cout << ">> Arquivo \"" << file << "\" não está na base de busca." << std::endl;
                delete file;
                exit(1);
            }
            delete file;
            //remove one or more files to the search base
        }//if it has zero files to insert, print a message explaining that the "-r" argument need at least
        //one file name
    }

    int Config::listFiles(int argc, char **argv) {
        //list the files in the search base with NATURAl order
        List<FileInfo>::Comparator order = nullptr;

        switch (argv[1][2]) {
            case WORDS_COUNT_ORDER:
                order = wordsCountOrder;
                break;
            case ALPHABETICAL_ORDER:
                order = alphabeticalOrder;
                break;
            case NATURAL_ORDER:
                break;
            default:
                return 1;
        }

        List<FileInfo> files = Config::getInstance().getFiles();
        if (order) {
            files.sort(order);
        }
        if (files.size()) {
            std::cout << ">> Arquivos contidos na base de buscas " << std::endl;
            for (FileInfo fileInfo : files) {
                char * file_charArr = fileInfo.filePath.asCharArray();
                std::cout << "\t-\t\"" << file_charArr << "\"" << std::endl;
                delete[] file_charArr;
            }
        } else {
            std::cout << ">> Base de buscas vazia" << std::endl;
        }
        return 0;
    }


}