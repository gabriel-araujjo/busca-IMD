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
        std::cout << "Argumento invalido" << std::endl
                << "Os argumentos posseveis sao: " << std::endl
                << "Gerenciar base de arquivos: <opcao> <nome_arquivo>" << std::endl
                << "<opcao>" << std::endl
                << "-i" << " <nome_arquivo>" << " : inserir arquivo na base de busca" << std::endl
                << "-r" << " <nome_arquivo>" << " : remover arquivo na base de busca" << std::endl
                << "-li, -la, -lt" << " : listar arquivos que estão na base de busca" << std::endl
                << "\t-li" << " : listar na ordem em que os arquivos foram inseridos" << std::endl
                << "\t-la" << " : listar os arquivos em ordem alfabetica" << std::endl
                << "\t-lt" << " : listar os arquivos em ordem decrescente da quantidade de palavras em cada arquivo" << std::endl
                << std::endl
                << "Realizar uma busca: <opcao_busca> <opcao_impressao> <tempo> <palavras_chave>" << std::endl
                << "<opcao_busca> " << "-bAND, -bOR" << std::endl
                << "\t-bAND" << " : busca por linhas que contenham todas as palavras chave" << std::endl
                << "\t-bOR" << " : busca por linhas que contenham ao menos uma palavra chave" << std::endl
                << "<opcao_impressao> " << "-pA, -pC, -pI" << std::endl
                << "\t-pA" << " : resultados listados em ordem alfabetica do nome do arquivo" << std::endl
                << "\t-pC" << " : resultados listados em ordem decrescente do numero de vezes que a palavra foi \n\tencontrada no arquivo" << std::endl
                << "\t-pI" << " : resultados listados na ordem em que cada arquivo foi inserido na base" << std::endl
                << "<tempo> " << "-tF, -tT" << std::endl
                << "\t-tF" << " : nao sera exibido o tempo de execucao da busca" << std::endl
                << "\t-tT" << " : sera exibido o tempo de execucao da busca" << std::endl
                << "<palavras_chave>" << std::endl;
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