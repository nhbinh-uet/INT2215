#include <iostream>
#include "Manager.h"
#include "Dictionary.h"

Manager *game;

int main(int argc, char *args[]) {
    game = new Manager();

    game->Init("Typing game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, false);
    game->InitTexture();
    dict.InitDictionary();

    while(game->isRunning) {
        if(game->gameState == gameMenu) {
            game->HandleMenuEvents();
            game->UpdateMenu();
            game->RenderMenu();
        } else if(game->gameState == gamePlaying) {
            game->HandleGameEvents();
            game->UpdateGame();
            game->RenderGame();
        } else if(game->gameState == gameEnded) {
            game->RenderEnded();
        }
    }

    game->Clean();

    return 0;
}
