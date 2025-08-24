// Outro.h
#pragma once

#include <string>
#include <vector>
#include <raylib.h>
#include "raymath.h"
class Outro {
public:
    Outro();
    ~Outro();

    void LoadScenes();
    void Update();
    void Draw();
    bool IsFinished() const;
    void AdvanceScene();
    Music getMusic() const { return music; }
private:
    struct Scene {
        std::string imagePath;
        std::string text;
    };

    std::vector<Scene> scenes;
    std::vector<Texture2D> images;

    Music music;
    Sound typingSound;
    Sound commSound; // sound for the comm / deep voice
    Font font;
    Sound pilotSound;
    bool pilotPlayed = false;

    int currentScene = 0;
    float sceneTimer = 0.0f;
    float sceneDuration = 18.0f; // seconds per scene
    bool finished = false;

    bool waiting = false;
    float waitTimer = 0.0f;

    int displayedChars = 0;
    bool commPlayed = false;

    bool allowSkipButton = true;

    // outro ending flow
    bool endingTriggered = false; // when last scene's duration passed
    float endingTextTimer = 0.0f;
    bool showEndingText = false;

private:
    void DrawMultilineText(const std::string& text, Vector2 pos, float maxWidth, float fontSize, float spacing, Color color);
};

