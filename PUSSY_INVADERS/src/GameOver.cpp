#include "GameOver.h"
#include<iostream>

GameOver::GameOver()
{
    gameOverPhase = FREEZE;
    gameOverTimer = 0.0f;
}

void GameOver::HandleGameOver(float blurOpacity) 
{
    gameOverTimer += GetFrameTime();
    switch (gameOverPhase) 
    {
        case FREEZE:
            gameOverTimer = 0.0f;
            gameOverPhase = FADE_BLUR;
            break;

        case FADE_BLUR:
            blurOpacity = (gameOverTimer / FADE_BLUR_DURATION) * MAX_BLUR_OPACITY;
            if (gameOverTimer >= FADE_BLUR_DURATION + HOLD_DURATION) 
            {
                gameOverPhase = RED_FLASH;
                gameOverTimer = 0.0f;
                blurOpacity = MAX_BLUR_OPACITY;
            }
            break;

        case RED_FLASH:
            if (gameOverTimer >= RED_FLASH_DURATION) 
            {
                gameOverPhase = TEXT_DISPLAY;
                gameOverTimer = 0.0f;
            }
            break;

        case TEXT_DISPLAY:
            break;
    }
}

void GameOver::SetTime(float data) 
{
    gameOverTimer = data;
    return;
}

float GameOver::GetTime() const { return gameOverTimer; }

GameOverPhase GameOver::GetPhase() const { return gameOverPhase; }

void GameOver::SetPhase(GameOverPhase data)
{
    gameOverPhase = data;
    return;
}
