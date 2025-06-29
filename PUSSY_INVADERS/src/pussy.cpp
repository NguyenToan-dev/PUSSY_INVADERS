
#include "pussy.h"

Texture2D Pussy::image = { 0 };

Pussy::Pussy(int type, Vector2 position) {
    this->type = type;
    this->position = position;
}

void Pussy::LoadImage() {
    if (image.id == 0) {
        image = LoadTexture("image/pussy.png");
        if (image.id == 0) {
            TraceLog(LOG_ERROR, "Failed to load image/pussy.png");
        }
    }
}

void Pussy::UnloadImage() {
    if (image.id != 0) {
        UnloadTexture(image);
        image.id = 0;
    }
}

void Pussy::Update(int direction) {
    position.x += direction;
}

void Pussy::Draw() {
    if (image.id == 0) return;

    static constexpr float SCALE = 0.3f;
    Vector2 drawPos = {
        position.x - (image.width * SCALE) / 2,
        position.y - (image.height * SCALE) / 2
    };
    DrawTextureEx(image, drawPos, 0.0f, SCALE, WHITE);
}

int Pussy::GetType() {
    return type;
}

Rectangle Pussy::getRect() {
    static constexpr float SCALE = 0.3f;
    return {
        position.x - (image.width * SCALE) / 2,
        position.y - (image.height * SCALE) / 2,
        image.width * SCALE,
        image.height * SCALE
    };
}
