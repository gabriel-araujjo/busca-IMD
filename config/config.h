//
// Created by gabriel on 5/17/16.
//

#ifndef BUSCA_IMD_CONFIG_H
#define BUSCA_IMD_CONFIG_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>

#include "hash_map.h"
#include "iterator.h"
#include "short_string.h"

using namespace std;
using namespace core;

namespace config{
    //this class will read the files inputed by the user, add the properties of this file to the file index and
    //will populate the word hash map
    class File{
    private:
//        char * searchBasePath = (char*)"C:\\Users\\Pedro Paulo\\Desktop\\BUSCA-IMD\\config\\SearchBase.txt";

        typedef struct tpInfos{
            char * filePath;
            time_t lastMofified;
            int totalWords;
            HashMap<short_string, void *> mapOfWords;
        } * Infos;

        bool needToUpdate(char*);
        char * removeSpecialCharacters(char *);

    public:

        File();
        ~File();

        void insertFile(char*);
        void removeFile(char*);
        void listFiles(char*);
    };

}

#endif //BUSCA_IMD_CONFIG_H
