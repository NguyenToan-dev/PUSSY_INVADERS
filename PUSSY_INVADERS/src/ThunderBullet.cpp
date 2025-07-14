#include "ThunderBullet.h"

ThunderBullet::ThunderBullet(Vector2 pos, Texture2D* tex) : Bullet(pos, tex) {
    speed = -6.0f;
    damage = 3;
}

void ThunderBullet::Update() {
    position.y -= speed;  // Đi xuống
    position.x += sin(GetTime() * 15.0f) * 2.0f; // zigzag
    if (position.y > GetScreenHeight()) active = false;
}

void ThunderBullet::Draw() {
    if (active && texture) {
        float scale = 0.2f;
        Vector2 drawPos = {
            position.x - (texture->width * scale) / 2,
            position.y - (texture->height * scale) / 2
        };
        DrawTextureEx(*texture, drawPos, 0.0f, scale, WHITE);
    }
}

Rectangle ThunderBullet::getRect() const
{
    float scale = 0.2f;
    return {
        position.x - (texture->width * scale) / 2,
        position.y - (texture->height * scale) / 2,
        texture->width * scale,
        texture->height * scale
    };
}
