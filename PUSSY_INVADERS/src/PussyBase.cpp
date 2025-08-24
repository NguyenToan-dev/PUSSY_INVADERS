#include "PussyBase.h"

float PussyBase::pussyDirection = 2.0f;
float PussyBase::pussyShootTimer = 0.0f;
float PussyBase::pussyShootInterval = 1.5f;

PussyBase::PussyBase(int type, Vector2 position, int health)
    : type(type), position(position), health(health) {}

void PussyBase::Draw() {
    Texture2D& image = getTexture();
    if (image.id == 0) return;

    static constexpr float SCALE = 0.3f;
    Vector2 drawPos = {
        position.x - (image.width * SCALE) / 2,
        position.y - (image.height * SCALE) / 2
    };
    DrawTextureEx(image, drawPos, 0.0f, SCALE, WHITE);
}

void PussyBase::Update(float direction) {
    position.x += direction;
}

Rectangle PussyBase::getRect() {
    Texture2D& image = getTexture();
    static constexpr float SCALE = 0.3f;
    return {
        position.x - (image.width * SCALE) / 2,
        position.y - (image.height * SCALE) / 2,
        image.width * SCALE,
        image.height * SCALE
    };
}