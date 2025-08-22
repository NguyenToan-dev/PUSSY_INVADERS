#include "Pussy.h"

Texture2D Pussy::image = { 0 };

Pussy::Pussy(int type, Vector2 position) : PussyBase(type, position, MAX_HEALTH) {}

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

int Pussy::GetType() {
    return type;
}

Texture2D& Pussy::getTexture() {
    return image;
}
