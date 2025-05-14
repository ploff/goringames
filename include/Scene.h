#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>

class Scene {
public:
    virtual ~Scene() {}
    virtual bool init(SDL_Renderer* renderer) = 0;
    virtual void handleEvents(SDL_Event& e) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void cleanUp() = 0;
};

#endif
