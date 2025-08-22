#ifndef METEOR_H
#define METEOR_H

#include "raylib.h"
#include "Bullet.h"
#include <vector>

class Meteor {
public:
    Vector2 position;
    Vector2 velocity;
    float scale;
    float rotation = 0.0f;
    float rotationSpeed;
    Texture2D* texture;
    float health; // Thêm thuộc tính máu

    Meteor(Vector2 pos, Vector2 vel, float s, Texture2D* tex, float rotSpeed);
    void Update(float dt);
    void Draw() const;
    bool IsOffScreen(int sw, int sh) const;
    Rectangle GetRect() const;
};

class MeteorManager {
public:
    static void Init();
    static void Unload();
    static void StartWaves();
    static void UpdateAll(float dt);
    static void DrawAll();
    static bool CheckCollisionWithShip(const Rectangle& shipRect);
    static void CheckCollisionWithBullets(std::vector<Bullet>& bullets);
};

#endif