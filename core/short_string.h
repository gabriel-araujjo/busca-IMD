//
// Created by Pedro Paulo on 30/05/2016.
//

#ifndef BUSCA_IMD_SHORT_STRING_H
#define BUSCA_IMD_SHORT_STRING_H

#include <iosfwd>
#include <stdint.h>
#include "list.h"

namespace busca_imd_core{

    bool isAlphaCharacter(uint32_t a);

    void toUpperCase(uint32_t &a);

    uint8_t readUtf8Char(std::istream &input, uint32_t &dest);

    class ShortString;

    uint32_t ultraFastHash(ShortString * const & key);
    uint32_t directUltraFastHash(ShortString const & key);


    class ShortString{
    private:
        char * mValue;
        uint16_t mLength;

        void copy(const char * const & other);
        void copy(ShortString const & other);
        void copy(List<char> const & other);
    public:

        ShortString();
        ~ShortString();

        ShortString(ShortString const & other);

        ShortString(List<char> const & list);

        ShortString(char const * word);

        ShortString &operator=(ShortString const & other);

        ShortString &operator=(List<char> const & list);

        ShortString &operator=(const char * const word);

        bool operator==(ShortString const & ss) const;

        bool operator==(const char * const other) const;

        bool operator!=(ShortString const & ss) const;

        bool operator!=(const char * const other) const;

        const uint16_t length() const;

        char * asCharArray(char * target = nullptr) const;

        int compare(ShortString const & ss) const;

        ShortString toUpperCase() const;

        friend std::ostream  &operator<<( std::ostream &output,
                                    const ShortString &ss );

        friend std::istream  &operator>>( std::istream &input,
                                          ShortString &ss );

        friend uint32_t directUltraFastHash(ShortString const & key);

    };
}





#endif //BUSCA_IMD_SHORT_STRING_H
