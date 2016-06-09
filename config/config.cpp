//
// Created by gabriel on 5/17/16.
//
#define INDEX_FILE_PATH "C:\\TESTE\\index_file_imd.bin"
#define INDEX_WORD_PATH "C:\\TESTE\\index_words_imd.bin"


#include <sys/stat.h>
#include <fstream>
#include <unistd.h>

#include "config.h"
#include "list.h"

using busca_imd_core::Array;
using busca_imd_config::Config;
using busca_imd_core::List;
using std::fstream;
using std::ios;
using busca_imd_core::ShortString;


Config::Config() {
    //verify if the file index exists or not, if not, create one empty
    //struct buffer to verify if the file already exists or not
    struct stat buffer1;
    struct stat buffer2;
    //verify if the file exists
    if((stat(INDEX_FILE_PATH, &buffer1) != 0) ||
            stat(INDEX_WORD_PATH, &buffer2) != 0){

        //create the file in binary mode
        fstream indexFile;
        indexFile.open(INDEX_FILE_PATH, ios::binary);
        indexFile.close();

        fstream indexWords;
        indexWords.open(INDEX_WORD_PATH, ios::binary);
        indexWords.close();
    }

    ShortString filePath(INDEX_FILE_PATH);
    //read the index to fullfill the mInfoList
    readInfoList(filePath);

    //read the index to fullfill the mIndex
    filePath = INDEX_WORD_PATH;
    readIndex(filePath);
}

void Config::insertOrUpdateFile(ShortString filePath) {
    //it will read the file_index, verify if need to update, if the file isn't on the index or needs to update, insert
    //the file on the file_index and words_index
}

void Config::removeFile(ShortString fileName) {

}

void Config::listFiles(ShortString order) {
    //will list the order from the files given an argument
}

bool Config::needToUpdate(ShortString fileName) {
    //verify if the file have the modified date diferent from the modified date on the searchBase or if the file is in the searchBase
    //if the file isn't in the search base, return true
    //if the modified date is diferent from the one in the searchBase, return true
    //get last modified data
    struct stat attrib;
    //load the struct
    stat(fileName.asCharArray(), &attrib);
    //get the last modified data
//    if(attrib.st_ctime != /*lastModifiedDate from the file_index*/){
//        //update
//        return true;
//    }//otherwise, the file is the same, the index is already created, no need to update

    //return false;

    //returning true for test purposes
    return true;
}

//remove special characters from the words
void Config::removeSpecialCharacters(ShortString & wordToTrim) {
    List<char> trimmedCharList;
    const char * rawWord = wordToTrim.asCharArray();
    const uint16_t length = wordToTrim.length();
    char c;
    int i;

    for(i = 0, c = rawWord[0]; i < length; ++i, c = rawWord[i]) {
        if (isalnum(c)) {
            trimmedCharList.add(c);
        }
    }

    wordToTrim = trimmedCharList;
}

Config &Config::getInstance() {
    static Config instance;
    return instance;
}

void Config::readIndex(busca_imd_core::ShortString indexPath) {
    Array<ShortString *> filesPathArray;
    fstream wordFileIndex;

    wordFileIndex.open(indexPath.asCharArray(), ios::binary);

    if(!wordFileIndex.is_open())
        return;

    uint16_t size = readUint16(wordFileIndex);

    filesPathArray.resize(size);

    for(unsigned int i = 0; i < size; i++){
        filesPathArray[i] = readShortString(wordFileIndex);
    }
}

busca_imd_core::ShortString * Config::readShortString(std::fstream & fileStream) {
    uint16_t length = readUint16(fileStream);
    char * value = new char(length);
    fileStream.read(value, length);

    ShortString * result = new ShortString(value);
    delete value;
    return result;
}

busca_imd_index::FileHashMap *Config::readFileHashMap(std::fstream &fileStream, Array<ShortString *> & filesLUT) {

    busca_imd_index::FileHashMap * fileHashMap = new busca_imd_index::FileHashMap;

    uint16_t size = readUint16(fileStream);

    for(unsigned int i = 0; i < size; i++){
        fileHashMap->put(filesLUT[readUint16(fileStream)], readListOfOccurrences(fileStream));
    }

    return fileHashMap;
}

uint16_t Config::readUint16(std::fstream &fileStream) {
    uint16_t value;
    fileStream.read((char*)&value, sizeof(uint16_t));

    return value;
}
uint32_t Config::readUint32(std::fstream &fileStream) {
    uint32_t value;
    fileStream.read((char*)&value, sizeof(uint32_t));

    return value;
}


busca_imd_core::Array<busca_imd_core::ShortString *> Config::readFilesPathArray(std::fstream &fileStream) {

    return busca_imd_core::Array<ShortString *>();
}

busca_imd_core::List<int> * Config::readListOfOccurrences(std::fstream &fileStream) {

    busca_imd_core::List<int> * listOfOccurrences = new busca_imd_core::List<int>;

    uint32_t size = readUint32(fileStream);

    for(uint32_t i = 0; i < size; i++){
        uint32_t lineOfOccurrence = readUint32(fileStream);
        listOfOccurrences->add(lineOfOccurrence);
    }

    return listOfOccurrences;
}

void Config::readInfoList(busca_imd_core::ShortString infoListPath) {

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
 * /////PARA LEMBRAR COMO ESCREVE E LÊ EM UM ARQUIVO BINARIO
// * //verify if the file is open
//        if(indexFile.is_open()){
//            //will run through the file till the end of the file
//            char * teste_escrita = "Teste";
//            //write on the file
//            //element to write on the file, the size of that element in bytes
//            indexFile.write(teste_escrita, sizeof(teste_escrita));
//            //close the file
//            indexFile.close();
//        }
 *
 *
// * indexFile.open("C:\\TESTE\\index_file_imd.bin", ios::binary | ios::in);
//    if(indexFile.is_open()){
//        char * teste_leitura = new char();
//        //element where we will store the data read, the size in bytes that we will read from the file
//        indexFile.read(teste_leitura, sizeof(teste_leitura));
//
//        cout << teste_leitura << endl;
//    }
 *
 *
 *
 */


















