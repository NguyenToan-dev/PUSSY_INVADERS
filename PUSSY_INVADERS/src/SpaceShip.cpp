//SpcaeShip.cpp
#include "SpaceShip.h"
#include "Pickup.h"
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

    //timer = 0.0f; // removable
}

void SpaceShip::SetAttribute()
{
    live_counter = 5;   
    missile_counter = 0;    
    weapon_level = 0;   
    thigh_counter = 0;  
    overheat = 0;   
    score = 0;  
    InsertSpaceShipTexture();
    font = LoadFont("font/ChonkyBitsFontBold.otf");

    //timer = 0.0f; // removable
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
    Vector2 pos = BoundChecking(mouse);
    DrawTextureEx(image.ship.texture, pos, 0.0f, 0.2f, WHITE);

    Vector2 pos1 = RocketPosition(pos);
    Color tint = fireball_brightness.TintColor();
    DrawTextureEx(image.fireball.texture, pos1, 0.0f, 0.2f, tint);
    return;
}

void SpaceShip::MovingWhileBlinking(Color shiptint)
{
    Vector2 mouse = GetMousePosition();
    Vector2 pos = BoundChecking(mouse);
    DrawTextureEx(image.ship.texture, pos, 0.0f, 0.2f, shiptint);

    Vector2 pos1 = RocketPosition(pos);
    DrawTextureEx(image.fireball.texture, pos1, 0.0f, 0.2f, shiptint);
    return;
}

void SpaceShip::StatusBar()
{
    UpdateStatus(HEAT_DECREASE);
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
}

void SpaceShip::UpdateStatus(ShipStatus flag)
{
    if(flag == HEAT_DECREASE)
    {
        overheat -= 5;
        if(overheat <= 0)
        overheat = 0;   
    }
    else if(flag == HEAT_INCREASE)
    {
        if(overheat >= heat_limit)
        {
            overheat = heat_limit;
            return;
        }
        overheat += 40;
    }  
    else if(flag >= SCORE_GAIN_1 && flag <= SCORE_GAIN_3)   // gain score depends on the type of enemy.
    {
        if(flag == SCORE_GAIN_1)
            score += 50;
        else if(flag == SCORE_GAIN_2)
            score += 100;
        else if(flag == SCORE_GAIN_3)
            score += 150;
    }
    else if(flag == LIVE_DECREASE)
    {
        live_counter--;
        if(live_counter <= 0)
            live_counter = 0;
    }    
    else if(flag == LEVEL_UP)
    {
        if(weapon_level >= 5)
        {
            weapon_level = 5;
            return;
        }
        weapon_level++;
    }  
    else if(flag == MISSILE_ADD)
    {
        if(missile_counter >= 10)
        {
            missile_counter = 10;
            return;
        }
        missile_counter++;
    }
    else if (flag == SUSHI_ADD)
    {
        sushi_collected++;

        int totalSushiValue = sushi_collected + milk_collected * 5;
        while (totalSushiValue >= 10) {
            missile_counter++;
            if (missile_counter > 10) missile_counter = 10;

            // Trừ sushi
            if (sushi_collected >= 10) {
                sushi_collected -= 10;
            }
            else {
                int remaining = 10 - sushi_collected;
                sushi_collected = 0;
                int milkToUse = (remaining + 4) / 5; // làm tròn lên
                milk_collected -= milkToUse;
                if (milk_collected < 0) milk_collected = 0;
            }

            totalSushiValue = sushi_collected + milk_collected * 5;
        }
    }
    else if (flag == MILK_ADD)
    {
        milk_collected++;

        int totalSushiValue = sushi_collected + milk_collected * 5;
        while (totalSushiValue >= 10) {
            missile_counter++;
            if (missile_counter > 10) missile_counter = 10;

            // Trừ sushi
            if (sushi_collected >= 10) {
                sushi_collected -= 10;
            }
            else {
                int remaining = 10 - sushi_collected;
                sushi_collected = 0;
                int milkToUse = (remaining + 4) / 5;
                milk_collected -= milkToUse;
                if (milk_collected < 0) milk_collected = 0;
            }

            totalSushiValue = sushi_collected + milk_collected * 5;
        }
    }
    else if (flag == NEW_BULLET)
    {
        // logic thay đổi đạn – ví dụ như đổi texture, bulletType, hoặc đặt 1 flag để kiểm tra
        // giả định: có biến bool isNewBullet
        // isNewBullet = true;
    }
    else if (flag == LEVEL_UP)
    {
        battery_collected++;
        if (weapon_level >= 5)
            weapon_level = 5;
        else
            weapon_level++;
    }

    return;
}

void SpaceShip::AdjustStatus(ShipStatus flag)
{
    if(flag <= 10 && flag >= 0)
        UpdateStatus(flag);
    return;
}

Vector2 SpaceShip::BoundChecking(Vector2 mouse)
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
    UpdateStatus(HEAT_INCREASE);
    Vector2 mouse = GetMousePosition();
    Vector2 pos = BoundChecking(mouse);
    float scale = 0.2f;
    Vector2 center;
    center.x = pos.x + static_cast<float>(image.ship.texture.width) * scale / 2.0f;
    center.y = pos.y + static_cast<float>(image.ship.texture.height) * scale / 2.0f;
    bullets.emplace_back(center, bulletTexture);
}

Rectangle SpaceShip::getRect()
{
    Vector2 mouse = GetMousePosition();
    Rectangle ret;
    ret.x = mouse.x - image.ship.texture.height * .2f /2.0;
    ret.y = mouse.y - image.ship.texture.width * .2f /2.0;
    ret.width = image.ship.texture.width * .2f;
    ret.height  = image.ship.texture.height * .2f;
    return ret;
}

int SpaceShip::HitBoxChecking(vector<Bullet*> &bullets)
{
    for(auto &bullet : bullets)
    {
        if(CheckCollisionRecs(bullet->getRect(), getRect()))
        {
            bullet->active = false;
            bullet->position.x = -9999;
            UpdateStatus(LIVE_DECREASE);
            if(live_counter == 0)
                return 2;
            return 1;
        }
    }
    return 0;
}
void SpaceShip::EatPickup()
{
    Rectangle shipRect = getRect();
    const auto& allPickups = Pickup::GetAll();

    for (int i = 0; i < (int)allPickups.size(); ++i) {
        Vector2 pickupPos = allPickups[i].GetPosition();
        float size = 40.f;

        Rectangle pickupRect = {
            pickupPos.x - size / 2,
            pickupPos.y - size / 2,
            size,
            size
        };

        if (CheckCollisionRecs(shipRect, pickupRect)) {
            PickupType type = allPickups[i].GetType();
            switch (type) {
            case PickupType::Sushi:
                AdjustStatus(SUSHI_ADD); break;
            case PickupType::Milk:
                AdjustStatus(MILK_ADD); break;
            case PickupType::Battery:
                AdjustStatus(LEVEL_UP); break;
            case PickupType::Gift1:
            case PickupType::Gift2:
            case PickupType::Gift3:
                AdjustStatus(NEW_BULLET); break;
            default:
                break;
            }

            // Xóa pickup thông qua hàm tĩnh
            Pickup::RemoveAt(i);
            --i;
        }
    }
}

