#pragma once
#include<raylib.h>
#include<vector>
#include "bullet.h"
#include "Brightness.h"
#include "SpaceShipTexture.h"
#define ull unsigned long long
using namespace std;

class SpaceShip
{
private:
    int live_counter;
    int missile_counter;
    int weapon_level;
    int thigh_counter;
    int overheat;
    ull score;
    static int heat_limit;

    float timer;    // removable

    Brightness fireball_brightness;
    SpaceShipTexture image;
    Font font;

public:
    SpaceShip();
    ~SpaceShip();
    void InsertSpaceShipTexture();
    Texture2D GetShip() const;
    Texture2D GetFireball() const;
    void Moving();
    void StatusBar();
    void UpdateStatus();
    Vector2 HitBoxChecking(Vector2);
    Vector2 RocketPosition(Vector2);
    void Shooting(std::vector<Bullet>& bullets, Texture2D* bulletTexture);
};