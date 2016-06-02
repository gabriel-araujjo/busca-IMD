//
// Created by Pedro Paulo on 31/05/2016.
//
#include <string.h>

#include "short_string.h"

using core::ShortString;

void ShortString::copy(const ShortString &other) {
    length = other.length;
    delete value;
    value = new char[length];
    strcpy(value, other.value);
}

ShortString::ShortString(){
    length = 0;
    value = nullptr;
}

ShortString::ShortString(const char * word){
    length = strlen(word);
    value = new char[length];
    strcpy(value, word);
}

ShortString::ShortString(ShortString & other){
    copy(other);
}

ShortString::~ShortString() {
    delete value;
}

ShortString &ShortString::operator=(const char * word) {
    length = (uint16_t)strlen(word);
    delete value;
    value = new char[length];
    strcpy(value, word);

    return *this;
}

ShortString &ShortString::operator=(const ShortString & word) {
    length = word.length;
    delete value;
    value = new char[length];
    strcpy(value, word.value);

    return *this;
}

bool ShortString::operator==(const ShortString& ss) const {
    return(ss.length == this->length) && (strcmp(ss.value, this->value) == 0);
}