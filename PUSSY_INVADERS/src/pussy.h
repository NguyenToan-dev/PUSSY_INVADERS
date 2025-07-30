#pragma once
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
