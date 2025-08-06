#include "Pussy2.h"

Texture2D Pussy2::image = { 0 };

Pussy2::Pussy2(int type, Vector2 position) : PussyBase(type, position, MAX_HEALTH) {}

void Pussy2::LoadImage() {
    if (image.id == 0) {
        image = LoadTexture("image/pussy2.png");
        if (image.id == 0) {
            TraceLog(LOG_ERROR, "Failed to load image/pussy2.png");
        }
    }
}

void Pussy2::UnloadImage() {
    if (image.id != 0) {
        UnloadTexture(image);
        image.id = 0;
    }
}

int Pussy2::GetType() {
    return type;
}

Texture2D& Pussy2::getTexture() {
    return image;
}