#ifndef MAIN_CPP_MUSIC_H
#define MAIN_CPP_MUSIC_H

#include <SDL_mixer.h>
#include <string>

class Sound {
public:
    Sound(bool isShort, std::string path);
    ~Sound();

    void play();
    void stop();

    bool isShort;
    Mix_Music *music = nullptr;
    Mix_Chunk *sound = nullptr;
};

#endif //MAIN_CPP_MUSIC_H
