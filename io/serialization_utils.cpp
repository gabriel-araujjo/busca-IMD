//
// Created by gabriel on 6/11/16.
//
#include <iosfwd>

#include "serialization_utils.h"

using busca_imd_core::ShortString;
using std::istream;
using std::ostream;

namespace busca_imd_io {
    ShortString * readShortString(std::istream & in) {
        uint16_t length = readUint16(in);
        char * value = new char(length);
        in.read(value, length);

        ShortString * result = new ShortString(value);
        delete value;
        return result;
    }

    uint16_t readUint16(std::istream & in) {
        uint16_t value;
        in.read((char*)&value, sizeof(uint16_t));

        return value;
    }

    uint32_t readUint32(std::istream &in) {
        uint32_t value;
        in.read((char*)&value, sizeof(uint32_t));
        return value;
    }

    void writeShortString(std::ostream & out, busca_imd_core::ShortString * string) {
        writeUint16(out, string->length());
        //TODO, get string data
    }

    void writeUint16(std::ostream & out, uint16_t number) {
        out.write((char*) &number, sizeof(uint16_t));
    }

    void writeUint32(std::ostream & out, uint32_t number) {
        out.write((char*) &number, sizeof(uint32_t));
    }
}