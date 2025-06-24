#pragma once
#include<raylib.h>

class Brightness
{
private:
    float brightness;
    float brightnessSpeed;
    float brightnessDirection;
    Color color;
    float timer;
    const float interval = .2;

public:
    Brightness();
    Color TintColor();
};