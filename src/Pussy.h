#pragma once
<<<<<<< HEAD:src/Pussy.h
#include "PussyBase.h"

class Pussy : public PussyBase {
public: 
    Pussy(int type, Vector2 position);
    int GetType() override;
    static void LoadImage();
    static void UnloadImage();
    static constexpr int MAX_HEALTH = 1;

private:
    static Texture2D image;
    Texture2D& getTexture() override;
};
=======
#include <raylib.h>

class Pussy {
public:
    Pussy(int type, Vector2 position);
    void Update(int direction);
    void Draw();
    int GetType();

    static int pussyDirection;
    static float pussyShootTimer;
    static float pussyShootInterval;

    Rectangle getRect();

    static void LoadImage();     
    static void UnloadImage();   

    static Texture2D image;     
    Vector2 position;
    int type;
};
>>>>>>> Toan:PUSSY_INVADERS/src/Pussy.h
