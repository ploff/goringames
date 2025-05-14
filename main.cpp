#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "include/GameScene.h"
#include "include/MainMenuScene.h"
#include "include/SceneManager.h"

#define NAME "axe dev build"
#define LOGICAL_WIDTH 446
#define LOGICAL_HEIGHT 253

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

void close() {
    if (gRenderer) {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = nullptr;
    }
    if (gWindow) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }


    gWindow = SDL_CreateWindow(NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LOGICAL_WIDTH, LOGICAL_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!gWindow) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
        SDL_Quit();
        return false;
    }


    if (SDL_RenderSetLogicalSize(gRenderer, LOGICAL_WIDTH, LOGICAL_HEIGHT) < 0) {
        std::cerr << "Failed to set logical size! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gRenderer = nullptr;
        gWindow = nullptr;
        SDL_Quit();
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        close();
        return false;
    }


    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        IMG_Quit();
        close();
        return false;
    }
    return true;
}

int main(int argc, char* args[]) {

    if (!init()) {
        std::cerr << "Initialization failed." << std::endl;
        return -1;
    }


    SceneManager sceneManager;
    MainMenuScene* mainMenu = new MainMenuScene(sceneManager);
    GameScene* gameScene = new GameScene();


    if (!mainMenu->init(gRenderer)) {
        std::cerr << "Failed to initialize MainMenuScene!" << std::endl;
        delete mainMenu;
        delete gameScene;
        close();
        return -1;
    }
    if (!gameScene->init(gRenderer)) {
        std::cerr << "Failed to initialize GameScene!" << std::endl;
        mainMenu->cleanUp();
        delete mainMenu;
        delete gameScene;
        close();
        return -1;
    }

    sceneManager.addScene(mainMenu);
    sceneManager.addScene(gameScene);

    sceneManager.changeScene(SceneID::MainMenu);

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            sceneManager.handleEvents(e);
        }
        sceneManager.update();
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);
        sceneManager.render();
        SDL_RenderPresent(gRenderer);
    }
    close();
    return 0;
}
