#pragma once
#include "PussyBase.h"

class Pussy2 : public PussyBase {
public: 
    Pussy2(int type, Vector2 position);
    int GetType() override;
    static void LoadImage() ;
    static void UnloadImage();
    static constexpr int MAX_HEALTH = 5; // Khác với Pussy

private:
    static Texture2D image;
    Texture2D& getTexture() override;
};