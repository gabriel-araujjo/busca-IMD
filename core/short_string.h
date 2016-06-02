//
// Created by Pedro Paulo on 30/05/2016.
//

#ifndef BUSCA_IMD_SHORT_STRING_H
#define BUSCA_IMD_SHORT_STRING_H

#include <stdint.h>

namespace core{
    class ShortString{
    public:
        char * value;
        uint16_t length;

        ShortString & operator=(const char * word);

        ShortString & operator=(const ShortString & word);

        bool operator==(const ShortString& ss) const;

        ShortString();
        ShortString(const char * word);
        ShortString(ShortString & other);
        ~ShortString();

    private:
        void copy(const ShortString& other);
    };
}


#endif //BUSCA_IMD_SHORT_STRING_H
