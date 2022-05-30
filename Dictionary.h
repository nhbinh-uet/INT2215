#ifndef MAIN_CPP_DICTIONARY_H
#define MAIN_CPP_DICTIONARY_H

#include <iostream>
#include "TextureLoader.h"

class Dictionary {
public:
    void InitDictionary();

    static std::vector<SDL_KeyCode> typeList;

    std::vector<std::string> dictionary;
    std::vector<std::string> wordlength[10];

    std::string WordLength(int u, int v);
    std::string LetterWordLength(int letter, int u, int v);
};

extern Dictionary dict;

#endif //MAIN_CPP_DICTIONARY_H
