#pragma once
#include "raylib.h"

class Bullet {
public:
    Bullet(Vector2 pos, Texture2D* tex);
    virtual void Update();
    virtual void Draw();
    virtual Rectangle getRect() const;

    // Getter methods
    Vector2 GetPosition() const { return position; }
    float GetSpeed() const { return speed; }
    bool IsActive() const { return active; }
    Texture2D* GetTexture() const { return texture; }
    int GetDamage() const { return damage; }

    // Setter methods
    void SetActive(bool isActive) { active = isActive; }
    void SetPosition(Vector2 pos) { position = pos; }
    void SetSpeed(float s) { speed = s; }
    void SetDamage(int dmg) { damage = dmg; }

protected:
    Vector2 position;
    float speed;
    bool active;
    Texture2D* texture;
    int damage;
};