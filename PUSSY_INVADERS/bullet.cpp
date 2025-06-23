// bullet.cpp
#include "bullet.h"

Bullet::Bullet(Vector2 pos) {
    position = pos;
    speed = 500.0f; // pixels per second
    active = true;
}

void Bullet::Update() {
    if (active) {
        position.y -= speed * GetFrameTime();
        // Nếu viên đạn ra khỏi màn hình, đặt active = false
        if (position.y < 0) {
            active = false;
        }
    }
}

void Bullet::Draw() {
    if (active) {
        DrawCircleV(position, 5, YELLOW); // Vẽ viên đạn là hình tròn nhỏ màu vàng
    }
}