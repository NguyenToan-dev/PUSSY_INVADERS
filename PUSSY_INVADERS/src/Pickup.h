//Pickup.h
#pragma once
#include <raylib.h>
#include <vector>

enum class PickupType {
    Sushi,
    Battery,
    Milk,
    Gift1,
    Gift2,
    Gift3
};


class Pickup {
public:
    Pickup(PickupType t, const Vector2& at);
    // spawn một pickup tại vị trí at
    static void Spawn(const Vector2& at);
    //spawn gift sau 15s
    static void SpawnGift();

    // update tất cả – gọi trong GameController::Update()
    static void UpdateAll(float dt);

    // draw tất cả – gọi trong GameController::Draw()
    static void DrawAll();

private:
    // instance update & draw
    void Update(float dt);
    void Draw() const;

    // dữ liệu instance
    Vector2    position;
    Vector2    velocity;
    float      lifeTime;
    bool       hasBounced;
    PickupType type;
    float rotationAngle;  // góc xoay hiện tại (theo độ)

    // container và textures chung
    static std::vector<Pickup> pickups;
    static Texture2D           texSushi, texBattery, texMilk;
    static Texture2D texGift1, texGift2, texGift3;
    static float giftTimer;
    static bool                isLoaded;
    // để đảm bảo battery chỉ rớt 1 lần
    static bool                batteryDropped;

    static void LoadTextures();
};
