#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Button.h"
#include "Paths.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class GameScene : public Scene {
private:
    SDL_Texture* infoScreen = nullptr;
    SDL_Texture* attScreen = nullptr;
    SDL_Texture* endgameScreen = nullptr;
    SDL_Texture* axe1 = nullptr;
    SDL_Texture* axe2 = nullptr;
    SDL_Texture* axe3 = nullptr;
    SDL_Texture* gameScreen = nullptr;
    SDL_Texture* scoreTexture = nullptr;
    SDL_Texture* attemptsTexture = nullptr;
    SDL_Texture* timerTexture = nullptr;
    SDL_Texture* currentAxe = nullptr;
    SDL_Rect attemptsRect;
    SDL_Rect timerRect;
    SDL_Color textColor = {0, 217, 12, 255};
    SDL_Color debugTextColor = {255, 0, 255, 255};
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    const int maxAttempts = 11;
    const int timerDuration = 11000;
    int correctAnswers = 0;
    int attempts = 1;
    bool buttonsEnabled = true;
    bool timerActive = false;
    bool renderAxe = false;
    Uint32 timerStartTime = 1;
    Uint32 lastStateChangeTime;
    Uint32 axeRenderStartTime = 0;
    Button* button1 = nullptr;
    Button* button2 = nullptr;
    Button* button3 = nullptr;

    enum GameState {
        INFO_SCREEN,
        ATTEMPT_SCREEN,
        GUESS_SCREEN,
        ANSWER_SCREEN,
        ENDGAME_SCREEN
    };

    GameState currentState;

public:
    bool init(SDL_Renderer* renderer) override {
        this->renderer = renderer;

        if (!this->renderer) {
            std::cerr << "SDL_Renderer is null!" << std::endl;
            return false;
        }

        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return false;
        }
        font = TTF_OpenFont(FONT, 24);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return false;
        }

        infoScreen = IMG_LoadTexture(renderer, INFOSCREEN);
        if (!infoScreen) {
            std::cerr << "Failed to load info.jpg: " << IMG_GetError() << std::endl;
            cleanUp();
            return false;
        }

        gameScreen = IMG_LoadTexture(renderer, GAMESCREEN);
        if (!gameScreen) {
            std::cerr << "Failed to load game.jpg: " << IMG_GetError() << std::endl;
            cleanUp();
            return false;
        }

        attScreen = IMG_LoadTexture(renderer, ATTEMPTSCREEN);
        if (!attScreen) {
            std::cerr << "Failed to load attempts.jpg: " << IMG_GetError() << std::endl;
            cleanUp();
            return false;
        }

        endgameScreen = IMG_LoadTexture(renderer, ENDSCREEN);
        if (!endgameScreen) {
            std::cerr << "Failed to load end.jpg: " << IMG_GetError() << std::endl;
            cleanUp();
            return false;
        }

        axe1 = IMG_LoadTexture(renderer, FIRST);
        if (!axe1) {
            std::cerr << "Failed to load 1.jpg: " << IMG_GetError() << std::endl;
            cleanUp();
            return false;
        }

        axe2 = IMG_LoadTexture(renderer, SECOND);
        if (!axe2) {
            std::cerr << "Failed to load 2.jpg: " << IMG_GetError() << std::endl;
            cleanUp();
            return false;
        }

        axe3 = IMG_LoadTexture(renderer, THIRD);
        if (!axe3) {
            std::cerr << "Failed to load 3.jpg: " << IMG_GetError() << std::endl;
            cleanUp();
            return false;
        }

        button1 = new Button(renderer, "", 300, 29, 142, 61);
        button2 = new Button(renderer, "", 305, 100, 136, 66);
        button3 = new Button(renderer, "", 296, 178, 146, 64);

        // button borders for debug
        button1->enableBorder(debugTextColor);
        button2->enableBorder(debugTextColor);
        button3->enableBorder(debugTextColor);

        currentState = INFO_SCREEN;
        lastStateChangeTime = SDL_GetTicks();

        // random num gen for axes
        srand(static_cast<unsigned int>(time(0)));

        return true;
    }

    void handleEvents(SDL_Event& e) override {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (currentState == ANSWER_SCREEN && buttonsEnabled && attempts < maxAttempts) {
                button1->handleEvents(e);
                button2->handleEvents(e);
                button3->handleEvents(e);

                if (button1->isClicked()) {
                    handleAnswer(1);
                } else if (button2->isClicked()) {
                    handleAnswer(2);
                } else if (button3->isClicked()) {
                    handleAnswer(3);
                }
            }
        }
    }

    void update() override {
        Uint32 currentTime = SDL_GetTicks();

        switch (currentState) {
            case INFO_SCREEN:
                if (currentTime - lastStateChangeTime >= 5000) {
                    currentState = ATTEMPT_SCREEN;
                    lastStateChangeTime = currentTime;
                }
                break;

            case ATTEMPT_SCREEN:
                if (currentTime - lastStateChangeTime >= 3000) {
                    currentState = GUESS_SCREEN;
                    lastStateChangeTime = currentTime;
                    renderAxe = false;
                    currentAxe = nullptr;
                    buttonsEnabled = true;
                    if(attempts == maxAttempts) {
                        currentState = ENDGAME_SCREEN;
                        lastStateChangeTime = currentTime;
                        renderScore();
                    }
                }
                break;

            case GUESS_SCREEN:
                if (currentTime - lastStateChangeTime >= 0) {
                    currentState = ANSWER_SCREEN;
                    lastStateChangeTime = currentTime;
                    timerActive = true;
                    timerStartTime = currentTime;
                    updateTimer();
                }
                break;

            case ANSWER_SCREEN:
                if (timerActive && currentTime - timerStartTime >= timerDuration) {
                    timerActive = false;
                    showRandomAxe();
                    axeRenderStartTime = currentTime;
                    renderAxe = true;
                }
                if (renderAxe && currentTime - axeRenderStartTime >= 3000) {
                    renderAxe = false;
                    if (attempts < maxAttempts) {
                        currentState = ATTEMPT_SCREEN;
                        lastStateChangeTime = currentTime;
                    } else {
                        currentState = ENDGAME_SCREEN;
                        lastStateChangeTime = currentTime;
                        renderScore();
                    }
                }
                if (timerActive) {
                    updateTimer();
                }
                break;

            case ENDGAME_SCREEN:

                break;
        }
    }

    void render() override {

        SDL_RenderClear(renderer);

        switch (currentState) {
            case INFO_SCREEN:
                SDL_RenderCopy(renderer, infoScreen, nullptr, nullptr);
                break;

            case ATTEMPT_SCREEN:
                SDL_RenderCopy(renderer, attScreen, nullptr, nullptr);
                renderAttempts();
                break;

            case GUESS_SCREEN:
                SDL_RenderCopy(renderer, gameScreen, nullptr, nullptr);
                break;

            case ANSWER_SCREEN:
                renderGameScreen();
                if (renderAxe && currentAxe != nullptr) {
                    SDL_RenderCopy(renderer, currentAxe, nullptr, nullptr);
                }
                if (timerActive && timerTexture) {
                    SDL_RenderCopy(renderer, timerTexture, nullptr, &timerRect);
                }
                break;

            case ENDGAME_SCREEN:
                SDL_RenderCopy(renderer, endgameScreen, nullptr, nullptr);
                if (scoreTexture) {
                    SDL_Rect scoreRect;
                    SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreRect.w, &scoreRect.h);
                    scoreRect.x = 95;
                    scoreRect.y = 150;
                    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
                }
                break;
        }
        SDL_RenderPresent(renderer);
    }

    void cleanUp() override {

        if (infoScreen) {
            SDL_DestroyTexture(infoScreen);
            infoScreen = nullptr;
        }
        if (attScreen) {
            SDL_DestroyTexture(attScreen);
            attScreen = nullptr;
        }
        if (endgameScreen) {
            SDL_DestroyTexture(endgameScreen);
            endgameScreen = nullptr;
        }
        if (axe1) {
            SDL_DestroyTexture(axe1);
            axe1 = nullptr;
        }
        if (axe2) {
            SDL_DestroyTexture(axe2);
            axe2 = nullptr;
        }
        if (axe3) {
            SDL_DestroyTexture(axe3);
            axe3 = nullptr;
        }
        if (gameScreen) {
            SDL_DestroyTexture(gameScreen);
            gameScreen = nullptr;
        }
        if(scoreTexture) {
            SDL_DestroyTexture(scoreTexture);
            scoreTexture = nullptr;
        }
        if (timerTexture) {
            SDL_DestroyTexture(timerTexture);
            timerTexture = nullptr;
        }
        if(font){
            TTF_CloseFont(font);
            font = nullptr;
        }
        if (button1) {
            delete button1;
            button1 = nullptr;
        }
        if (button2) {
            delete button2;
            button2 = nullptr;
        }
        if (button3) {
            delete button3;
            button3 = nullptr;
        }

        TTF_Quit();
    }

private:
    void renderGameScreen() {
        SDL_RenderCopy(renderer, gameScreen, nullptr, nullptr);

        button1->render();
        button2->render();
        button3->render();
    }

    void handleAnswer(int selectedButton) {
        buttonsEnabled = false;
        timerActive = false;
        if (timerTexture) {
            SDL_DestroyTexture(timerTexture);
            timerTexture = nullptr;
        }

        int correctAnswer = rand() % 3 + 1;

        if (correctAnswer == 1) {
            currentAxe = axe1;
        }
        else if (correctAnswer == 2) {
            currentAxe = axe2;
        }
        else {
            currentAxe = axe3;
        }

        if(selectedButton == correctAnswer){
            correctAnswers++;
        }

        attempts++;

        renderAxe = true;
        axeRenderStartTime = SDL_GetTicks();

        lastStateChangeTime = SDL_GetTicks();
    }

    void renderScore(){
        std::string scoreText = "Угадано " + std::to_string(correctAnswers) + " из 10";
        SDL_Surface* scoreSurface = TTF_RenderUTF8_Solid(font, scoreText.c_str(), textColor);

        if (scoreSurface) {
            scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            SDL_FreeSurface(scoreSurface);
        }
    }

    void renderAttempts(){
        std::string attemptsText = std::to_string(attempts);
        SDL_Surface* attemptsSurface = TTF_RenderText_Solid(font, attemptsText.c_str(), textColor);

        if(attemptsSurface){
            attemptsTexture = SDL_CreateTextureFromSurface(renderer, attemptsSurface);
            SDL_FreeSurface(attemptsSurface);

            if(attemptsTexture){
                attemptsRect.x = 187;
                attemptsRect.y = 96;
                SDL_QueryTexture(attemptsTexture, nullptr, nullptr, &attemptsRect.w, &attemptsRect.h);

                SDL_RenderCopy(renderer, attemptsTexture, nullptr, &attemptsRect);
                SDL_DestroyTexture(attemptsTexture);
                attemptsTexture = nullptr;
            }
        }
    }

    void updateTimer() {
        if (!timerActive) return;

        Uint32 currentTime = SDL_GetTicks();
        int timeLeft = timerDuration - (currentTime - timerStartTime);

        if (timeLeft <= 0) {
            timeLeft = 0;
            timerActive = false;
            currentState = ATTEMPT_SCREEN;
            lastStateChangeTime = currentTime;

            if (attempts < maxAttempts) {
                attempts++;
            }
        }

        std::string timerText = std::to_string(timeLeft / 1000);
        SDL_Surface* timerSurface = TTF_RenderText_Solid(font, timerText.c_str(), textColor);

        if (timerSurface) {
            if (timerTexture) {
                SDL_DestroyTexture(timerTexture);
            }
            timerTexture = SDL_CreateTextureFromSurface(renderer, timerSurface);
            SDL_FreeSurface(timerSurface);

            if (timerTexture) {
                timerRect.x = 25;
                timerRect.y = 20;
                SDL_QueryTexture(timerTexture, nullptr, nullptr, &timerRect.w, &timerRect.h);
            }
        }
    }

    void showRandomAxe() {
        int randomAxe = rand() % 3 + 1;
        if (randomAxe == 1) {
            currentAxe = axe1;
        } else if (randomAxe == 2) {
            currentAxe = axe2;
        } else {
            currentAxe = axe3;
        }
        attempts++;
    }
};

#endif
