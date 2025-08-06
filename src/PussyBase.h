#pragma once
#include <raylib.h>

class PussyBase {
public:
    virtual ~PussyBase() = default;
    void Draw();
    void Update(float direction);
    Rectangle getRect();
    virtual int GetType() = 0;
    static void LoadImage();
    static void UnloadImage();

    Vector2 position;
    int health;
    static float pussyDirection;
    static float pussyShootTimer;
    static float pussyShootInterval;

protected:
    PussyBase(int type, Vector2 position, int health);
    int type;
    virtual Texture2D& getTexture() = 0;
};