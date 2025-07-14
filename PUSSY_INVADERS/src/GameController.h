//GameController.h
#pragma once
#include "Background.h"
#include "MusicController.h"
#include "GameOver.h"
#include "SpaceShip.h"
#include "Pussy.h"
#include<algorithm>
#include<vector>
#include "ThunderBullet.h"  
#include "Pickup.h"//->recently added

class GameController
{
private:
    MusicController music;
    Background background;
    GameOver gameover;
    GameState gameState;
    float blurOpacity;
    float countdownTimer;

    SpaceShip ship;
    Sound ship_shootsound;
    Sound pussy_shootsound;
    std::vector<Bullet> bullets;
    Texture2D bullet_texture;
    std::vector<Pussy> pussies;
    Texture pussy_shit_texture;
    std::vector<Bullet*> pussyBullets;
    Texture2D thunder_texture;
    //std::vector<Bullet*> thunderBullets;
    Sound thunderSound;

    double timestart;
    float blinkspeed;
    float blinkduration;
    bool isblinking;
    Color blink1;
    Color blink2;


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
    void HandleObjectDrawing();

};