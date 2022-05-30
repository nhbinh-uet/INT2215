#ifndef MAIN_CPP_TEXTURELOADER_H
#define MAIN_CPP_TEXTURELOADER_H

#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

class TextureLoader {
public:
    static SDL_Texture *LoadTexture(const char *fileName);
    static std::vector<SDL_Texture *> GetAnimation(const std::string name);
    static SDL_Texture *LoadText(std::string text, int size, int colorID = 0);
    static SDL_Texture *LoadChar(const char *text, int size, int colorID = 0);
};

#endif //MAIN_CPP_TEXTURELOADER_H
