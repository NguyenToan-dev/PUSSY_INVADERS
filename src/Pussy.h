#pragma once
#include "PussyBase.h"

class Pussy : public PussyBase {
public: 
    Pussy(int type, Vector2 position);
    int GetType() override;
    static void LoadImage();
    static void UnloadImage();
    static constexpr int MAX_HEALTH = 2;

private:
    static Texture2D image;
    Texture2D& getTexture() override;
};
