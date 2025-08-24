#pragma once
#include "PussyBase.h"

class Pussy3 : public PussyBase {
public: 
    Pussy3(int type, Vector2 position);
    int GetType() override;
    static void LoadImage() ;
    static void UnloadImage();
    static constexpr int MAX_HEALTH = 10; // Khác với Pussy và Pussy2

private:
    static Texture2D image;
    Texture2D& getTexture() override;
};