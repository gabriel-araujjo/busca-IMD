//
// Created by gabriel on 6/11/16.
//

#include <iosfwd>

#ifndef BUSCA_IMD_SERIALIZATION_UTILS_H
#define BUSCA_IMD_SERIALIZATION_UTILS_H

#include "short_string.h"

namespace busca_imd_io {
    busca_imd_core::ShortString * readShortString(std::istream & in);
    uint16_t readUint16(std::istream & in);
    uint32_t readUint32(std::istream &in);

    void writeShortString(std::ostream & out, busca_imd_core::ShortString * string);
    void writeUint16(std::ostream & out, uint16_t number);
    void writeUint32(std::ostream & out, uint32_t number);
}


#endif //BUSCA_IMD_SERIALIZATION_UTILS_H
