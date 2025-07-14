//Pickup.cpp
#include "Pickup.h"
#include <raylib.h>

// — static members —
std::vector<Pickup> Pickup::pickups;
Texture2D           Pickup::texSushi{};
Texture2D           Pickup::texBattery{};
Texture2D           Pickup::texMilk{};
bool                Pickup::isLoaded = false;
bool                Pickup::batteryDropped = false;

// ------------------------------------------------------------
// Lazy-load textures
// ------------------------------------------------------------
void Pickup::LoadTextures() {
    if (isLoaded) return;
    texSushi = LoadTexture("image/sushi.png");
    texBattery = LoadTexture("image/battery.png");
    texMilk = LoadTexture("image/milk.png");
    isLoaded = true;
}

// ------------------------------------------------------------
// private ctor
// ------------------------------------------------------------
Pickup::Pickup(PickupType t, const Vector2& at)
    : type(t), position(at), velocity({ 0, 200 }), lifeTime(0.0f), hasBounced(false)
{
}

// ------------------------------------------------------------
// Spawn: decide loại
// ------------------------------------------------------------
void Pickup::Spawn(const Vector2& at) {
    LoadTextures();

    PickupType chosen;
    if (!batteryDropped) {
        // 1/3 chance rớt battery, chỉ 1 lần duy nhất
        int r = GetRandomValue(1, 3);
        if (r == 1) {
            chosen = PickupType::Battery;
            batteryDropped = true;
        }
        else {
            // còn lại chia đều milk/sushi
            chosen = (GetRandomValue(0, 1) == 0 ? PickupType::Milk : PickupType::Sushi);
        }
    }
    else {
        // đã rớt battery rồi: chỉ milk/sushi 50/50
        chosen = (GetRandomValue(0, 1) == 0 ? PickupType::Milk : PickupType::Sushi);
    }

    pickups.emplace_back(chosen, at);
}

// ------------------------------------------------------------
// UpdateAll & remove sau 4s
// ------------------------------------------------------------
void Pickup::UpdateAll(float dt) {
    LoadTextures();
    float groundY = (float)GetScreenHeight() - 50;
    for (int i = 0; i < (int)pickups.size(); ++i) {
        pickups[i].Update(dt);
        pickups[i].lifeTime += dt;
        if (pickups[i].lifeTime >= 4.0f) {
            pickups.erase(pickups.begin() + i);
            --i;
        }
    }
}

// ------------------------------------------------------------
// DrawAll
// ------------------------------------------------------------
void Pickup::DrawAll() {
    for (auto& p : pickups)
        p.Draw();
}

// ------------------------------------------------------------
// instance Update: rơi, bounce, trượt
// ------------------------------------------------------------
void Pickup::Update(float dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    float groundY = (float)GetScreenHeight() - 50;
    if (!hasBounced && position.y >= groundY) {
        hasBounced = true;
        position.y = groundY;
        velocity.y = -velocity.y * 0.5f;
        velocity.x = (GetRandomValue(0, 1) ? 1.f : -1.f) * 100.f;
    }
    if (hasBounced) {
        velocity.y += 800.f * dt;
        if (fabsf(velocity.y) < 50.f) velocity.y = 0;
    }
}

// ------------------------------------------------------------
// instance Draw
// ------------------------------------------------------------
void Pickup::Draw() const {
    Texture2D tex;
    switch (type) {
    case PickupType::Sushi:   tex = texSushi;   break;
    case PickupType::Battery: tex = texBattery; break;
    case PickupType::Milk:    tex = texMilk;    break;
    }
    const float size = 40.f;
    DrawTexturePro(
        tex,
        { 0, 0, (float)tex.width, (float)tex.height },
        { position.x - size / 2, position.y - size / 2, size, size },
        { size / 2, size / 2 },
        0.f,
        WHITE
    );
}
