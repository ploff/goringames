#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

using namespace std;

class Button {
private:
    SDL_Rect mRect;
    SDL_Texture* mTexture;
    SDL_Renderer* mRenderer;
    bool mIsClicked;
    bool drawBorder;
    SDL_Color borderColor;

public:
    Button(SDL_Renderer* renderer, const string& texturePath, int x, int y, int width, int height)
    : mRenderer(renderer), mIsClicked(false), drawBorder(false), borderColor({ 0, 0, 0, 255 }) {
        mRect = { x, y, width, height };
        if (!texturePath.empty()) {
            mTexture = IMG_LoadTexture(mRenderer, texturePath.c_str());
            if (!mTexture) {
                cerr << "Failed to load button texture: " << IMG_GetError() << " on path: " << texturePath << endl;
            }
        } else {
            mTexture = nullptr;
        }
    }

    ~Button() {
        if (mTexture) {
            SDL_DestroyTexture(mTexture);
        }
    }

    bool isClicked() const {
        return mIsClicked;
    }

    void render() {
        if (mTexture) {
            SDL_RenderCopy(mRenderer, mTexture, nullptr, &mRect);
        }


        if (drawBorder) {
            SDL_SetRenderDrawColor(mRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
            SDL_RenderDrawRect(mRenderer, &mRect);
        }
    }

    void handleEvents(SDL_Event& e) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                int windowX = e.button.x;
                int windowY = e.button.y;
                float logicalX, logicalY;
                SDL_RenderWindowToLogical(mRenderer, windowX, windowY, &logicalX, &logicalY);

                if (logicalX >= mRect.x && logicalX < mRect.x + mRect.w &&
                    logicalY >= mRect.y && logicalY < mRect.y + mRect.h) {
                    mIsClicked = true;
                    } else {
                        mIsClicked = false;
                    }
            }
        }

        if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                if (mIsClicked) {
                }
                mIsClicked = false;
            }
        }
    }

    void resetClick() {
        mIsClicked = false;
    }

    void enableBorder(const SDL_Color& color = { 0, 0, 0, 255 }) {
        drawBorder = true;
        borderColor = color;
    }

    void disableBorder() {
        drawBorder = false;
    }

    SDL_Rect getRect() const {
        return mRect;
    }

};

#endif
