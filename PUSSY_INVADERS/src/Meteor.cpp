#include "Meteor.h"
#include "raylib.h"
#include <cmath>
#include <cstdlib>

static std::vector<Meteor> meteors;
static Texture2D meteorTextures[4];
static bool initialized = false;

static int wavePhase = 1;
static float waveTimer = 0.0f;
static const float waveDurations[4] = { 15.0f, 10.0f, 8.0f, 5.0f };
static float spawnTimer = 0.0f;
static float spawnInterval = 0.5f;

Meteor::Meteor(Vector2 pos, Vector2 vel, float s, Texture2D* tex, float rotSpeed)
    : position(pos), velocity(vel), scale(s), texture(tex), rotationSpeed(rotSpeed)
{
    // Ánh xạ scale từ [0.3, 1.0] sang health từ [1, 4]
    health = 1.0f + (scale - 0.3f) * 3.0f / 0.7f;
}

void Meteor::Update(float dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    rotation += rotationSpeed * dt;
}

void Meteor::Draw() const {
    if (texture) {
        DrawTextureEx(*texture, position, rotation * 180.0f / PI, scale, WHITE);
    }
}

bool Meteor::IsOffScreen(int sw, int sh) const {
    return position.x < -100 || position.x > sw + 100 ||
        position.y < -100 || position.y > sh + 100;
}

Rectangle Meteor::GetRect() const {
    if (!texture) return { 0,0,0,0 };
    return { position.x, position.y,
             texture->width * scale,
             texture->height * scale };
}

void MeteorManager::Init() {
    if (initialized) return;
    for (int i = 0; i < 4; i++) {
        meteorTextures[i] = LoadTexture(TextFormat("image/meteor%d.png", i + 1));
    }
    initialized = true;
}

void MeteorManager::Unload() {
    for (int i = 0; i < 4; i++) UnloadTexture(meteorTextures[i]);
    meteors.clear();
    initialized = false;
}

void MeteorManager::StartWaves() {
    wavePhase = 1;
    waveTimer = 0.0f;
    spawnInterval = 1.0f;
    meteors.clear();
}

void MeteorManager::UpdateAll(float dt) {
    if (!initialized) Init();

    waveTimer += dt;
    if (wavePhase <= 4 && waveTimer >= waveDurations[wavePhase - 1]) {
        wavePhase++;
        waveTimer = 0.0f;
        spawnInterval = std::max(0.3f, spawnInterval - 0.05f);
        meteors.clear();
    }
    if (wavePhase > 4) return;

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        Vector2 pos, vel;
        float speedBase = 150.0f + wavePhase * 50.0f;
        float angleRad = 0.0f;

        switch (wavePhase) {
        case 1:
            pos = { (float)GetRandomValue(0, GetScreenWidth()), -50 };
            vel = { 0, speedBase };
            break;
        case 2:
            pos = { (float)(GetScreenWidth() + 50), (float)GetRandomValue(0, GetScreenHeight()) };
            angleRad = atan2f(-(GetScreenHeight() + 100), -(GetScreenWidth() + 100));
            vel = { cosf(angleRad) * speedBase, sinf(angleRad) * speedBase };
            break;
        case 3:
            pos = { static_cast<float>(GetRandomValue(0, GetScreenWidth())), static_cast<float>(GetScreenHeight() + 50) };
            vel = { 0, -speedBase };
            break;
        case 4:
        default:
            pos = {
                (float)GetRandomValue(-50, GetScreenWidth() + 50),
                (float)GetRandomValue(-50, GetScreenHeight() + 50)
            };
            angleRad = GetRandomValue(0, 360) * (PI / 180.0f);
            vel = { cosf(angleRad) * (speedBase * 2), sinf(angleRad) * (speedBase * 2) };
            break;
        }

        int ti = (wavePhase - 1) % 4;
        Texture2D* tex = &meteorTextures[ti];
        float maxScale = (GetScreenHeight() / 8.0f) / tex->height;
        float t = GetRandomValue(30, 100) / 100.0f;
        float scale = maxScale * t;
        float rotSpeed = (GetRandomValue(10, 60) * (PI / 180.0f));

        meteors.emplace_back(pos, vel, scale, tex, rotSpeed);
    }

    for (int i = 0; i < (int)meteors.size(); i++) {
        meteors[i].Update(dt);
        if (meteors[i].IsOffScreen(GetScreenWidth(), GetScreenHeight())) {
            meteors.erase(meteors.begin() + i);
            i--;
        }
    }
}

void MeteorManager::DrawAll() {
    if (!initialized) return;
    for (auto& m : meteors) m.Draw();
}

bool MeteorManager::CheckCollisionWithShip(const Rectangle& shipRect) {
    if (!initialized) return false;

    for (auto it = meteors.begin(); it != meteors.end(); ++it) {
        if (CheckCollisionRecs(shipRect, it->GetRect())) {
            meteors.erase(it);
            return true;
        }
    }
    return false;
}

void MeteorManager::CheckCollisionWithBullets(std::vector<Bullet>& bullets) {
    if (!initialized) return;

    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        if (!bulletIt->IsActive()) {
            ++bulletIt;
            continue;
        }

        bool bulletHit = false;
        for (auto meteorIt = meteors.begin(); meteorIt != meteors.end(); ) {
            if (CheckCollisionRecs(bulletIt->getRect(), meteorIt->GetRect())) {
                meteorIt->health -= bulletIt->GetDamage();
                bulletIt->SetActive(false);
                bulletHit = true;
                if (meteorIt->health <= 0) {
                    meteorIt = meteors.erase(meteorIt);
                }
                else {
                    ++meteorIt;
                }
                break;
            }
            else {
                ++meteorIt;
            }
        }

        if (bulletHit) {
            ++bulletIt;
        }
        else {
            ++bulletIt;
        }
    }
}