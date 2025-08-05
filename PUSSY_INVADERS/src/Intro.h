#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <sstream>
struct IntroScene {
    std::string imagePath;
    std::string text;
};

class Intro {
public:
    Intro();
    ~Intro();

    void Update();
    void Draw();
    bool IsFinished() const;
    Music getMusic() const { return music; }
private:
    void LoadScenes();
    void AdvanceScene();
    void DrawMultilineText(const std::string& text, Vector2 pos, float maxWidth, float fontSize, float spacing, Color color);

    std::vector<IntroScene> scenes;
    std::vector<Texture2D> images;
    Music music;
    Sound meowSound;
    Font font;

    int currentScene = 0;
    float sceneTimer = 0.0f;
    float sceneDuration = 7.0f;
    bool finished = false;
    bool allowSkipButton = false;
    bool meowPlayed = false;
    bool waiting = false; 
    float waitTimer = 0.0f; 
};
