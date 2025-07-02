#pragma once
#include "Background.h"
#include "MusicController.h"
#include "GameOver.h"
#include<algorithm>

class GameController
{
private:
    MusicController music;
    Background background;
    GameOver gameover;
    GameState gameState;
    float blurOpacity; 
    float countdownTimer;

    void DrawPaused();
    void DrawUI();
    void DrawCountdown();
    void DrawGameOver();
    void HandleInput();
    void HandleCountdown();

public:
    GameController();
    ~GameController();
    void Update();
    void Draw();

};