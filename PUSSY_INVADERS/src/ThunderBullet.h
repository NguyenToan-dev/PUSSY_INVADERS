#pragma once
#include "Bullet.h"
#include <raylib.h>
#include <cmath>

class ThunderBullet : public Bullet {
public:
    ThunderBullet(Vector2 pos, Texture2D* tex);

    void Update() override;
    void Draw() override;
    Rectangle getRect() const override;
};
