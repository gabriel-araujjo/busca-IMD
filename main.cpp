// trata os comandos do usuário

#define COMMAND_INSERT 'i'
#define COMMAND_REMOVE 'r'
#define COMMAND_LIST 'l'
#define COMMAND_SEARCH 'b'

#define SEARCH_ORDER_ALPHABETICAL 'A'
#define SEARCH_ORDER_OCCURRENCES 'C'
#define SEARCH_ORDER_INSERTION 'I'

#include <stdlib.h>
#include <string>
#include "config.h"

using busca_imd_config::Config;
using busca_imd_config::FileInfo;
using busca_imd_core::ShortString;
using busca_imd_core::List;
using busca_imd_core::HashMap;
using busca_imd_index::Index;
using busca_imd_index::SearchResult;
using busca_imd_index::FileHashMap;
using busca_imd_index::SearchParams;

void readSearchParams(int argc, char ** argv, SearchParams & searchParams);

void checkIsValidArgs(int argc, char ** argv);

int main(int argc, char ** argv) {

    const clock_t begin = clock();

    checkIsValidArgs(argc, argv);

    // check whether command is a config command
    switch (argv[1][1]) {
        case COMMAND_INSERT:
            return Config::insertFiles(argc, argv);
        case COMMAND_REMOVE:
            return Config::removeFiles(argc, argv);
        case COMMAND_LIST:
            return Config::listFiles(argc, argv);
        default:
            break;
    }

    Config::loadIndex();

    SearchParams searchParams;
    readSearchParams(argc, argv, searchParams);
    if (!searchParams.words.size()) return 1;

    SearchResult * result = Index::getInstance().search(searchParams);

    char filePath_charArray[4000];
    for (auto entry : *result) {
        entry.key.asCharArray(filePath_charArray);
        std::ifstream in(filePath_charArray, std::ios::in);
        if (!in.is_open() || entry.value->size() == 0) {
            continue;
        }
        if (entry.value->size() == 1) {
            std::cout << ">> Foi encontrada 1 linha";
        } else {
            std::cout << ">> Foram encontradas " << entry.value->size() << " linhas";
        }

        std::cout << " no arquivo \"" << filePath_charArray << "\":" << std::endl;

        int currentLine = 0;
        std::string line;
        for (int lineNumber : *entry.value) {
            while (currentLine <= lineNumber) {
                std::getline(in, line);
                if (currentLine == lineNumber) {
                    std::cout << "  - linha " << (lineNumber + 1) << ": \"" << line << "\"" << std::endl;
                }
                currentLine++;
            }
        }
        std::cout << std::endl;
    }

    return 0;
}

void checkIsValidArgs(int argc, char ** argv) {
    if (argc < 2 || argv[1][0] != '-') {
        //show help
        exit(1);
    }
}

void readSearchParams(int argc, char **argv, SearchParams &searchParams) {
    searchParams.exclusive = false;
    searchParams.showTime = false;
    searchParams.order = SEARCH_ORDER_INSERTION;
    for (int i = 1; i < argc; ++i) {
        switch (*argv[i]) {
            case '-':
                switch (argv[i][1]) {
                    case 'b':
                        searchParams.exclusive = argv[i][2] == 'A';
                        break;
                    case 't':
                        searchParams.showTime = argv[i][2] == 'T';
                        break;
                    case 'p':
                        searchParams.order = argv[i][2];
                        break;
                    default:
                        break;
                }
                break;
            default:
                searchParams.words.add(ShortString(argv[i]).toUpperCase());
        }
    }
}