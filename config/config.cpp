//
// Created by gabriel on 5/17/16.
//


#include <stdlib.h>
#include <fstream>
#if defined(WIN32) || defined(_WIN32)
#include <fileapi.h>
#endif
#include "index.h"
#include "config_file_utils.h"
#include "config.h"


using busca_imd_core::Array;
using busca_imd_core::List;
using busca_imd_core::ShortString;
using busca_imd_index::Index;
using busca_imd_index::SearchResult;
using busca_imd_index::SearchResultEntryList;

namespace busca_imd_config {

    int wordsCountOrder(FileInfo const &a, FileInfo const &b) {
        return a.totalWords - b.totalWords;
    }

    int alphabeticalOrder(FileInfo const & a, FileInfo const & b) {
        return a.filePath.compare(b.filePath);
    }

    int searchEntryAlphabeticalOrder(SearchResult::Entry const & a, SearchResult::Entry const & b) {
        return a.key.compare(b.key);
    }

    int searchEntryReverseWordsCountOrder(SearchResult::Entry const & a, SearchResult::Entry const & b) {
        return b.value->size() - a.value->size();
    }

    int searchEntryFileInsertionOrder(SearchResult::Entry const & a, SearchResult::Entry const & b) {

        ShortString file = a.key;
        time_t aInsertion = Config::getInstance().mInfoIndex.get(&file)->insertionDate;
        file = b.key;
        time_t bInsertion = Config::getInstance().mInfoIndex.get(&file)->insertionDate;
        if (aInsertion > bInsertion) {
            return 1;
        } else if (aInsertion == bInsertion) {
            return 0;
        } else {
            return -1;
        }
    }

    Config::Config() : initialized(false), mInfoIndex(busca_imd_core::ultraFastHash){
        std::setlocale(LC_COLLATE, "pt_BR.UTF-8");
        char *cfgDir = getConfigDirPath();
        mIndexFilePath = joinPath(cfgDir, INDEX_WORD_PATH);
        mInfoListFilePath = joinPath(cfgDir, INDEX_FILE_PATH);
        delete []cfgDir;
    }

    void Config::init() {
        if (!initialized) {
            initialized = true;
            internalLoadIndex();
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
            for (auto &&info : mInfoList) {
                mInfoIndex.put(&info.filePath, &info);
            }
        } catch (std::ifstream::failure e) {
            mInfoList.clear();
            mInfoIndex.clear();
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

    int Config::insertOrUpdateFile(busca_imd_core::ShortString filePath) {
        int modificationType = DB_FILE_NOT_MODIFIED;
        try {
            FileInfo * info = mInfoIndex.get(&filePath);
            if (info->updateIndex()) {
                FileInfo updatedInfo = *info;
                mInfoIndex.remove(&info->filePath);
                mInfoList.remove(*info);
                mInfoList.add(updatedInfo);
                mInfoIndex.put(&updatedInfo.filePath, &mInfoList.get(mInfoList.size()- 1));
                modificationType = DB_FILE_UPDATE;
            }
        } catch (int e) {
            modificationType = DB_FILE_INSERTION;
            FileInfo info(filePath);
            mInfoList.add(info);
            mInfoIndex.put(&info.filePath, &mInfoList.get(mInfoList.size() -1));
        }
        if (modificationType != DB_FILE_NOT_MODIFIED) {
            persistIndex();
            persistInfoList();
        }
        return modificationType;
    }

    void Config::removeFile(busca_imd_core::ShortString filePath) {
        try {
            FileInfo * info = mInfoIndex.get(&filePath);
            Index::getInstance().removeFile(filePath);
            mInfoIndex.remove(&info->filePath);
            mInfoList.remove(*info);
            persistIndex();
            persistInfoList();
            return;
        } catch (int e) {
            throw FILE_NOT_FOUND;
        }
    }

    busca_imd_core::List<FileInfo> Config::getFiles() {
        return mInfoList;
    }

    Config &Config::getInstance() {
        static Config config;
        config.init();
        return config;
    }

    void Config::loadIndex() {
        getInstance();
    }

    int Config::insertFiles(int argc, char **argv) {
        //insert a new file on the search base
        ShortString shortString;
        for (int i = 2; i < argc; i++) {
#if defined(WIN32) || defined(_WIN32)
            char *file = new char[strlen(argv[i])+1];
            GetFullPathName(argv[i], strlen(argv[i])+1, file, 0);
#else
            char *file = new char[4096];
            file = realpath(argv[i], file);
#endif
//            std::cout << std::endl <<" File = " << file << std::endl;
            shortString = file;
            try {
                switch (Config::getInstance().insertOrUpdateFile(shortString)) {
                    case DB_FILE_INSERTION:
                        std::cout << ">> Arquivo \"" << file << "\" inserido." << std::endl;
                        break;
                    case DB_FILE_UPDATE:
                        std::cout << ">> Arquivo \"" << file << "\" atualizado." << std::endl;
                        break;
                    case DB_FILE_NOT_MODIFIED:
                    default:
                        std::cout << ">> Arquivo \"" << file << "\" sem modificações." << std::endl;
                        break;
                }
            } catch (int e) {
                std::cout << ">> Arquivo \"" << file << "\" não encontrado." << std::endl;
                delete[] file;
                exit(1);
            }
            delete[] file;
            //insert one or more files to the search base
        }//if it has zero files to insert, print a message explaining that the "-i" argument need at least
        //one file directory
        return 0;
    }

    int Config::removeFiles(int argc, char **argv) {
        //remove a file from the search base
        ShortString shortString;
        for (int i = 2; i < argc; i++) {
#if defined(WIN32) || defined(_WIN32)
            char *file = new char[strlen(argv[i])+1];
            GetFullPathName(argv[i], strlen(argv[i])+1, file, 0);
#else
            char *file = realpath(argv[i], nullptr);
#endif
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
        return 0;
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

    busca_imd_index::SearchResultEntryList *Config::orderSearch(busca_imd_index::SearchResult &result,
                                                                busca_imd_index::SearchParams &params) {
        SearchResultEntryList * list = new SearchResultEntryList;
        for (auto &&item : result) {
            list->add(item);
        }
        switch (params.order) {
            case 'A':
                list->sort(searchEntryAlphabeticalOrder);
                break;
            case 'C':
                list->sort(searchEntryReverseWordsCountOrder);
                break;
            case 'I':
            default:
                list->sort(searchEntryFileInsertionOrder);
                break;
        }
        return list;
    }

}