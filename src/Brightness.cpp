#include "Brightness.h"

Brightness::Brightness()
{
    brightness = 1.0;
    brightnessSpeed = .02;
    brightnessDirection = 1.0;
    timer = .0;
    color = WHITE;
}

Color Brightness::TintColor()
{
    Color tint = color;
    timer += GetFrameTime();
    if (timer >= interval)
    {
        timer = .0;
        brightness += brightnessSpeed * brightnessDirection;

        if (brightness > 1.0)
        {
            brightness = 1.0;
            brightnessDirection = -1.0;
        }
        else if (brightness < .9)
        {
            brightness = .9;
            brightnessDirection = 1.0;
        }

        unsigned char value = (unsigned char)(brightness * 255);
        tint = color = { value, value, value, 255 };
    }
    return tint;
}