//
// Created by gabriel on 5/17/16.
//
#define INDEX_FILE_PATH "C:\\TESTE\\index_file_imd.bin"
#define INDEX_WORD_PATH "C:\\TESTE\\index_words_imd.bin"


#include <sys/stat.h>
#include <fstream>
#include "config.h"
#include <unistd.h>

using config::Config;
using std::fstream;
using std::ios;
using core::ShortString;


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

    //read the index to fullfill the mInfoList
    readInfoList(INDEX_FILE_PATH);

    //read the index to fullfill the mIndex
    readIndex(INDEX_WORD_PATH);
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
    stat(fileName, &attrib);
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
ShortString Config::removeSpecialCharacters(ShortString wordToTrim) {
    char * word_final = new char();
    int i, c;
//    for(i = 0, c = 0; i < strlen(wordToTrim); i++){
//        if(isalnum(wordToTrim[i])){
//            word_final[c] = wordToTrim[i];
//            c++;
//        }
//    }
    word_final[c] = '\0';

    return word_final;
}

Config &Config::getInstance() {
    static Config instance;
    return instance;
}

void Config::readIndex(core::ShortString indexPath) {
    Array<ShortString *> filesPathArray;
    fstream wordFileIndex;

    wordFileIndex.open(indexPath.value, ios::binary);

    if(!wordFileIndex.is_open())
        return;

    uint16_t size = readUint16(wordFileIndex);

    filesPathArray.resize(size);

    for(unsigned int i = 0; i < size; i++){
        filesPathArray[i] = readShortString(wordFileIndex);
    }
}

core::ShortString * Config::readShortString(std::fstream & fileStream) {
    ShortString * shortString = new ShortString;

    fileStream.read((char *)shortString->length, sizeof(shortString->length));
    shortString->value = new char[shortString->length];
    fileStream.read(shortString->value, shortString->length);
    return shortString;
}

index::FileHashMap *Config::readFileHashMap(std::fstream &fileStream, Array<ShortString *> & filesLUT) {

    index::FileHashMap * fileHashMap = new index::FileHashMap;

    uint16_t size = readUint16(fileStream);

    for(unsigned int i = 0; i < size; i++){
        fileHashMap->put(filesLUT[readUint16(fileStream)], readListOfOcurrences(fileStream));
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


core::Array<core::ShortString *> Config::readFilesPathArray(std::fstream &fileStream) {

    return core::Array<ShortString *>();
}

core::List<int> * Config::readListOfOcurrences(std::fstream &fileStream) {

    core::List<int> * listOfOcurrences = new core::List<int>;

    uint32_t size = readUint32(fileStream);

    for(uint32_t i = 0; i < size; i++){
        uint32_t lineOfOcurrence = readUint32(fileStream);
        listOfOcurrences->add(lineOfOcurrence);
    }

    return listOfOcurrences;
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


















