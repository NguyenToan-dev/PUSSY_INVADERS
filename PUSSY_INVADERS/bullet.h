// bullet.h
#pragma once
#include "raylib.h"

class Bullet {
public:
    Vector2 position;
    float speed;
    bool active;

    Bullet(Vector2 pos);
    void Update();
    void Draw();
};