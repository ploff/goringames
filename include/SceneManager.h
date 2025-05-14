#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

#include <vector>

enum class SceneID {
    MainMenu,
    Game,
};

class SceneManager {
private:
    std::vector<Scene*> scenes;
    SceneID currentSceneIndex;

public:
    SceneManager() : currentSceneIndex(SceneID::MainMenu) {}

    ~SceneManager() {
        for (auto& scene : scenes) {
            scene->cleanUp();
            delete scene;
        }
    }

    void addScene(Scene* scene) {
        scenes.push_back(scene);
    }

    void changeScene(SceneID index) {
        if (static_cast<int>(index) >= 0 && static_cast<int>(index) < scenes.size()) {
            currentSceneIndex = index;
        }
    }

    void handleEvents(SDL_Event& e) {
        scenes[static_cast<int>(currentSceneIndex)]->handleEvents(e);
    }

    void update() {
        scenes[static_cast<int>(currentSceneIndex)]->update();
    }

    void render() {
        scenes[static_cast<int>(currentSceneIndex)]->render();
    }
};

#endif
