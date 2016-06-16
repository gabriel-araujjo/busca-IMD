//
// Created by gabriel on 6/12/16.
//

#ifndef BUSCA_IMD_FILE_INFO_H
#define BUSCA_IMD_FILE_INFO_H

#ifndef FILE_NOT_FOUND
#define FILE_NOT_FOUND 2
#endif

#include <iosfwd>
#include <time.h>

#include "short_string.h"
#include "hash_map.h"

namespace busca_imd_config {
    class FileInfo {
    private:
        time_t getFileLastModifiedAttr(char * file);
        void readWords(char * file);
    public:
        busca_imd_core::ShortString filePath;
        time_t lastModified;
        time_t insertionDate;
        int totalWords;

        FileInfo();
        FileInfo(FileInfo const & other);
        FileInfo(busca_imd_core::ShortString const & file);
        bool updateIndex();
        FileInfo &operator = (FileInfo const & other);
        bool operator==(FileInfo const & other);


        friend std::ostream &operator<<( std::ostream &output,
                                         const busca_imd_config::FileInfo &fileInfo );

        friend std::istream &operator>>( std::istream &input,
                                         busca_imd_config::FileInfo &fileInfo );
    };

}


#endif //BUSCA_IMD_FILE_INFO_H
