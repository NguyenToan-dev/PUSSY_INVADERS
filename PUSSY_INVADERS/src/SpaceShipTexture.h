#pragma once
#include<raylib.h>

class Element
{
public:
    Texture2D texture;
    Vector2 pos;
};

class SpaceShipTexture
{
public:
    Element ship;
    Element fireball;
    Element live;
    Element level;
    Element thigh;
    Element missile;
    Element bar;

public:
    SpaceShipTexture();
    void SetTexture();
    Vector2 GetPosition(float, float);
    ~SpaceShipTexture();
};