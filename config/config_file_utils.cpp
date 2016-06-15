//
// Created by Pedro Paulo on 11/06/2016.
//
#if defined(WIN32) || defined(_WIN32)
#ifndef FILE_SEPARATOR
#define FILE_SEPARATOR "\\"
#endif //FILE_SEPARATOR
#include <fileapi.h>
#else //UNIX System
#ifndef FILE_SEPARATOR
#define FILE_SEPARATOR "/"
#endif //FILE_SEPARATOR
#endif //System choose

#include <iostream>
#include <sys/stat.h>
#include <string.h>

#include "config_file_utils.h"
#include "short_string.h"

using namespace std;
using busca_imd_core::roundUpToPowerOfTwo;

namespace busca_imd_config{
    char * getHomeDir() {
        char * home = nullptr;
#if defined(WIN32) || defined(_WIN32)
        char * homeDrive = getenv("HOMEDRIVE");
        char * homePath = getenv("HOMEPATH");
        if (homeDrive && homePath) {
            uint16_t pathLength = (uint16_t) (strlen(homeDrive) + strlen(homePath) + 1);
            roundUpToPowerOfTwo(pathLength);
            home = new char[pathLength];
            *home = '\0';
            strcat(home, homeDrive);
            strcat(home, homePath);
        }
#else
        char * homePath = getenv("HOME");
        if (homePath) {
            uint16_t pathLength = (uint16_t) (strlen(homePath) + 1);
            roundUpToPowerOfTwo(pathLength);
            home = new char[pathLength];
            strcpy(home, homePath);
        }
#endif
        return home;
    }

    bool createHiddenDir(const char * path){
#if defined(WIN32) || defined(_WIN32)
        if(!CreateDirectory(path, nullptr))
            return false;

        DWORD attr = GetFileAttributes(path);
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            return SetFileAttributes(path, attr | FILE_ATTRIBUTE_HIDDEN) != 0;
        }
        return true;
#else
        // create a dir with rwx rwx r-x permission
        return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 && errno != EEXIST;
#endif
    }

    bool dirExists(const char *path){
        struct stat info;

        return stat( path, &info ) == 0 && (info.st_mode & S_IFDIR);
    }

    bool fileExists (const char * path){
        struct stat info;

        return stat( path, &info ) == 0;
    }

    char * joinPath(char * parent, const char * file){
        uint16_t pathLength = (uint16_t) (strlen(parent) + strlen(FILE_SEPARATOR) + strlen(file));
        roundUpToPowerOfTwo(pathLength);
        char * joinedPath = new char[pathLength];
        strcpy(joinedPath, parent);
        strcat(joinedPath, FILE_SEPARATOR);
        strcat(joinedPath, file);
        return joinedPath;
    }

    const char *getFileSeparator() {
        return FILE_SEPARATOR;
    }


}



