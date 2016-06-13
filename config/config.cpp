//
// Created by gabriel on 5/17/16.
//


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

    Config::Config() {
        std::setlocale(LC_COLLATE, "pt_BR.UTF-8");
        char *cfgDir = getConfigDirPath();
        mIndexFilePath = joinPath(cfgDir, INDEX_WORD_PATH);
        mInfoListFilePath = joinPath(cfgDir, INDEX_FILE_PATH);
        loadIndex();
        loadInfoList();
    }

    char * Config::getConfigDirPath() {

        char * home = getHomeDir();
        char * config_dir = joinPath(home, CONFIG_DIR_NAME);

        delete home;

        if(!dirExists(config_dir)){
            createHiddenDir(config_dir);
        }

        return config_dir;
    }

    void Config::loadIndex() {
        //If index file does not exist, don't read it
        if (!fileExists(mIndexFilePath)) return;
        try {
            std::ifstream in;
            in.open(mIndexFilePath, std::ios::in);
            if (!in.is_open()) {
                return;
            }
            in >> Index::getInstance();
        } catch (std::ifstream::failure e) {
            Index::getInstance().release();
        }
    }

    void Config::loadInfoList() {
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
            std::cout << std::endl << "trying persist " << mIndexFilePath << std::endl;
            out.open(mIndexFilePath, std::ios::trunc | std::ios::out);
            if (!out.is_open()) {
                std::cout << std::endl << "can't persist index file" << std::endl;
                return;
            }
            std::cout << "persisting index into file " << mIndexFilePath << std::endl;
            out << Index::getInstance();
            out.flush();
        } catch (std::fstream::failure e) {
            std::cout << std::endl << "can't persist index file" << std::endl;
        }
    }

    void Config::persistInfoList() {
        try {
            std::fstream out;
            out.open(mInfoListFilePath, std::ios::trunc | std::ios::out);
            if (!out.is_open()) {
                std::cout << std::endl << "can't persist index file" << std::endl;
                return;
            }
            out << mInfoList;
            out.flush();
        } catch (std::fstream::failure e) {
            std::cout << std::endl << "can't persist index file" << std::endl;
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

        std::cout << "adding " << filePath << std::endl;
        FileInfo info(filePath);
        mInfoList.add(info);

        persistIndex();
        persistInfoList();
    }

    void Config::removeFile(busca_imd_core::ShortString filePath) {
        for (FileInfo info : mInfoList) {
            if (info.filePath == filePath) {
                Index::getInstance().remove(filePath);
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

    void Config::orderFiles(busca_imd_core::List<FileInfo> &files, char order) {
        switch (order) {
            case WORDS_COUNT_ORDER:
                files.sort(wordsCountOrder);
                break;
            case ALPHABETICAL_ORDER:
                files.sort(alphabeticalOrder);
                break;
            case NATURAL_ORDER:
                files = mInfoList;
                break;
        }
    }

    Config &Config::getInstance() {
        static Config config;
        return config;
    }
}