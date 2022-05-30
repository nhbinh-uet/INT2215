#include <vector>
#include <cstdio>
#include <iostream>
#include "TextureLoader.h"
#include "Manager.h"

static const std::vector<SDL_Color> colorPalette = {
    {171, 171, 171},
    {4,   84,  89},
    {8,   115, 83},
    {21,  194, 134},
    {171, 217, 109},
    {251, 191, 84},
    {238, 107, 59},
    {236, 15,  71},
    {160, 44,  93},
    {112, 4,   96}
};

SDL_Texture *TextureLoader::LoadTexture(const char *fileName) {
    auto file = fopen(fileName, "r");
    if(!file) {
        std::cout << "Cannot open file: " << fileName << "\n";
        exit(0);
    }
    fclose(file);
    SDL_Surface *tmp = IMG_Load(fileName);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Manager::renderer, tmp);
    SDL_FreeSurface(tmp);
    return tex;
}

std::vector<SDL_Texture *> TextureLoader::GetAnimation(const std::string name) {
    std::vector<SDL_Texture *> ret;
    auto file = fopen(("resources/" + name + std::to_string(0) + ".png").c_str(), "r");
    if(!file) {
        std::cout << "Cannot open file: resources/" + name + std::to_string(0) + ".png" << "\n";
        exit(0);
    }

    for(int i = 0; file; ++i) {
        ret.push_back(TextureLoader::LoadTexture(("resources/" + name + std::to_string(i) + ".png").c_str()));
        fclose(file);
        file = fopen(("resources/" + name + std::to_string(i + 1) + ".png").c_str(), "r");
    }
    fclose(file);
    return ret;
}

SDL_Texture *TextureLoader::LoadText(const std::string text, int size, int colorID) {
    TTF_Font *font = TTF_OpenFont("font/pixel2.ttf", size);
    TTF_Font *font_outline = TTF_OpenFont("font/pixel2.ttf", size);
    TTF_SetFontOutline(font_outline, 4);
    SDL_Color color = colorPalette[colorID];
    SDL_Color black = {0x00, 0x00, 0x00};
    SDL_Surface *bg_surface = TTF_RenderText_Blended(font_outline, text.c_str(), black);
    SDL_Surface *fg_surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Rect rect = {4, 4, fg_surface->w, fg_surface->h};
    SDL_SetSurfaceBlendMode(fg_surface, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(fg_surface, nullptr, bg_surface, &rect);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Manager::renderer, bg_surface);
    SDL_FreeSurface(bg_surface);
    SDL_FreeSurface(fg_surface);
    TTF_CloseFont(font);
    TTF_CloseFont(font_outline);
    return texture;
}
