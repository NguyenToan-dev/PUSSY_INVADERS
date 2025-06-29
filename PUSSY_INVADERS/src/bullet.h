#pragma once
#include "raylib.h"

class Bullet {
public:
    Vector2 position;
    float speed;
    bool active;
    Texture2D* texture;

    Bullet(Vector2 pos, Texture2D* tex);
    void Update();
    void Draw();
    Rectangle getRect() const;  // 👈 Thêm dòng này
};
