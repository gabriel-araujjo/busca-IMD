// trata os comandos do usuário

#define COMMAND_INSERT 'i'
#define COMMAND_REMOVE 'r'
#define COMMAND_LIST 'l'
#define COMMAND_SEARCH 'b'

#define SEARCH_ORDER_ALPHABETICAL 'A'
#define SEARCH_ORDER_OCCURRENCES 'C'
#define SEARCH_ORDER_INSERTION 'I'

#include "stdlib.h"
#include "string.h"
#include "config.h"

using busca_imd_config::Config;
using busca_imd_config::FileInfo;
using busca_imd_core::ShortString;
using busca_imd_core::List;
using busca_imd_core::HashMap;
using busca_imd_index::Index;
using busca_imd_index::FileHashMap;

int int_comparator (const int & a, const int & b) {
    return a - b;
}

int main(int argc, char ** argv) {

    const clock_t begin = clock();
    if (argc < 2 || argv[1][0] != '-') {
        //show help
        return 1;

    }

    int command = argv[1][1];

        //search base arguments
        //<-i <file_directory>, -r <file_name>, -li, -la, -lt>
    if (command == COMMAND_INSERT) {
        //insert a new file on the search base
        ShortString shortString;
        for (int i = 2; i < argc; i++) {
            char *file = realpath(argv[i], nullptr);
            shortString = file;
            try {
                Config::getInstance().insertOrUpdateFile(shortString);
                std::cout << ">> Arquivo \"" << file << "\" inserido/atualizado." << std::endl;
            } catch (int fileNotFound) {
                std::cout << ">> Arquivo \"" << file << "\" não encontrado." << std::endl;
                delete file;
                return 1;
            }
            delete file;
            //insert one or more files to the search base
        }//if it has zero files to insert, print a message explaining that the "-i" argument need at least
        //one file directory
        return 0;
    }

    if (command == COMMAND_REMOVE) {
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
                return 1;
            }
            delete file;
            //remove one or more files to the search base
        }//if it has zero files to insert, print a message explaining that the "-r" argument need at least
        //one file name
        return 0;
    }

    if (command == COMMAND_LIST) {
        //list the files in the search base with NATURAl order
        List<FileInfo> files = Config::getInstance().getFiles();
        Config::getInstance().orderFiles(files, argv[1][2]);
        if (files.size()) {
            std::cout << ">> Arquivos contidos na base de buscas " << std::endl;
            for (FileInfo fileInfo : files) {
                char * file_charArr = fileInfo.filePath.asCharArray();
                std::cout << "\t-\t\"" << file_charArr << "\"" << std::endl;
                delete file_charArr;
            }
        } else {
            std::cout << ">> Base de buscas vazia" << std::endl;
        }
        return 0;
    }

    bool exclusive = false;
    bool showTime = false;
    char order = SEARCH_ORDER_INSERTION;
    List<ShortString> words;
    for (int i = 1; i < argc; ++i) {
        switch (*argv[i]) {
            case '-':
                switch (argv[i][1]) {
                    case 'b':
                        exclusive = argv[i][2] == 'A';
                        break;
                    case 't':
                        showTime = argv[i][2] == 'T';
                        break;
                    case 'p':
                        order = argv[i][2];
                        break;
                    default:
                        break;
                }
                break;
            default:
                ShortString s(argv[i]);
                words.add(s);
        }
    }

    //FIXME:
//    if (!words.size()) return 1;
//    HashMap<ShortString, List<int>*> results(busca_imd_core::directUltraFastHash);
//
//    for (auto word : words) {
//        for (FileHashMap::Entry entry : *Index::getInstance().get(word)) {
//            List<int> * list;
//            try {
//                list = results.get(*entry.key);
//            } catch (int e) {
//                list = new List<int>;
//                results.put(*entry.key, list);
//                list->join(*entry.value, int_comparator);
//                continue;
//            }
//            if (exclusive) {
//                list->intersection(*entry.value, int_comparator);
//            } else {
//                list->join(*entry.value, int_comparator);
//            }
//        }
//    }
//
//    for (auto entry : results) {
//        std::cout << "Arquivo " << entry.key << std::endl;
//        for (int line : *entry.value) {
//            std::cout << "\tLinha " << line << std::endl;
//        }
//    }

//    if (command == COMMAND_SEARCH) {
//        switch (argv[1][2]) {
//            case 'A':
//
//                break;
//            case 'O':
//                break;
//        }
//    }



//    //search options
//    // <-bAND, -bOR> <-pA, -pC, -pI> <-tT, -tF> <key_words[]>
//    //if it hasn't any of the print options or time options
//    //-pI and -tF are the default arguments
//    if (strcmp(argv[1], "-bAND") == 0) {
//        //search using the bAND option
//        //---------------------------- pensar numa melhor maneira de filtrar esses próximos argumentos
//        //function to return the impression options (passing a argv[2] and returning the const Result::SortOrder)
//        //function to return the time option (passing a argv[3] and returning the const bool showTime)
//
//        //searchAnd(const Result::SortOrder order, const bool showTime, char ** words)
//    } else if (strcmp(argv[1], "-bOR") == 0) {
//        //search using the bOR option
//        //---------------------------- pensar numa melhor maneira de filtrar esses próximos argumentos
//        //function to return the impression options (passing a argv[2] and returning the const Result::SortOrder)
//        //function to return the time option (passing a argv[3] and returning the const bool showTime)
//
//        //searchOr(const Result::SortOrder order, const bool showTime, char ** words)
//    } else {
//        //print an message telling that the user inputed a wrong argument
//        //list all the possible arguments
//    }

    return 0;
}