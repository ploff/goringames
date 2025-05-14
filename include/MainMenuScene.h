#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Scene.h"
#include "Button.h"
#include "SceneManager.h"
#include "Paths.h"

#include <iostream>
#include <SDL2/SDL_image.h>

class MainMenuScene : public Scene {
private:
    SceneManager& sceneManager;
    SDL_Texture* backgroundTexture;
    SDL_Renderer* renderer;
    Button* startButton;

    int logicalWidth = 446;
    int logicalHeight = 253;

public:
    MainMenuScene(SceneManager& sceneMgr) : backgroundTexture(nullptr), renderer(nullptr), startButton(nullptr), sceneManager(sceneMgr) {}

    bool init(SDL_Renderer* renderer) override {
        this->renderer = renderer;
        if (!this->renderer) {
            std::cerr << "Renderer is null in MainMenuScene::init" << std::endl;
            return false;
        }

        backgroundTexture = IMG_LoadTexture(renderer, MENUBG);

        if (!backgroundTexture) {
            std::cerr << "Failed to load start.jpg! Error: " << IMG_GetError() << std::endl;
            return false;
        }

        int buttonWidth = 109;
        int buttonHeight = 46;
        int buttonX = (logicalWidth - buttonWidth) / 2;
        int buttonY = logicalHeight / 2 + 70;

        startButton = new Button(renderer, PLAYBUTTON, buttonX, buttonY, buttonWidth, buttonHeight);
        if (!startButton) {
            std::cerr << "Failed to create start button!" << std::endl;
            SDL_DestroyTexture(backgroundTexture);
            backgroundTexture = nullptr;
            return false;
        }

        return true;
    }

    void handleEvents(SDL_Event& e) override {
        startButton->handleEvents(e);

        if (startButton->isClicked()) {
            sceneManager.changeScene(SceneID::Game);
        }
    }


    void update() override {
    }

    void render() override {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        startButton->render();
    }

    void cleanUp() override {
        if (backgroundTexture) {
            SDL_DestroyTexture(backgroundTexture);
            backgroundTexture = nullptr;
        }
        if (startButton) {
            delete startButton;
            startButton = nullptr;
        }
    }

};

#endif // MAINMENUSCENE_H
