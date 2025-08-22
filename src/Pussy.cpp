#include "Pussy.h"

Texture2D Pussy::image = { 0 };

<<<<<<< HEAD:src/Pussy.cpp
Pussy::Pussy(int type, Vector2 position) : PussyBase(type, position, MAX_HEALTH) {}
=======
int Pussy::pussyDirection = 2;
float Pussy::pussyShootTimer = 0.0f;
float Pussy::pussyShootInterval = 1.5f;

Pussy::Pussy(int type, Vector2 position) {
    this->type = type;
    this->position = position;
}
>>>>>>> Toan:PUSSY_INVADERS/src/Pussy.cpp

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

<<<<<<< HEAD:src/Pussy.cpp
=======
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

>>>>>>> Toan:PUSSY_INVADERS/src/Pussy.cpp
int Pussy::GetType() {
    return type;
}

<<<<<<< HEAD:src/Pussy.cpp
Texture2D& Pussy::getTexture() {
    return image;
}
=======
Rectangle Pussy::getRect() {
    static constexpr float SCALE = 0.3f;
    return {
        position.x - (image.width * SCALE) / 2,
        position.y - (image.height * SCALE) / 2,
        image.width * SCALE,
        image.height * SCALE
    };
}
>>>>>>> Toan:PUSSY_INVADERS/src/Pussy.cpp
