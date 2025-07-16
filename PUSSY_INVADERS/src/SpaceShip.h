//SpaceShip.h
#pragma once
#include<raylib.h>
#include<vector>
#include "Bullet.h"
#include "Brightness.h"
#include "SpaceShipTexture.h"
#include "Addition.h"
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

    int sushi_collected = 0;
    int milk_collected = 0;
    int battery_collected = 0;
    bool isNewBullet = false;


    Brightness fireball_brightness;
    SpaceShipTexture image;
    Font font;

    void UpdateStatus(ShipStatus);

public:
    SpaceShip();
    ~SpaceShip();
    void InsertSpaceShipTexture();
    void SetAttribute();
    Texture2D GetShip() const;
    Texture2D GetFireball() const;
    void Moving();
    void MovingWhileBlinking(Color);
    void StatusBar();
    Vector2 BoundChecking(Vector2);
    Vector2 RocketPosition(Vector2);
    void Shooting(std::vector<Bullet>& bullets, Texture2D* bulletTexture);

    Rectangle getRect();
    int HitBoxChecking(vector<Bullet*>&);
    void AdjustStatus(ShipStatus);
    void EatPickup(); // để ăn pickup
};
