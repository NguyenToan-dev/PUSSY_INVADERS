#pragma once
#include "raylib.h"

class Bullet {
public:
    Vector2 position;
    float speed;
    bool active;
    Texture2D* texture;
    int damage;  

public:
    Bullet(Vector2 pos, Texture2D* tex);
    virtual void Update();
    virtual void Draw();
    virtual Rectangle getRect() const;
};


