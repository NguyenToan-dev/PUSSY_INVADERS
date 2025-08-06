#include "SpaceShipTexture.h"
#include<cmath>

SpaceShipTexture::SpaceShipTexture() {}

void SpaceShipTexture::SetTexture()
{
    ship.texture = LoadTexture("image/spaceship.png");
    fireball.texture = LoadTexture("image/fireball.png");
    live.texture = LoadTexture("image/live.png");
    level.texture = LoadTexture("image/level.png");
    missile.texture = LoadTexture("image/missile.png");
    thigh.texture = LoadTexture("image/thigh.png");
    bar.texture = LoadTexture("image/bar.png");

    Vector2 pos = GetPosition(20, live.texture.height * .02);
    live.pos = pos;
    pos = GetPosition(20 + 150, level.texture.height * .065);
    level.pos = pos;
    pos = GetPosition(20 + 150 * 2, missile.texture.height * .1);
    missile.pos = pos;
    pos = GetPosition(20 + 150 * 3, thigh.texture.height * .07);
    thigh.pos = pos;
    bar.pos = { 0, (float)GetScreenHeight() - 90 };
}

Vector2 SpaceShipTexture::GetPosition(float x, float height)
{
    Vector2 pos;
    pos.x = x;
    pos.y = (float)(GetScreenHeight() - ceil(height)) - 10;
    return pos;
}

SpaceShipTexture::~SpaceShipTexture()
{
    UnloadTexture(ship.texture);
    UnloadTexture(fireball.texture);
    UnloadTexture(level.texture);
    UnloadTexture(missile.texture);
    UnloadTexture(thigh.texture);
    UnloadTexture(live.texture);
}