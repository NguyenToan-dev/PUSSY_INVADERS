#include "Pickup.h"
#include <raylib.h>

// — static members —
std::vector<Pickup> Pickup::pickups;
Texture2D Pickup::texSushi{};
Texture2D Pickup::texBattery{};
Texture2D Pickup::texMilk{};
Texture2D Pickup::texGift1{};
Texture2D Pickup::texGift2{};
Texture2D Pickup::texGift3{};
bool Pickup::isLoaded = false;
bool Pickup::batteryDropped = false;
float Pickup::giftTimer = 0.0f;

const std::vector<Pickup>& Pickup::GetAll() {
    return pickups;
}

Vector2 Pickup::GetPosition() const {
    return position;
}

PickupType Pickup::GetType() const {
    return type;
}

void Pickup::RemoveAt(int index) {
    if (index >= 0 && index < (int)pickups.size()) {
        pickups.erase(pickups.begin() + index);
    }
}


// ------------------------------------------------------------
// load textures
// ------------------------------------------------------------
void Pickup::LoadTextures() {
    if (isLoaded) return;
    texSushi = LoadTexture("image/sushi.png");
    texBattery = LoadTexture("image/battery.png");
    texMilk = LoadTexture("image/milk.png");

    texGift1 = LoadTexture("image/gift-1.png");
    texGift2 = LoadTexture("image/gift-2.png");
    texGift3 = LoadTexture("image/gift-3.png");

    isLoaded = true;
}

// ------------------------------------------------------------
// private ctor
// ------------------------------------------------------------
Pickup::Pickup(PickupType t, const Vector2& at)
    : type(t), position(at), velocity({ 0, 200 }), lifeTime(0.0f),
    hasBounced(false), bounceCount(0), rotationAngle(0.0f)
{
    if (t == PickupType::Gift1 || t == PickupType::Gift2 || t == PickupType::Gift3) {
        velocity = { 0, 50 };  // tốc độ rơi khởi đầu quà rất chậm
    }
}



// ------------------------------------------------------------
// Spawn: decide loại
// ------------------------------------------------------------
void Pickup::Spawn(const Vector2& at) {
    LoadTextures();

    PickupType chosen;

    if (!batteryDropped) {
        int r = GetRandomValue(1, 5); // 1/5 xác suất để chọn Battery (20%)
        if (r == 1) {
            chosen = PickupType::Battery;
            batteryDropped = true;
        }
        else {
            // Chọn giữa Sushi (80%) và Milk (20%)
            int m = GetRandomValue(1, 100);
            chosen = (m <= 20) ? PickupType::Milk : PickupType::Sushi;
        }
    }
    else {
        // Battery đã rớt rồi -> chỉ chọn giữa Milk và Sushi
        int m = GetRandomValue(1, 100);
        chosen = (m <= 20) ? PickupType::Milk : PickupType::Sushi;
    }

    pickups.emplace_back(chosen, at);
}


// ------------------------------------------------------------
// SpawnGift: tự động từ Pickup
// ------------------------------------------------------------
void Pickup::SpawnGift() {
    int r = GetRandomValue(1, 3);
    PickupType chosen;
    switch (r) {
    case 1: chosen = PickupType::Gift1; break;
    case 2: chosen = PickupType::Gift2; break;
    default: chosen = PickupType::Gift3; break;
    }

    Vector2 at = { (float)GetRandomValue(50, GetScreenWidth() - 50), -30 };
    pickups.emplace_back(chosen, at);
}

// ------------------------------------------------------------
// UpdateAll & remove sau 4s
// ------------------------------------------------------------
void Pickup::UpdateAll(float dt) {
    LoadTextures();

    giftTimer += dt;
    if (giftTimer >= 15.0f) {
        giftTimer = 0.f;
        SpawnGift();
    }

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
// instance Update: rơi, bounce, trượt + xoay
// ------------------------------------------------------------
void Pickup::Update(float dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    float groundY = (float)GetScreenHeight() - 50;

    if (type == PickupType::Gift1 || type == PickupType::Gift2 || type == PickupType::Gift3) {
        // Gift rơi thẳng, biến mất khi chạm đáy
        if (position.y >= groundY) {
            position.y = groundY;
            velocity = { 0, 0 };
            lifeTime = 4.0f;  // đánh dấu để xóa trong UpdateAll
        }
        else {
            // Giảm tốc độ rơi bằng cách giảm trọng lực 
            velocity.y += 100.f * dt;
        }
    }

    else {
        // Bounce 2 lần: cao -> thấp -> dừng
        if (position.y >= groundY) {
            if (bounceCount == 0) {
                // Lần 1: bounce cao
                bounceCount++;
                position.y = groundY;
                velocity.y = -400.f;
                velocity.x = (GetRandomValue(0, 1) ? 1.f : -1.f) * 80.f;
            }
            else if (bounceCount == 1) {
                // Lần 2: bounce thấp hơn
                bounceCount++;
                position.y = groundY;
                velocity.y = -250.f;
                velocity.x *= 0.5f;  // giảm trượt
            }
            else {
                // Lần 3: dừng
                position.y = groundY;
                velocity = { 0, 0 };
            }
        }

        // Trọng lực
        velocity.y += 800.f * dt;

        // Nếu tốc độ quá nhỏ thì cho dừng hẳn
        if (bounceCount >= 2 && fabsf(velocity.y) < 30.f) {
            velocity.y = 0;
        }
    }

    // Góc xoay
    rotationAngle += 180.f * dt;
    if (rotationAngle >= 360.f) rotationAngle -= 360.f;
}


// ------------------------------------------------------------
// instance Draw (có xoay)
// ------------------------------------------------------------
void Pickup::Draw() const {
    Texture2D tex;
    switch (type) {
    case PickupType::Sushi:   tex = texSushi;   break;
    case PickupType::Battery: tex = texBattery; break;
    case PickupType::Milk:    tex = texMilk;    break;
    case PickupType::Gift1:   tex = texGift1;   break;
    case PickupType::Gift2:   tex = texGift2;   break;
    case PickupType::Gift3:   tex = texGift3;   break;
    }

    const float size = 40.f;
    DrawTexturePro(
        tex,
        { 0, 0, (float)tex.width, (float)tex.height },
        { position.x - size / 2, position.y - size / 2, size, size },
        { size / 2, size / 2 },
        rotationAngle,
        WHITE
    );
}
