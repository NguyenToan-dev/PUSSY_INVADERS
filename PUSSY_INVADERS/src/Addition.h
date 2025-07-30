#pragma once

const float FADE_BLUR_DURATION = 1.0f;
const float HOLD_DURATION = 2.0f;
const float RED_FLASH_DURATION = 0.5f;
const float COUNTDOWN_DURATION = 3.0f;
const float MAX_BLUR_OPACITY = 200;
const float SLOW_MOTION_FACTOR = 0.25f;

enum MusicState 
{
    MUSIC_IDLE,
    MUSIC_FADING_IN,
    MUSIC_PLAYING,
    MUSIC_FADING_OUT,
    MUSIC_PAUSED
};

enum GameState 
{
    GAME_PLAYING_STATE,
    GAME_GAME_OVER,
    GAME_COUNTDOWN,
    GAME_PAUSED
};

enum GameOverPhase 
{
    FREEZE,
    FADE_BLUR,
    RED_FLASH,
    TEXT_DISPLAY
};

enum ShipStatus
{  
    SCORE_GAIN_1, SCORE_GAIN_2, SCORE_GAIN_3,
    HEAT_INCREASE, HEAT_DECREASE,
    LIVE_DECREASE,
    LEVEL_UP,
    SUSHI_ADD, MILK_ADD,
    MISSILE_ADD,
    NEW_BULLET
};
