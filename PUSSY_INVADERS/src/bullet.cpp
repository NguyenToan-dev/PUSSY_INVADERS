#include "bullet.h"

Bullet::Bullet(Vector2 pos, Texture2D* tex) {
    position = pos;
    speed = 500.0f;
    active = true;
    texture = tex;
}

void Bullet::Update() {
    if (active) {
        position.y -= speed * GetFrameTime();
        float scale = 0.1f;
        if (position.y + (texture ? texture->height * scale : 0) < 0) {
            active = false;
        }
    }
}

void Bullet::Draw() {
    if (active && texture) {
        float scale = 0.1f;
        Vector2 drawPos = {
            position.x - (texture->width * scale) / 2,
            position.y - (texture->height * scale) / 2
        };
        DrawTextureEx(*texture, drawPos, 0.0f, scale, WHITE);
    }
}