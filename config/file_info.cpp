//
// Created by gabriel on 6/12/16.
//

#define UTF8_0BYTE 0x00
#define UTF8_1BYTE 0x80
#define UTF8_2BYTES 0xC0
#define UTF8_3BYTES 0xE0
#define UTF8_4BYTES 0xF0
#define UTF8_5BYTES 0xF8

#include <time.h>
#include <sys/stat.h>
#include <stdint.h>
#include <locale>
#include <iostream>
#include <fstream>

#include "list.h"
#include "short_string.h"
#include "file_info.h"
#include "index.h"
#include "config_file_utils.h"

using busca_imd_core::List;
using busca_imd_index::Index;
using busca_imd_core::ShortString;
using busca_imd_core::directUltraFastHash;

namespace busca_imd_config {

    bool my_isalpha(uint32_t a) {
        return (a & 0xc3) == 0xc3 || (a >= 'a' && a <= 'z') || (a >= 'A' && a <='Z');
    }

    void toUpperCase(uint32_t &a) {
        if ((a & 0xa0c3) == 0xa0c3 ) {
            a &= 0x9fc3;
        }
        if (a >= 'a' && a <= 'z') {
            a &= 0x5F;
        }
    }

    FileInfo::FileInfo() { }
    FileInfo::FileInfo(FileInfo const & other) :
            filePath(other.filePath), totalWords(other.totalWords), lastModified(other.lastModified) {

    }

    FileInfo& FileInfo::operator=(FileInfo const & other) {
        filePath = other.filePath;
        totalWords = other.totalWords;
        lastModified = other.lastModified;
        return *this;
    }

    bool FileInfo::operator==(FileInfo const &other) {
        return filePath == other.filePath;
    }

    FileInfo::FileInfo(ShortString const & file) :
            filePath(file), totalWords(0), lastModified(0) {
        char * filePath_charArr = filePath.asCharArray();
        std::cout << std::endl << "checking existency of file" << filePath_charArr << std::endl;
        if (fileExists(filePath_charArr)) {
            lastModified = getFileLastModifiedAttr(filePath_charArr);
            std::cout << std::endl << "reading words " << std::endl;
            readWords(filePath_charArr);
            delete filePath_charArr;
        } else {
            delete filePath_charArr;
            throw FILE_NOT_FOUND;
        }
    }

    bool FileInfo::updateIndex() {
        char * filePath_charArr = filePath.asCharArray();
        time_t lastFileModification = getFileLastModifiedAttr(filePath_charArr);
        if (lastModified < lastFileModification) {

            Index::getInstance().removeFile(filePath);
            totalWords = 0;
            lastModified = lastFileModification;
            readWords(filePath_charArr);
            delete filePath_charArr;
            return true;
        }
        delete filePath_charArr;
        return false;
    }

    time_t FileInfo::getFileLastModifiedAttr(char * filePath_charArr) {
        struct stat attr;
        stat(filePath_charArr, &attr);
        return attr.st_ctim.tv_sec;
    }

    void FileInfo::readWords(char * filePath_charArr) {
        std::fstream in;
        in.open(filePath_charArr, std::ios::in);
        if (!in.is_open()) {
            return;
        }
        ShortString word;
        List<char> word_charList;
        uint32_t letter;
        uint8_t letterSize;
        uint16_t line = 0;
        while (!in.eof()) {
            letterSize = readUtf8Char(in, letter);

            char * l = (char *) &letter;
            if (my_isalpha(letter)) {
                toUpperCase(letter);
                for (; letterSize; --letterSize, ++l) {
                    word_charList.add(*l);
                }
                continue;
            }

            if (word_charList.size() > 0) {
                word = word_charList;
                Index::getInstance().addEntry(filePath, word, line);
                totalWords++;
                word_charList.clear();
            }

            if (*l == '\n') {
                line++;
            }
        }
        if (word_charList.size() > 0) {
            word = word_charList;
            Index::getInstance().addEntry(filePath, word, line);
            totalWords++;
            word_charList.clear();
        }
        std::cout << "fim da leitura" << std::endl;
    }

    uint8_t FileInfo::readUtf8Char(std::istream &input, uint32_t &dest) {
        uint8_t size = 0;
        dest = 0;
        char * letter = (char *) &dest;
        input.read(letter, 1);
        if ((*letter & UTF8_1BYTE) == UTF8_0BYTE) {
            size = 1;
        } else if ((*letter & UTF8_3BYTES) == UTF8_2BYTES) {
            size = 2;
        } else if ((*letter & UTF8_4BYTES) == UTF8_3BYTES) {
            size = 3;
        } else if ((*letter & UTF8_5BYTES) == UTF8_4BYTES) {
            size = 4;
        }

        for (int i = 1; i < size; i++) {
            input.read(&letter[i], 1);
        }

        return size;
    }


    std::ostream &operator<<( std::ostream &output,
                              const busca_imd_config::FileInfo &fileInfo ) {
        output << fileInfo.filePath;
        output.write((char *) &fileInfo.lastModified, sizeof(time_t));
        output.write((char *) &fileInfo.totalWords, sizeof(int));
        return output;
    }

    std::istream &operator>>( std::istream &input,
                              busca_imd_config::FileInfo &fileInfo ) {
        input >> fileInfo.filePath;
        input.read((char*) &fileInfo.lastModified, sizeof(time_t));
        input.read((char*) &fileInfo.totalWords, sizeof(int));
        return input;
    }
}