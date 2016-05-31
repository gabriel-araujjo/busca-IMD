//
// Created by gabriel on 5/17/16.
//

#include "config.h"
#include <sys/stat.h>

using config::File;

File::File() {
    //verify if the file index exists or not, if not, create one empty
}

File::~File(){
    //delete the file index
}

void File::insertFile(char * filePath) {

}

void File::removeFile(char * fileName) {

}

void File::listFiles(char * order) {
    //will list the order from the files given an argument
}

bool File::needToUpdate(char * fileName) {
    //verify if the file have the modified date diferent from the modified date on the searchBase or if the file is in the searchBase

    //if the file isn't in the search base, return true

    //if the modified date is diferent from the one in the searchBase, return true
    //get last modified data
    struct stat attrib;
    //load the struct
    stat(fileName, &attrib);
    //get the last modified data
    //newInfos->lastMofified = attrib.st_ctime;

    return true;
}

//remove special characters from the words
char * File::removeSpecialCharacters(char * wordToTrim) {
    char * word_final = new char();
    int i, c;
    for(i = 0, c = 0; i < strlen(wordToTrim); i++){
        if(isalnum(wordToTrim[i])){
            word_final[c] = wordToTrim[i];
            c++;
        }
    }
    word_final[c] = '\0';

    return word_final;
}


/**
 * CONSTRUTOR
 * -->> LER no construtor do config, o indice (arquivo), se não tiver, criar um vazio;
 *
 * DESTRUIDOR
 * -->> SALVAR os indices e a lista de tpInfos no arquivo de indices e no indice_arquivo
 *
 * LISTA
 * -->> LISTA contendo tpInfos na classe config;
 *
 * -->> Funções para Adicionar e remover
 *          -->>Para adicionar: checar o arquivo na lista(indice_arquivo) para ver se precisa atualizar (Last modified data)
 *
 *          -->>Para remover: remover da lista(arquivo) e remover do indice(indice_arquivo) (hashMap(word, fileHashMap(fileName, linha)));
 *
 * -->> Sobreescrever operador == na struct;
 *
 *
 *
 *
 */


















