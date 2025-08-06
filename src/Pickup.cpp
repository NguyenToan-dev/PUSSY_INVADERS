#include "Pickup.h"
#include <raylib.h>
#include <cmath>
// — static members initialization —
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

// Trả về danh sách pickup hiện có
const std::vector<Pickup>& Pickup::GetAll() {
    return pickups;
}

// Getter vị trí
Vector2 Pickup::GetPosition() const {
    return position;
}

// Getter loại
PickupType Pickup::GetType() const {
    return type;
}

// Xóa pickup tại index khỏi vector
void Pickup::RemoveAt(int index) {
    if (index >= 0 && index < (int)pickups.size()) {
        pickups.erase(pickups.begin() + index);
    }
}

// ------------------------------------------------------------
// Load các texture – chỉ load 1 lần duy nhất
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
// Constructor: khởi tạo một pickup tại vị trí `at` với loại `t`
// ------------------------------------------------------------
Pickup::Pickup(PickupType t, const Vector2& at)
    : type(t), position(at), velocity({ 0, 200 }), lifeTime(0.0f),
    hasBounced(false), bounceCount(0), rotationAngle(0.0f)
{
    // Nếu là Gift thì rơi chậm hơn nhiều
    if (t == PickupType::Gift1 || t == PickupType::Gift2 || t == PickupType::Gift3) {
        velocity = { 0, 50 };
    }
}

// ------------------------------------------------------------
// Tạo ngẫu nhiên một pickup tại vị trí `at`
// - Có 20% cơ hội là Battery (nếu chưa từng rơi)
// - Còn lại là Sushi (80%) hoặc Milk (20%)
// ------------------------------------------------------------
void Pickup::Spawn(const Vector2& at) {
    LoadTextures();

    PickupType chosen;

    if (!batteryDropped) {
        int r = GetRandomValue(1, 5); // 1/5 -> Battery
        if (r == 1) {
            chosen = PickupType::Battery;
            batteryDropped = true;
        }
        else {
            int m = GetRandomValue(1, 100);
            chosen = (m <= 20) ? PickupType::Milk : PickupType::Sushi;
        }
    }
    else {
        // Battery đã rơi rồi -> bỏ qua
        int m = GetRandomValue(1, 100);
        chosen = (m <= 20) ? PickupType::Milk : PickupType::Sushi;
    }

    pickups.emplace_back(chosen, at);
}

// ------------------------------------------------------------
// Tự động sinh gift sau mỗi 15s
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
// Cập nhật tất cả pickup: di chuyển, xóa nếu quá hạn
// - Tạo gift sau mỗi 15s
// - Xóa pickup sau 4s
// ------------------------------------------------------------
void Pickup::UpdateAll(float dt) {
    LoadTextures();

    // Gift timer – đếm 15s
    giftTimer += dt;
    if (giftTimer >= 15.0f) {
        giftTimer = 0.f;
        SpawnGift();
    }

    for (int i = 0; i < (int)pickups.size(); ++i) {
        pickups[i].Update(dt);
        pickups[i].lifeTime += dt;

        // Sau 4 giây thì tự động biến mất
        if (pickups[i].lifeTime >= 4.0f) {
            pickups.erase(pickups.begin() + i);
            --i;
        }
    }
}

// ------------------------------------------------------------
// Vẽ toàn bộ pickup ra màn hình
// ------------------------------------------------------------
void Pickup::DrawAll() {
    for (auto& p : pickups)
        p.Draw();
}

// ------------------------------------------------------------
// Cập nhật vị trí, bounce, rơi, xoay của một pickup
// ------------------------------------------------------------
void Pickup::Update(float dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    float groundY = (float)GetScreenHeight() - 50;

    if (type == PickupType::Gift1 || type == PickupType::Gift2 || type == PickupType::Gift3) {
        // Gift: rơi thẳng, không nảy
        if (position.y >= groundY) {
            position.y = groundY;
            velocity = { 0, 0 };
            lifeTime = 4.0f; // đánh dấu để xóa
        }
        else {
            velocity.y += 100.f * dt; // trọng lực nhẹ
        }
    }

    else {
        // Sushi/Milk/Battery – có hiệu ứng nảy
        if (position.y >= groundY) {
            if (bounceCount == 0) {
                // Bounce lần 1: cao và trượt mạnh
                bounceCount++;
                position.y = groundY;
                velocity.y = -400.f;
                velocity.x = (GetRandomValue(0, 1) ? 1.f : -1.f) * 80.f;
            }
            else if (bounceCount == 1) {
                // Bounce lần 2: thấp hơn
                bounceCount++;
                position.y = groundY;
                velocity.y = -250.f;
                velocity.x *= 0.5f;
            }
            else {
                // Sau 2 lần thì đứng yên
                position.y = groundY;
                velocity = { 0, 0 };
            }
        }

        // Trọng lực
        velocity.y += 800.f * dt;

        // Nếu rơi quá chậm thì dừng luôn
        if (bounceCount >= 2 && fabsf(velocity.y) < 30.f) {
            velocity.y = 0;
        }
    }

    // Xoay hình ảnh
    rotationAngle += 180.f * dt;
    if (rotationAngle >= 360.f) rotationAngle -= 360.f;
}

// ------------------------------------------------------------
// Vẽ 1 pickup với xoay
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
