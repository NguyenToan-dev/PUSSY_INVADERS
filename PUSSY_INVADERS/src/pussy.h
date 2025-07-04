#pragma once
#include <raylib.h>

class Pussy {
public:
    Pussy(int type, Vector2 position);
    void Update(int direction);
    void Draw();
    int GetType();
    Rectangle getRect();

    static void LoadImage();     // Gọi sau InitWindow
    static void UnloadImage();   // Khi thoát game

    static Texture2D image;      // Dùng chung cho tất cả pussy
    Vector2 position;
    int type;
};
