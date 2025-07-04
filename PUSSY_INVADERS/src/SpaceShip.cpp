#include "SpaceShip.h"
#include <random>

int SpaceShip::heat_limit = 2000;

SpaceShip::SpaceShip()
{
    live_counter = 5;   // max health 10
    missile_counter = 0;    // max missile 10
    weapon_level = 0;   // max level 20
    thigh_counter = 0;  // as many as possible
    overheat = 0;   // heat limit is 2000
    score = 0;  // can be any non-negative number
    InsertSpaceShipTexture();
    font = LoadFont("font/ChonkyBitsFontBold.otf");

    timer = 0.0f; // removable
}

void SpaceShip::SetAttribute()
{
    live_counter = 5;   // max health 10
    missile_counter = 0;    // max missile 10
    weapon_level = 0;   // max level 20
    thigh_counter = 0;  // as many as possible
    overheat = 0;   // heat limit is 2000
    score = 0;  // can be any non-negative number
    InsertSpaceShipTexture();
    font = LoadFont("font/ChonkyBitsFontBold.otf");

    timer = 0.0f; // removable
}

void SpaceShip::InsertSpaceShipTexture()
{
    image.SetTexture();
    return;
}

SpaceShip::~SpaceShip()
{
    UnloadFont(font);
}

Texture2D SpaceShip::GetShip() const { return image.ship.texture; }

Texture2D SpaceShip::GetFireball() const { return image.fireball.texture; }

void SpaceShip::Moving()
{
    Vector2 mouse = GetMousePosition();
    Vector2 pos = HitBoxChecking(mouse);
    DrawTextureEx(image.ship.texture, pos, 0.0f, 0.2f, WHITE);

    Vector2 pos1 = RocketPosition(pos);
    Color tint = fireball_brightness.TintColor();
    DrawTextureEx(image.fireball.texture, pos1, 0.0f, 0.2f, tint);
    return;
}

void SpaceShip::StatusBar()
{
    DrawTextEx(font, TextFormat("SCORE: %llu", score), { 0.0f, 0.0f }, 40.0f, 0.0f, GREEN);
    DrawTextEx(font, TextFormat("HEAT: %d/%d", overheat, heat_limit), { 0.0f, 40.0f }, 40.0f, 0.0f, GREEN);
    DrawTextureEx(image.bar.texture, image.bar.pos, 0.0f, 1.2f, WHITE);

    Vector2 pos;
    DrawTextureEx(image.live.texture, image.live.pos, 0.0f, 0.02f, WHITE);
    pos.x = image.live.pos.x + static_cast<float>(image.live.texture.width) * 0.02f;
    pos.y = static_cast<float>(GetScreenHeight() - 50);
    DrawTextEx(font, TextFormat("\t%d\t", live_counter), pos, 40.0f, 0.0f, GREEN);

    DrawTextureEx(image.missile.texture, image.missile.pos, 0.0f, 0.1f, WHITE);
    pos.x = image.missile.pos.x + static_cast<float>(image.missile.texture.width) * 0.1f;
    pos.y = static_cast<float>(GetScreenHeight() - 50);
    DrawTextEx(font, TextFormat("\t%d\t", missile_counter), pos, 40.0f, 0.0f, GREEN);

    DrawTextureEx(image.thigh.texture, image.thigh.pos, 0.0f, 0.07f, WHITE);
    pos.x = image.thigh.pos.x + static_cast<float>(image.thigh.texture.width) * 0.07f;
    pos.y = static_cast<float>(GetScreenHeight() - 50);
    DrawTextEx(font, TextFormat("\t%d\t", thigh_counter), pos, 40.0f, 0.0f, GREEN);

    DrawTextureEx(image.level.texture, image.level.pos, 0.0f, 0.065f, WHITE);
    pos.x = image.level.pos.x + static_cast<float>(image.level.texture.width) * 0.065f;
    pos.y = static_cast<float>(GetScreenHeight() - 50);
    DrawTextEx(font, TextFormat("\t%d\t", weapon_level), pos, 40.0f, 0.0f, GREEN);

    timer += GetFrameTime();
    if (timer >= 0.2f)
    {
        timer = 0.0f;
        UpdateStatus();
    }
    return;
}

void SpaceShip::UpdateStatus()
{
    std::mt19937 engine(std::random_device{}());

    std::uniform_int_distribution<int> live_dist(0, 10);
    std::uniform_int_distribution<int> missile_dist(0, 10);
    std::uniform_int_distribution<int> weapon_dist(0, 20);
    std::uniform_int_distribution<int> thigh_dist(0, 100); // Example upper bound for "as many as possible"
    std::uniform_int_distribution<int> overheat_dist(0, 2000);
    std::uniform_int_distribution<int> score_dist(0, 100000); // Example upper bound for score

    live_counter = live_dist(engine);
    missile_counter = missile_dist(engine);
    weapon_level = weapon_dist(engine);
    thigh_counter = thigh_dist(engine);
    overheat = overheat_dist(engine);
    score = score_dist(engine);
}

Vector2 SpaceShip::HitBoxChecking(Vector2 mouse)
{
    float width = static_cast<float>(image.ship.texture.width) * 0.2f;
    float height = static_cast<float>(image.ship.texture.height) * 0.2f;

    Vector2 pos;
    pos.x = mouse.x - width / 2.0f;
    pos.y = mouse.y - height / 2.0f;

    if (mouse.x - width / 2.0f < 0.0f)
        pos.x = 0.0f;
    if (mouse.y - height / 2.0f < 0.0f)
        pos.y = 0.0f;

    if (mouse.x + width / 2.0f > static_cast<float>(GetScreenWidth()))
        pos.x = static_cast<float>(GetScreenWidth()) - width;
    if (mouse.y + height / 2.0f > static_cast<float>(GetScreenHeight()))
        pos.y = static_cast<float>(GetScreenHeight()) - height;

    return pos;
}

Vector2 SpaceShip::RocketPosition(Vector2 topleft)
{
    float Swidth = static_cast<float>(image.ship.texture.width) * 0.2f;
    float Sheight = static_cast<float>(image.ship.texture.height) * 0.2f;
    float Fwidth = static_cast<float>(image.fireball.texture.width) * 0.2f;
    Vector2 pos;
    pos.y = topleft.y + Sheight;
    pos.x = topleft.x + Swidth / 2.0f - Fwidth / 2.0f;
    return pos;
}

void SpaceShip::Shooting(std::vector<Bullet>& bullets, Texture2D* bulletTexture)
{
    Vector2 mouse = GetMousePosition();
    Vector2 pos = HitBoxChecking(mouse);
    float scale = 0.2f;
    Vector2 center;
    center.x = pos.x + static_cast<float>(image.ship.texture.width) * scale / 2.0f;
    center.y = pos.y + static_cast<float>(image.ship.texture.height) * scale / 2.0f;
    bullets.emplace_back(center, bulletTexture);
}