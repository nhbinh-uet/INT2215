#include <cassert>
#include "Sound.h"
#include "SDL_mixer.h"

Sound::Sound(bool isShort, std::string path) {
    auto fp = fopen(path.c_str(), "r");
    assert(fp);
    fclose(fp);
    if(isShort) {
        sound = Mix_LoadWAV(path.c_str());
    } else {
        music = Mix_LoadMUS(path.c_str());
    }
    this->isShort = isShort;
}

Sound::~Sound() {
}

void Sound::play() {
    if(isShort) {
        Mix_PlayChannel(-1, sound, 0);
    } else {
        Mix_HaltMusic();
        Mix_PlayMusic(music, -1);
    }
}

void Sound::stop() {
    if(!isShort) {
        Mix_HaltMusic();
    }
}
