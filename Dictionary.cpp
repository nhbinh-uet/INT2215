#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Dictionary.h"
#include "Manager.h"

std::vector<SDL_KeyCode> Dictionary::typeList = {
    SDLK_a, SDLK_b, SDLK_c, SDLK_d,
    SDLK_e, SDLK_f, SDLK_g, SDLK_h,
    SDLK_i, SDLK_j, SDLK_k, SDLK_l,
    SDLK_m, SDLK_n, SDLK_o, SDLK_p,
    SDLK_q, SDLK_r, SDLK_s, SDLK_t,
    SDLK_u, SDLK_v, SDLK_w, SDLK_x,
    SDLK_y, SDLK_z
};

Dictionary dict;

void Dictionary::InitDictionary() {
    auto file = fopen("words.txt", "r");
    if(file) {
        std::string tmp;
        std::ifstream cin("words.txt");
        while(cin >> tmp) {
            dictionary.push_back(tmp);
            if(tmp.size() <= 9)
                wordlength[tmp.size()].push_back(tmp);
        }
        std::sort(dictionary.begin(), dictionary.end(), [](std::string i, std::string j) {
            return i.size() < j.size();
        });
        cin.close();
    } else {
        std::cout << "Dictionary data not found!";
        exit(0);
    }
    fclose(file);
}

std::string Dictionary::WordLength(int u, int v) {
    int countWord = 0;
    for(int i = u; i <= v; i++)
        countWord += wordlength[i].size();
    int temp = rnd(1, countWord);
    for(int i = u; i <= v; i++) {
        if(temp <= wordlength[i].size())
            return wordlength[i][temp - 1];
        else
            temp -= wordlength[i].size();
    }
}

std::string Dictionary::LetterWordLength(int letter, int u, int v) {
    std::string res = "";
    bool alphabet[30];
    for(int i = 0; i <= 25; i++)
        alphabet[i] = 0;

    std::vector<int> letters;
    while(letters.size() < letter) {
        int temp = rnd(0, 25);
        if(alphabet[temp] == 0)
            alphabet[temp] = 1, letters.push_back(temp);
    }

    int length = rnd(u, v);
    for(int i = 1; i <= length; i++) {
        int temp2 = rnd(1, letter);
        res += (char) letters[temp2 - 1] + 'a';
    }
    return res;
}
