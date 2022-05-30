#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits.h>
#include "SDL_mixer.h"
#include "Manager.h"
#include "TextureLoader.h"
#include "Dictionary.h"

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
std::vector<std::string> convertBanner = {
    "resources/game-logo.png",
    "resources/you-lose.png",
    "resources/you-won.png"
};

int rnd(int l, int r) {
    return l + rng() % (r - l + 1);
}

SDL_Renderer *Manager::renderer = nullptr;
int Manager::playerHealth = 100;
int Manager::oldPlayerHealth = 100;
int Manager::debug1 = 0;
int Manager::debug2 = 0;
int Manager::maxHealth;
int Manager::totalEnemies;
int Manager::deceasedEnemies;
int Manager::oldDeceasedEnemies;


Manager::Manager() {
    TTF_Init();
    isRunning = true;
    banner = logo;
    gameState = gameMenu;
    maxHealth = 100;
    totalEnemies = 20;
    deceasedEnemies = oldDeceasedEnemies = 0;
}

void Manager::HandleGameEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
        case SDLK_UP:
            debug1++;
            break;
        case SDLK_DOWN:
            debug1--;
            break;
        case SDLK_LEFT:
            debug2--;
            break;
        case SDLK_RIGHT:
            debug2++;
            break;
        }
    default:
        break;
    }
    for(auto enemy : enemies)
        enemy->word->HandleEvent(event);
}

void Manager::UpdateGame() {
    gameBackgroundSky->Left(50);
    if(gameBackgroundSky->destR.x + WINDOWSIZEW == 0) {
        gameBackgroundSky->destR.x = 0;
    }

    /// - | Monster spawning
    Uint32 currentTime = SDL_GetTicks();
    if(currentTime >= lastSpawnTime + 1500 && enemies.size() >= 0 || !rnd(0, 2000)) {
        enemies.emplace_back();
        int random = rnd(0, 2);
        enemies.back() = new Enemy(random);
        lastSpawnTime = currentTime;
    }

    /// -| Clear completed monsters
    for(int i = (int) enemies.size() - 1; i >= 0; i--) {
        enemies[i]->StateUpdate();
        if(enemies[i]->state == isDead) {
            Sound *hit = new Sound(true, !enemies[i]->completed ? "sound/hit.wav" : "sound/kill.wav");
            hit->play();
            deceasedEnemies += enemies[i]->completed;
            std::swap(enemies[i], enemies.back());
            enemies.pop_back();
        }
    }

    for(auto enemy : enemies)
        enemy->FrameUpdate();

    healthBar->SetDestR(24 + 232 - (maxHealth - playerHealth) * 232 / maxHealth, 555,
                        (maxHealth - playerHealth) * 232 / maxHealth, 12);
    progressBar->SetDestR(24 + deceasedEnemies * 232 / totalEnemies, 573,
                          232 - deceasedEnemies * 232 / totalEnemies, 12);

    if(playerHealth <= 0) {
        gameState = gameMenu;
        backgroundMusic->play();
        banner = youLose;
        playerHealth = oldPlayerHealth = 50;
        CleanGame();
    }

    if(deceasedEnemies == totalEnemies) {
        gameState = gameMenu;
        backgroundMusic->play();
        banner = youWon;
        playerHealth = 50;
        CleanGame();
    }
}

void Manager::RenderGame() {
    SDL_RenderClear(renderer);

    gameBackgroundSky->Render();
    gameBackgroundLand->Render();

    for(auto enemy : enemies)
        enemy->Render();
    for(auto enemy : enemies)
        enemy->word->RenderWord();

    bars->Render();
    healthBar->Render();
    progressBar->Render();

    SDL_RenderPresent(renderer);
}

void Manager::InitTexture() {
    gameLogo = new Textures();
    gameLogo->SetAnimation(TextureLoader::LoadTexture(convertBanner[banner].c_str()));
    gameLogo->SetDestR(300, -50, 400, 400);

    menuBackground = new Textures();
    menuBackground->SetAnimation(TextureLoader::GetAnimation("menu-background-"));
    menuBackground->SetAnimationDelay(120);
    menuBackground->SetDestR(0, 0, WINDOWSIZEW, WINDOWSIZEH);

    gameBackgroundSky = new Textures();
    gameBackgroundSky->SetAnimation(TextureLoader::LoadTexture("resources/game-background-sky.png"));
    gameBackgroundSky->SetDestR(0, 0, WINDOWSIZEW * 2, WINDOWSIZEH);

    gameBackgroundLand = new Textures();
    gameBackgroundLand->SetAnimation(TextureLoader::LoadTexture("resources/game-background-land.png"));
    gameBackgroundLand->SetDestR(0, 0, WINDOWSIZEW * 2, WINDOWSIZEH);

    startButton = new Textures();
    startButton->SetAnimation(TextureLoader::LoadTexture("resources/menu-button.png"));
    startButton->SetSrcR(0, 0, 625, 125);
    startButton->SetDestR(422, 100 + 250, 156, 31);

    optionsButton = new Textures();
    optionsButton->SetAnimation(TextureLoader::LoadTexture("resources/menu-button.png"));
    optionsButton->SetSrcR(0, 125, 625, 162);
    optionsButton->SetDestR(422, 140 + 250, 156, 40);

    exitButton = new Textures();
    exitButton->SetAnimation(TextureLoader::LoadTexture("resources/menu-button.png"));
    exitButton->SetSrcR(0, 287, 625, 125);
    exitButton->SetDestR(422, 180 + 250, 156, 31);

    handPointer = new Textures();
    handPointer->SetAnimation(TextureLoader::LoadTexture("resources/hand-pointer.png"));
    handPointer->SetDestR(590, 355, 30, 25);

    gameOver = new Textures();
    gameOver->SetAnimation(TextureLoader::LoadTexture("resources/game-over.png"));
    gameOver->SetDestR(0, 0, WINDOWSIZEW, WINDOWSIZEH);

    bars = new Textures();
    bars = new Textures();
    bars->SetAnimation(TextureLoader::LoadTexture("resources/health-bar.png"));
    bars->SetDestR(20, 550, 240, 40);

    healthBar = new Textures();
    healthBar->SetAnimation(TextureLoader::LoadTexture("resources/white.png"));
    healthBar->SetDestR(24, 555, 232, 12);

    progressBar = new Textures();
    progressBar->SetAnimation(TextureLoader::LoadTexture("resources/white.png"));
    progressBar->SetDestR(24, 573, 232, 12);
}

void Manager::Init(const char *title, int x_pos, int y_pos, int width, int height, bool fullscreen) {
    int isFullScreen = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    if(!SDL_Init(SDL_INIT_EVERYTHING)) {
        window = SDL_CreateWindow(title, x_pos, y_pos, width, height, isFullScreen);
        Manager::renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(Manager::renderer, 255, 255, 255, 255);
    } else {
        std::cout << "Failed to load SDL!\n";
        exit(0);
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
    }

    backgroundMusic = new Sound(false, "sound/background.wav");
    backgroundMusic->play();
}

void Manager::HandleMenuEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
        case SDLK_UP:
            (handPointerPos += 2) %= 3;
            break;
        case SDLK_DOWN:
            (handPointerPos += 1) %= 3;
            break;
        case SDLK_RETURN:
            switch(handPointerPos) {
            case 0:
                gameState = gamePlaying;
                backgroundMusic->stop();
                maxHealth = 100;
                playerHealth = oldPlayerHealth = maxHealth;
                totalEnemies = 20;
                deceasedEnemies = oldDeceasedEnemies = 0;
                banner = logo;
                break;
            case 1:
                break;
            case 2:
                isRunning = false;
                break;
            }
            break;
        }
    default:
        break;
    }
}

int dir = 1;

void Manager::UpdateMenu() {
    Uint32 currentTime = SDL_GetTicks();
    if(gameLogo->destR.y == 10)
        dir = -1;
    if(gameLogo->destR.y == -50)
        dir = 1;
    if(currentTime >= gameLogo->lastUsedTime + 40) {
        gameLogo->destR.y += dir;
        gameLogo->lastUsedTime = currentTime;
    }
    handPointer->destR.y = handPointerPos * 40 + 355;
    menuBackground->UpdateAnimation();
}

void Manager::RenderMenu() {
    SDL_RenderClear(renderer);
    menuBackground->RenderAnimation();
    gameLogo->Render();
    startButton->Render(1);
    optionsButton->Render(1);
    exitButton->Render(1);
    handPointer->Render();
    SDL_RenderPresent(renderer);
}

void Manager::Clean() {
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void Manager::CleanGame() {
    gameState = gameMenu;
    playerHealth = oldPlayerHealth = maxHealth;
    backgroundMusic->play();
    enemies.clear();
    healthBar->SetDestR(24, 555, 232, 12);
    healthBar->SetDestR(24, 573, 232, 12);
    gameLogo = new Textures();
    gameLogo->SetAnimation(TextureLoader::LoadTexture(convertBanner[banner].c_str()));
    gameLogo->SetDestR(300, -50, 400, 400);
}

void Manager::RenderEnded() {
    SDL_RenderClear(renderer);
//    for(auto enemy : enemies)
//        enemy->Render();
//
//    for(auto enemy : enemies)
//        enemy->word->RenderWord();
//
//    gameOver->Render();

    SDL_RenderPresent(renderer);
}
