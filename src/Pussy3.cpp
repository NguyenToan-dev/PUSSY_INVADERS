#include "Pussy3.h"

Texture2D Pussy3::image = { 0 };

Pussy3::Pussy3(int type, Vector2 position) : PussyBase(type, position, MAX_HEALTH) {}

void Pussy3::LoadImage() {
    if (image.id == 0) {
        image = LoadTexture("image/pussy3.png");
        if (image.id == 0) {
            TraceLog(LOG_ERROR, "Failed to load image/pussy3.png");
        }
    }
}

void Pussy3::UnloadImage() {
    if (image.id != 0) {
        UnloadTexture(image);
        image.id = 0;
    }
}

int Pussy3::GetType() {
    return type;
}

Texture2D& Pussy3::getTexture() {
    return image;
}