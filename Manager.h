#ifndef MAIN_CPP_MANAGER_H
#define MAIN_CPP_MANAGER_H

#include "SDL.h"
#include "string"
#include "Object.h"
#include "Sound.h"

enum enemyType {
    enemyBird,
    enemyBat,
    enemyKnight
};

enum stateEnemy {
    isDead,
    isAlive,
    isKilled
};

enum stateEnemyLocal {
    knightIdle = 19937,
    knightRunning = 98013
};

enum gameState {
    gameMenu,
    gamePlaying,
    gameEnded
};

enum gameBanner {
    logo,
    youLose,
    youWon
};

const int WINDOWSIZEW = 1000;
const int WINDOWSIZEH = 600;
const int MIDDLESCREEN = 300;
const int PLAYERX = 0;

int rnd(int l, int r);

class Manager {
public:
    Manager();

    ~Manager();

    void Init(const char *title, int x_pos, int y_pos, int width, int height, bool fullscreen);
    void HandleGameEvents();
    void UpdateGame();
    void RenderGame();
    void InitTexture();
    void HandleMenuEvents();
    void UpdateMenu();
    void RenderMenu();
    void HandleEndedEvents();
    void UpdateEnded();
    void RenderEnded();
    void CleanGame();
    void Clean();

    static SDL_Renderer *renderer;

//private:
    bool isRunning;
    int gameState;
    SDL_Window *window;

    Textures *testButton;
    Textures *gameLogo;
    Textures *startButton;
    Textures *optionsButton;
    Textures *exitButton;
    Textures *handPointer;
    Textures *gameOver;
    Textures *bars;
    Textures *healthBar;
    Textures *progressBar;

    int handPointerPos = 0;

    Textures *menuBackground;
    Textures *gameBackgroundSky;
    Textures *gameBackgroundLand;

    Sound *backgroundMusic;

    std::vector<Enemy *> enemies;

    static int playerHealth;
    static int oldPlayerHealth;
    static int maxHealth;
    static int totalEnemies;
    static int deceasedEnemies;
    static int oldDeceasedEnemies;
    static int debug1;
    static int debug2;
    Uint32 lastSpawnTime = 0;

    gameBanner banner;
};

extern Manager *game;

#endif //MAIN_CPP_MANAGER_H
