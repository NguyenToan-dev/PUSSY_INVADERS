#pragma once
#include<raylib.h>
#include "Addition.h"

class GameOver
{
private:
    GameOverPhase gameOverPhase;
    float gameOverTimer;

public:
    GameOver();
    void HandleGameOver(float);
    void SetTime(float);
    void SetPhase(GameOverPhase);
    GameOverPhase GetPhase() const;
    float GetTime() const;
};